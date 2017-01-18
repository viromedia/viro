/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.utility;


import android.content.Context;
import android.graphics.Bitmap;

import com.facebook.common.executors.CallerThreadExecutor;
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
import com.facebook.react.bridge.ReadableMap;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CountDownLatch;

/**
 * This class downloads images and returns them as @{link Bitmap} objects
 * by leveraging the Facebook Fresco image downloading/caching library.
 */
public class ImageDownloader {
    private static final String TAG = ViroLog.getTag(ImageDownloader.class);
    private final Context mContext;
    private final ConcurrentHashMap<CountDownLatch, Bitmap> mImageMap;
    private final DefaultExecutorSupplier mExecutorSupplier;


    public ImageDownloader(Context context) {
        mContext = context;
        mImageMap = new ConcurrentHashMap<>();
        mExecutorSupplier = new DefaultExecutorSupplier(1);

    }

    public Bitmap getImageSync(ReadableMap map) {
        CountDownLatch latch = new CountDownLatch(1);
        getImage(map, latch, null);
        try {
            latch.await();
        } catch (InterruptedException e) {
            throw new IllegalStateException("Fetching bitmap was interrupted!");
        }
        Bitmap toReturn = mImageMap.get(latch);
        mImageMap.remove(latch);
        return toReturn;
    }

    public void getImageAsync(ReadableMap map, ImageDownloadListener listener) {
        if (listener == null) {
            ViroLog.warn(TAG, "The given ImageDownloadListener is null. Doing nothing.");
            return;
        }
        getImage(map, null, listener);
    }

    private void getImage(ReadableMap map, final CountDownLatch latch, final ImageDownloadListener listener) {
        ImagePipeline imagePipeline = Fresco.getImagePipeline();
        ImageRequest request = ImageRequest.fromUri(map.getString("uri"));
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
                                listener.completed(bitmap.copy(bitmap.getConfig(), true));
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
                        throw new IllegalStateException("Error downloading image: " + t.getMessage());
                    }
                };

        dataSource.subscribe(dataSubscriber, mExecutorSupplier.forBackgroundTasks());
    }
}
