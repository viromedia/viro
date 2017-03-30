/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Looper;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.ImageJni;
import com.viro.renderer.jni.MaterialJni;
import com.viro.renderer.jni.RenderContextJni;
import com.viro.renderer.jni.SurfaceJni;
import com.viro.renderer.jni.TextureFormat;
import com.viro.renderer.jni.TextureJni;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.ViroLog;

import java.util.List;

public class Image extends Control {
    private static final String TAG = ViroLog.getTag(Image.class);
    private MaterialJni mDefaultMaterial;
    private SurfaceJni mNativeSurface;
    private ImageJni mLatestImage;
    private TextureJni mLatestImageTexture;

    private ReadableMap mSourceMap;
    private ReadableMap mPlaceholderSourceMap;
    private float mWidth = 1;
    private float mHeight = 1;

    private boolean mMipmap = true;
    private TextureFormat mFormat = TextureFormat.RGBA8;

    private boolean mGeometryNeedsUpdate = false;
    private boolean mIsImageSet = false;
    private boolean mWidthOrHeightPropSet = false;
    private boolean mImageNeedsDownload = false;

    private Handler mMainHandler;

    public Image(ReactApplicationContext context) {
        super(context);
        mDefaultMaterial = new MaterialJni();
        mMainHandler = new Handler(Looper.getMainLooper());
        mImageNeedsDownload = false;
    }

    public void setSource(ReadableMap source) {
        mSourceMap = source;
        mImageNeedsDownload = true;
    }

    public void setPlaceholderSource(ReadableMap placeholderSource) {
        if (placeholderSource == null) {
            ViroLog.warn(TAG, "PlaceholderSource shouldn't be null. We should've provided a default.");
        }
        mPlaceholderSourceMap = placeholderSource;
    }

    public void setWidth(float width) {
        mWidth = width;
        mGeometryNeedsUpdate = true;
        mWidthOrHeightPropSet = true;
    }

    public void setHeight(float height) {
        mHeight = height;
        mGeometryNeedsUpdate = true;
        mWidthOrHeightPropSet = true;
    }

    public void setMipmap(boolean mipmap) {
        mMipmap = mipmap;
    }

    public void setFormat(String format) {
        mFormat = TextureFormat.forString(format);
        mImageNeedsDownload = true;
    }

    @Override
    public void setMaterials(List<MaterialJni> materials) {
        // Override materials setting because we want to control which materials are set.
        mMaterials = materials;
        setMaterialOnSurface();
        if (mNativeSurface != null && mLatestImageTexture != null) {
            mNativeSurface.setImageTexture(mLatestImageTexture);
        }
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        updateSurface();
        if (mImageNeedsDownload) {
            updateImage();
            mImageNeedsDownload = false;
        }
    }

    private void updateSurface() {
        boolean createdNewSurface = false;
        if (mNativeSurface == null) {
            mNativeSurface = new SurfaceJni(mWidth, mHeight, 0, 0, 1, 1);
            createdNewSurface = true;
        }
        else if (mGeometryNeedsUpdate) {
            SurfaceJni newSurface = new SurfaceJni(mWidth, mHeight, 0, 0, 1, 1, mNativeSurface);
            mNativeSurface.destroy();
            mNativeSurface = newSurface;
            createdNewSurface = true;
        }

        if (createdNewSurface) {
            getNodeJni().setGeometry(mNativeSurface);
            if (mLatestImageTexture != null) {
                mNativeSurface.setImageTexture(mLatestImageTexture);
            }
        }
        mGeometryNeedsUpdate = false;
    }

    public void updateImage() {
        final ImageDownloader downloader = new ImageDownloader(getContext());
        downloader.setTextureFormat(mFormat);

        // If an image isn't already set, then first fetch the placeholder (which should be on disk)
        // before downloading/fetching the source image. Otherwise, just immediately get the source.
        if (!mIsImageSet && mPlaceholderSourceMap != null && mSourceMap != null) {
            downloader.getImageAsync(mPlaceholderSourceMap, new ImageDownloadListener() {
                @Override
                public void completed(final Bitmap result) {
                    if (isTornDown()) {
                        return;
                    }
                    mMainHandler.post(new Runnable() {
                        public void run() {
                            setImageOnSurface(result);
                            downloadSourceImage(downloader);
                        }
                    });
                }

                @Override
                public void failed(String error) {
                    if (isTornDown()) {
                        return;
                    }
                    onError(error);
                }
            });
        } else {
            downloadSourceImage(downloader);
        }
    }

    private void downloadSourceImage(ImageDownloader downloader) {
        if (mSourceMap != null) {
            imageDownloadDidStart();
            downloader.getImageAsync(mSourceMap, new ImageDownloadListener() {
                @Override
                public void completed(final Bitmap result) {
                    mMainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            if (isTornDown()) {
                                return;
                            }
                            mIsImageSet = true;

                            // If no width or height property was set, then base these on the
                            // image's aspect ratio and update the surface
                            if (!mWidthOrHeightPropSet) {
                                float ratio = (float) result.getWidth() / (float) result.getHeight();
                                mHeight = mWidth / ratio;
                                mGeometryNeedsUpdate = true;
                                updateSurface();
                            }
 
                            setMaterialOnSurface();
                            setImageOnSurface(result);
                            imageDownloadDidFinish();
                        }
                    });
                }

                @Override
                public void failed(String error) {
                    if (isTornDown()) {
                        return;
                    }
                    onError(error);
                }
            });
        }

        // If no source was provided, just set the material
        else {
            setMaterialOnSurface();
        }
    }

    @Override
    public void setRenderContext(RenderContextJni context) {
        super.setRenderContext(context);
    }

    @Override
    public void onTearDown() {
        if (isTornDown()) {
            return;
        }
        super.onTearDown();
        if (mNativeSurface != null) {
            mNativeSurface.destroy();
            mNativeSurface = null;
        }
        if (mDefaultMaterial != null) {
            mDefaultMaterial.destroy();
            mDefaultMaterial = null;
        }
        if (mLatestImage != null) {
            mLatestImage.destroy();
            mLatestImage = null;
        }
        if (mLatestImageTexture != null) {
            mLatestImageTexture.destroy();
            mLatestImageTexture = null;
        }
    }

    private void setMaterialOnSurface() {
        if (mNativeSurface == null) {
            return;
        }
        if (mMaterials == null || mMaterials.size() == 0) {
            // set default (empty) material
            mNativeSurface.setMaterial(mDefaultMaterial);
        } else {
            MaterialJni nativeMaterial = mMaterials.get(0);
            mNativeSurface.setMaterial(nativeMaterial);
        }
    }

    private void setImageOnSurface(Bitmap image) {
        if (mNativeSurface == null) {
            return;
        }

        if (mLatestImage != null) {
            mLatestImage.destroy();
        }
        if (mLatestImageTexture != null) {
            mLatestImageTexture.destroy();
        }

        mLatestImage = new ImageJni(image, mFormat);
        mLatestImageTexture = new TextureJni(mLatestImage, mFormat, mMipmap);
        mNativeSurface.setImageTexture(mLatestImageTexture);
    }

    private void imageDownloadDidStart() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_LOAD_START,
                null
        );
    }

    private void imageDownloadDidFinish() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_LOAD_END,
                null
        );
    }
}
