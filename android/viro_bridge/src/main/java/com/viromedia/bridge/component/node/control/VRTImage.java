//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

package com.viromedia.bridge.component.node.control;


import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Looper;

import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.internal.Image;
import com.viro.core.Material;
import com.viro.core.Quad;
import com.viro.core.Texture;
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
    private Image mLatestImage;
    private String mStereoMode;
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
    private Texture.Format mFormat = Texture.Format.RGBA8;

    private boolean mGeometryNeedsUpdate = false;
    private boolean mWidthOrHeightPropSet = false;
    private boolean mResizeModeSet = false;
    private MainImageDownloadListener mMainListener;
    private PlaceholderImageDownloadListener mPlaceholderListener;

    boolean mImageNeedsDownload = false;
    Quad mNativeQuad;
    boolean mIsImageSet = false;
    Handler mMainHandler;
    Texture mLatestImageTexture;
    ReadableMap mPlaceholderSourceMap;
    ReadableMap mSourceMap;

    public VRTImage(ReactContext context) {
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
        mFormat = Texture.Format.forString(format);
        mImageNeedsDownload = true;
    }

    @Override
    public void setMaterials(List<Material> materials) {
        // Override materials setting because we want to control which materials are set.
        mMaterials = materials;
        setMaterialOnQuad();
        if (mNativeQuad != null && mLatestImageTexture != null) {
            mNativeQuad.setImageTexture(mLatestImageTexture);
        }
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if (isTornDown()) {
            return;
        }

        resizeImage();
        updateQuad();
        if (mImageNeedsDownload) {
            updateImage();
            mImageNeedsDownload = false;
        }
    }

    void updateQuad() {
        boolean createdNewQuad = false;
        float imageQuadWidth;
        float imageQuadHeight;
        if (mImageClipMode.equalsIgnoreCase("clipToBounds") && mResizeMode.equalsIgnoreCase("scaleToFill")) {
            imageQuadWidth = mWidth;
            imageQuadHeight = mHeight;
        } else {
            imageQuadWidth = mScaledWidth;
            imageQuadHeight = mScaledHeight;
        }
        if (mNativeQuad == null) {
            mNativeQuad = new Quad(imageQuadWidth, imageQuadHeight, mU0, mV0, mU1, mV1);
            createdNewQuad = true;
        }
        else if (mGeometryNeedsUpdate) {
            Quad newQuad = new Quad(imageQuadWidth, imageQuadHeight, mU0, mV0, mU1, mV1, mNativeQuad);
            mNativeQuad.dispose();
            mNativeQuad = newQuad;
            createdNewQuad = true;
        }

        if (createdNewQuad) {
            getNodeJni().setGeometry(mNativeQuad);
            if (mLatestImageTexture != null) {
                mNativeQuad.setImageTexture(mLatestImageTexture);
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

    void downloadSourceImage(ImageDownloader downloader) {
        if (mSourceMap != null) {
            imageDownloadDidStart();
            mMainListener = new MainImageDownloadListener();
            downloader.getImageAsync(mSourceMap, mMainListener);
        }

        // If no source was provided, just set the material
        else {
            setMaterialOnQuad();
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

        if (mNativeQuad != null) {
            mNativeQuad.dispose();
            mNativeQuad = null;
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

    void setMaterialOnQuad() {
        if (mNativeQuad == null) {
            return;
        }
        if (mMaterials == null || mMaterials.size() == 0) {
            // set default (empty) material
            mNativeQuad.setMaterial(mDefaultMaterial);
        } else {
            Material nativeMaterial = mMaterials.get(0);
            mNativeQuad.setMaterial(nativeMaterial);
        }
    }

    void setImageOnQuad(Bitmap image) {
        if (mNativeQuad == null) {
            return;
        }

        if (mLatestImage != null) {
            mLatestImage.destroy();
        }
        if (mLatestImageTexture != null) {
            mLatestImageTexture.dispose();
        }

        mLatestImage = new Image(image, mFormat);
        mLatestImageTexture = new Texture(mLatestImage, true, mMipmap, mStereoMode);
        mNativeQuad.setImageTexture(mLatestImageTexture);
    }

    void imageDownloadDidStart() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_LOAD_START,
                null
        );
    }

    void imageDownloadDidFinish() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_LOAD_END,
                null
        );
    }

    void resizeImage() {
        if (!mWidthOrHeightPropSet || !mResizeModeSet || !mIsImageSet) {
            return;
        }
        float aspectRatio = (float) mBitmapWidth / (float) mBitmapHeight;
        float targetAspectRatio = mWidth / mHeight;

        if (mResizeMode != null && mResizeMode.equalsIgnoreCase("scaleToFit")) {
            if (targetAspectRatio <= aspectRatio) { // target is taller than content
                mScaledWidth = mWidth;
                mScaledHeight = mScaledWidth / aspectRatio;
            } else { // target is wider than content
                mScaledHeight = mHeight;
                mScaledWidth = mScaledHeight * aspectRatio;
            }
        } else if(mResizeMode != null && mResizeMode.equalsIgnoreCase("scaleToFill")) {
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
        } else {
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
    class PlaceholderImageDownloadListener implements ImageDownloadListener {
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
                    setImageOnQuad(result);
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

    void updateMainImageDimensions(int width, int height) {
        mBitmapWidth = width;
        mBitmapHeight = height;

        // If no width or height property was set, then base these on the
        // image's aspect ratio and update the Quad
        if (!mWidthOrHeightPropSet) {
            float ratio = (float) mBitmapWidth / (float) mBitmapHeight;
            mHeight = mWidth / ratio;
            mGeometryNeedsUpdate = true;
            updateQuad();
        } else if (mResizeModeSet) {
            // If width and height props were set, along with resizeMode,
            // we'll calculate scaled width & height of the image
            resizeImage();
            mGeometryNeedsUpdate = true;
            updateQuad();
        }
    }

    /**
     * This is the ImageDownloadListener for the main source image
     */
    class MainImageDownloadListener implements ImageDownloadListener {
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

                    mIsImageSet = true;
                    updateMainImageDimensions(result.getWidth(), result.getHeight());

                    setMaterialOnQuad();
                    setImageOnQuad(result);

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
