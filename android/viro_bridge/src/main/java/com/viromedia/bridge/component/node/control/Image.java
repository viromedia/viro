/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import android.graphics.Bitmap;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.ImageJni;
import com.viro.renderer.jni.MaterialJni;
import com.viro.renderer.jni.SurfaceJni;
import com.viro.renderer.jni.TextureJni;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.ViroLog;

import java.util.List;

public class Image extends Control {
    private static final String TAG = ViroLog.getTag(Image.class);
    private final MaterialJni mDefaultMaterial;
    private SurfaceJni mNativeSurface;
    private ImageJni mLatestImage;
    private TextureJni mLatestImageTexture;

    private ReadableMap mSourceMap;
    private ReadableMap mPlaceholderSourceMap;
    private float mWidth = 1;
    private float mHeight = 1;

    private boolean mGeometryNeedsUpdate = false;
    private boolean mIsImageSet = false;

    public Image(ReactApplicationContext context) {
        super(context);
        mDefaultMaterial = new MaterialJni();
    }

    public void setSource(ReadableMap source) {
        mSourceMap = source;
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
}

    public void setHeight(float height) {
        mHeight = height;
        mGeometryNeedsUpdate = true;
    }

    @Override
    public void setMaterials(List<MaterialJni> materials) {
        // Override materials setting because we want to control which materials are set.
        mMaterials = materials;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        final ImageDownloader downloader = new ImageDownloader(getContext());

        if (mNativeSurface == null) {
            mNativeSurface = new SurfaceJni(mWidth, mHeight);
        } else if (mGeometryNeedsUpdate) {
            SurfaceJni newSurface = new SurfaceJni(mWidth, mHeight, mNativeSurface);
            mNativeSurface.destroy();
            mNativeSurface = newSurface;
        }
        getNodeJni().setGeometry(mNativeSurface);
        mGeometryNeedsUpdate = false;

        // If an image isn't already set, then first fetch the placeholder (which should be on disk)
        // before downloading/fetching the source image. Otherwise, just immediately get the source.
        if (!mIsImageSet && mPlaceholderSourceMap != null) {
            downloader.getImageAsync(mPlaceholderSourceMap, new ImageDownloadListener() {
                @Override
                public void completed(Bitmap result) {
                    setImageOnSurface(result);
                    downloadSourceImage(downloader);
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
                public void completed(Bitmap result) {
                    mIsImageSet = true;
                    setMaterialOnSurface();
                    setImageOnSurface(result);
                    imageDownloadDidFinish();
                }
            });
        }
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mNativeSurface != null) {
            mNativeSurface.destroy();
            mNativeSurface = null;
        }
        if (mDefaultMaterial != null) {
            mDefaultMaterial.destroy();
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

        mLatestImage = new ImageJni(image);
        mLatestImageTexture = new TextureJni(mLatestImage);
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
