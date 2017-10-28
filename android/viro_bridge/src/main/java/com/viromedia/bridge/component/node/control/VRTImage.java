/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Looper;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.Image;
import com.viro.renderer.jni.Material;
import com.viro.renderer.jni.Surface;
import com.viro.renderer.jni.Texture.TextureFormat;
import com.viro.renderer.jni.Texture;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.ViroLog;

import java.util.List;

public class VRTImage extends VRTControl {
    private static final String TAG = ViroLog.getTag(VRTImage.class);
    static final String DEFAULT_RESIZE_MODE = "stretchToFill";
    static final String DEFAULT_CLIP_MODE = "clipToBounds";
    static final float DEFAULT_WIDTH = 1;
    static final float DEFAULT_HEIGHT = 1;
    private Material mDefaultMaterial;
    private Surface mNativeSurface;
    private Image mLatestImage;
    private Texture mLatestImageTexture;
    private String mStereoMode;
    private ReadableMap mSourceMap;
    private ReadableMap mPlaceholderSourceMap;
    private float mWidth = DEFAULT_WIDTH;
    private float mHeight = DEFAULT_HEIGHT;
    private float mScaledWidth = DEFAULT_WIDTH;
    private float mScaledHeight = DEFAULT_HEIGHT;
    private int mBitmapWidth;
    private int mBitmapHeight;
    private float mU0 = 0;
    private float mV0 = 0;
    private float mU1 = 1;
    private float mV1 = 1;
    private String mResizeMode = DEFAULT_RESIZE_MODE;
    private String mImageClipMode = DEFAULT_CLIP_MODE;
    private boolean mMipmap = true;
    private TextureFormat mFormat = TextureFormat.RGBA8;

    private boolean mGeometryNeedsUpdate = false;
    private boolean mIsImageSet = false;
    private boolean mWidthOrHeightPropSet = false;
    private boolean mImageNeedsDownload = false;
    private boolean mResizeModeSet = false;

    private PlaceholderImageDownloadListener mPlaceholderListener;
    private MainImageDownloadListener mMainListener;

    private Handler mMainHandler;

    public VRTImage(ReactApplicationContext context) {
        super(context);
        mDefaultMaterial = new Material();
        mMainHandler = new Handler(Looper.getMainLooper());
        mImageNeedsDownload = false;
    }

    public void setStereoMode(String mode){
        mStereoMode = mode;
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
        mScaledWidth = width;
        mGeometryNeedsUpdate = true;
        mWidthOrHeightPropSet = true;
    }

    public void setHeight(float height) {
        mHeight = height;
        mScaledHeight = height;
        mGeometryNeedsUpdate = true;
        mWidthOrHeightPropSet = true;
    }

    public void setResizeMode(String resizeMode) {
        mResizeMode = resizeMode == null ? "" : resizeMode;
        mGeometryNeedsUpdate = true;
        mResizeModeSet = true;
    }

    public void setImageClipMode(String imageClipMode) {
        mImageClipMode = imageClipMode == null ? "" : imageClipMode;
        mGeometryNeedsUpdate = true;
    }

    public void setMipmap(boolean mipmap) {
        mMipmap = mipmap;
    }

    public void setFormat(String format) {
        mFormat = TextureFormat.forString(format);
        mImageNeedsDownload = true;
    }

    @Override
    public void setMaterials(List<Material> materials) {
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
        if (isTornDown()) {
            return;
        }

        resizeImage();
        updateSurface();
        if (mImageNeedsDownload) {
            updateImage();
            mImageNeedsDownload = false;
        }
    }

    private void updateSurface() {
        boolean createdNewSurface = false;
        float imageSurfaceWidth;
        float imageSurfaceHeight;
        if (mImageClipMode.equalsIgnoreCase("clipToBounds") && mResizeMode.equalsIgnoreCase("scaleToFill")) {
            imageSurfaceWidth = mWidth;
            imageSurfaceHeight = mHeight;
        } else {
            imageSurfaceWidth = mScaledWidth;
            imageSurfaceHeight = mScaledHeight;
        }
        if (mNativeSurface == null) {
            mNativeSurface = new Surface(imageSurfaceWidth, imageSurfaceHeight, mU0, mV0, mU1, mV1);
            createdNewSurface = true;
        }
        else if (mGeometryNeedsUpdate) {
            Surface newSurface = new Surface(imageSurfaceWidth, imageSurfaceHeight, mU0, mV0, mU1, mV1, mNativeSurface);
            mNativeSurface.dispose();
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
            mPlaceholderListener = new PlaceholderImageDownloadListener(downloader);
            downloader.getImageAsync(mPlaceholderSourceMap, mPlaceholderListener);
        } else {
            downloadSourceImage(downloader);
        }
    }

    private void downloadSourceImage(ImageDownloader downloader) {
        if (mSourceMap != null) {
            imageDownloadDidStart();
            mMainListener = new MainImageDownloadListener();
            downloader.getImageAsync(mSourceMap, mMainListener);
        }

        // If no source was provided, just set the material
        else {
            setMaterialOnSurface();
        }
    }

    @Override
    public void onTearDown() {
        if (isTornDown()) {
            return;
        }

        // Cancel all downloaders before tearing down.
        if (mPlaceholderListener != null) {
            mPlaceholderListener.invalidate();
        }

        if (mMainListener != null) {
            mMainListener.invalidate();
        }
        super.onTearDown();

        if (mNativeSurface != null) {
            mNativeSurface.dispose();
            mNativeSurface = null;
        }

        if (mDefaultMaterial != null) {
            mDefaultMaterial.dispose();
            mDefaultMaterial = null;
        }

        if (mLatestImage != null) {
            mLatestImage.destroy();
            mLatestImage = null;
        }

        if (mLatestImageTexture != null) {
            mLatestImageTexture.dispose();
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
            Material nativeMaterial = mMaterials.get(0);
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
            mLatestImageTexture.dispose();
        }

        mLatestImage = new Image(image, mFormat);
        mLatestImageTexture = new Texture(mLatestImage, mFormat, true, mMipmap, mStereoMode);
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

    private void resizeImage() {
        if (!mWidthOrHeightPropSet || !mResizeModeSet || !mIsImageSet) {
            return;
        }
        float aspectRatio = (float) mBitmapWidth / (float) mBitmapHeight;
        float targetAspectRatio = mWidth / mHeight;

        switch (mResizeMode) {
            case "scaleToFit":
                if (targetAspectRatio <= aspectRatio) { // target is taller than content
                    mScaledWidth = mWidth;
                    mScaledHeight = mScaledWidth / aspectRatio;
                } else { // target is wider than content
                    mScaledHeight = mHeight;
                    mScaledWidth = mScaledHeight * aspectRatio;
                }
                break;
            case "scaleToFill":
                if (targetAspectRatio <= aspectRatio) { // target is taller than content
                    mScaledHeight = mHeight;
                    mScaledWidth = mScaledHeight * aspectRatio;
                } else { // target is wider than content
                    mScaledWidth = mWidth;
                    mScaledHeight = mScaledWidth / aspectRatio;
                }
                // If clipMode is set to clipToBounds, we need to calculate u,v values so that the
                // image clips/crops to the image view bounds
                if (mImageClipMode.equalsIgnoreCase("clipToBounds")) {
                    float clipWidth = Math.abs(mScaledWidth - mWidth) / mScaledWidth;
                    float clipHeight = Math.abs(mScaledHeight - mHeight)/ mScaledHeight;
                    mU0 = clipWidth / 2;
                    mV0 = clipHeight / 2;
                    mU1 = 1 - clipWidth / 2;
                    mV1 = 1 - clipHeight / 2;
                }
                break;
            default: // And stretchToFill
                mScaledWidth = mWidth;
                mScaledHeight = mHeight;
                mU0 = 0;
                mV0 = 0;
                mU1 = 1;
                mV1 = 1;
        }
    }

    /**
     * This is the ImageDownloadListener for the placeholder image
     */
    private class PlaceholderImageDownloadListener implements ImageDownloadListener {
        private boolean mIsValid = true;
        private ImageDownloader mDownloader;

        public PlaceholderImageDownloadListener(ImageDownloader downloader) {
            mDownloader = downloader;
        }

        public void invalidate() {
            mIsValid = false;
            mDownloader = null;
        }

        @Override
        public boolean isValid() {
            return mIsValid;
        }

        @Override
        public void completed(final Bitmap result) {
            if (!isValid()) {
                return;
            }
            mMainHandler.post(new Runnable() {
                public void run() {
                    if (!isValid()) {
                        return;
                    }
                    setImageOnSurface(result);
                    downloadSourceImage(mDownloader);
                    mPlaceholderListener = null;
                }
            });
        }

        @Override
        public void failed(String error) {
            if (!isValid()) {
                return;
            }
            onError(error);
        }
    }

    /**
     * This is the ImageDownloadListener for the main source image
     */
    private class MainImageDownloadListener implements ImageDownloadListener {
        private boolean mIsValid = true;

        public void invalidate() {
            mIsValid = false;
        }

        @Override
        public boolean isValid() {
            return mIsValid;
        }

        @Override
        public void completed(final Bitmap result) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    if (!isValid()) {
                        return;
                    }
                    mBitmapWidth = result.getWidth();
                    mBitmapHeight = result.getHeight();
                    mIsImageSet = true;

                    // If no width or height property was set, then base these on the
                    // image's aspect ratio and update the surface
                    if (!mWidthOrHeightPropSet) {
                        float ratio = (float) mBitmapWidth / (float) mBitmapHeight;
                        mHeight = mWidth / ratio;
                        mGeometryNeedsUpdate = true;
                        updateSurface();
                    } else if (mResizeModeSet) {
                        // If width and height props were set, along with resizeMode,
                        // we'll calculate scaled width & height of the image
                        resizeImage();
                        mGeometryNeedsUpdate = true;
                        updateSurface();
                    }

                    setMaterialOnSurface();
                    setImageOnSurface(result);
                    imageDownloadDidFinish();
                    mMainListener = null;
                }
            });
        }

        @Override
        public void failed(String error) {
            if (!isValid()) {
                return;
            }
            onError(error);
        }
    }
}
