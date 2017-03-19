/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.utility;


import android.content.Context;
import android.graphics.Bitmap;
import android.net.Uri;

import com.facebook.common.references.CloseableReference;
import com.facebook.datasource.BaseDataSubscriber;
import com.facebook.datasource.DataSource;
import com.facebook.datasource.DataSubscriber;
import com.facebook.drawee.backends.pipeline.Fresco;
import com.facebook.imagepipeline.core.DefaultExecutorSupplier;
import com.facebook.imagepipeline.core.ImagePipeline;
import com.facebook.imagepipeline.image.CloseableBitmap;
import com.facebook.imagepipeline.image.CloseableImage;
import com.facebook.imagepipeline.request.ImageRequest;
import com.facebook.imagepipeline.request.ImageRequestBuilder;
import com.facebook.react.bridge.ReadableMap;
import com.viro.renderer.jni.TextureFormat;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CountDownLatch;

/**
 * This class downloads images and returns them as @{link Bitmap} objects
 * by leveraging the Facebook Fresco image downloading/caching library.
 */
public class ImageDownloader {
    private static final String TAG = ViroLog.getTag(ImageDownloader.class);
    private static final String URI_KEY = "uri";
    private final Context mContext;
    private final ConcurrentHashMap<CountDownLatch, Bitmap> mImageMap;
    private final DefaultExecutorSupplier mExecutorSupplier;
    private Bitmap.Config mConfig = Bitmap.Config.ARGB_8888;

    public ImageDownloader(Context context) {
        mContext = context;
        mImageMap = new ConcurrentHashMap<>();
        mExecutorSupplier = new DefaultExecutorSupplier(1);

    }

    /**
     * This method fetches an image synchronously.
     *
     * @param map a ReadableMap with a "uri" key, ideally the same one we get from the JS layer
     * @return the bitmap containing the image data
     */
    public Bitmap getImageSync(ReadableMap map) {
        if (!map.hasKey(URI_KEY)) {
            throw new IllegalArgumentException("Unable to find \"uri\" key in given source map.");
        }
        return getImageSync(Helper.parseUri(map.getString(URI_KEY), mContext));
    }

    /**
     * This method fetches an image synchronously.
     *
     * @param uri a URI representing the location of the image to fetch.
     * @return the bitmap containing the image data.
     */
    public Bitmap getImageSync(Uri uri) {
        CountDownLatch latch = new CountDownLatch(1);
        getImage(uri, latch, null);
        try {
            latch.await();
        } catch (InterruptedException e) {
            throw new IllegalStateException("Fetching bitmap was interrupted!");
        }
        Bitmap toReturn = mImageMap.get(latch);
        mImageMap.remove(latch);
        return toReturn.copy(mConfig, false);
    }

    /**
     * This method fetches an image asynchrously
     *
     * @param map a ReadableMap with a "uri" key, ideally the same one we get from the JS layer
     * @param listener object that will be called once the image is fetched.
     */
    public void getImageAsync(ReadableMap map, ImageDownloadListener listener) {
        if (listener == null) {
            ViroLog.warn(TAG, "The given ImageDownloadListener is null. Doing nothing.");
            return;
        }

        if (!map.hasKey(URI_KEY)) {
            throw new IllegalArgumentException("Unable to find \"uri\" key in given source map.");
        }

        getImage(Helper.parseUri(map.getString(URI_KEY), mContext), null, listener);
    }

    private void getImage(Uri uri, final CountDownLatch latch, final ImageDownloadListener listener) {
        ImagePipeline imagePipeline = Fresco.getImagePipeline();
        ImageRequest request = ImageRequestBuilder.newBuilderWithSource(uri).build();
        DataSource<CloseableReference<CloseableImage>> dataSource = imagePipeline.fetchDecodedImage(request, mContext);

        DataSubscriber<CloseableReference<CloseableImage>> dataSubscriber =
                new BaseDataSubscriber<CloseableReference<CloseableImage>>() {
                    @Override
                    protected void onNewResultImpl(DataSource<CloseableReference<CloseableImage>> dataSource) {
                        if (!dataSource.isFinished()) {
                            return;
                        }
                        CloseableImage image = dataSource.getResult().get();
                        if (image instanceof CloseableBitmap) {
                            Bitmap bitmap = ((CloseableBitmap) image).getUnderlyingBitmap();
                            if (listener != null) {
                                listener.completed(bitmap.copy(mConfig, true));
                            } else {
                                mImageMap.put(latch, bitmap);
                            }
                        }

                        dataSource.close();
                        if (latch != null) {
                            latch.countDown();
                        }
                    }

                    @Override
                    protected void onFailureImpl(DataSource<CloseableReference<CloseableImage>> dataSource) {
                        if (latch != null) {
                            latch.countDown();
                        }
                        Throwable t = dataSource.getFailureCause();
                        if (listener != null) {
                            listener.failed(t.getMessage());
                        }
                    }
                };

        dataSource.subscribe(dataSubscriber, mExecutorSupplier.forBackgroundTasks());
    }

    public void setTextureFormat(TextureFormat format) {
        if (format == TextureFormat.RGB565) {
            mConfig = Bitmap.Config.RGB_565;
        }
        else {
            mConfig = Bitmap.Config.ARGB_8888;
        }
    }
}
