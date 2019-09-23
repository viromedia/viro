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

package com.viromedia.bridge.component;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Looper;

import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.internal.Image;
import com.viro.core.PortalScene;
import com.viro.core.Texture;
import com.viromedia.bridge.component.node.VRTNode;
import com.viromedia.bridge.component.node.VRTScene;
import com.viromedia.bridge.utility.HdrImageDownloader;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;
import com.viromedia.bridge.utility.ViroEvents;

public class VRT360Image extends VRTNode {
    private static final float[] sDefaultRotation = {0, 0, 0};

    private ReadableMap mSourceMap;
    private float[] mRotation = sDefaultRotation;
    private Image mLatestImage;
    private Texture mLatestTexture;
    private String mStereoMode;
    private Texture.Format mFormat = Texture.Format.RGBA8;
    private Handler mMainHandler;
    private boolean mImageNeedsDownload;
    private HDRImageDownloadListener mHDRDownloadListener;
    private Image360DownloadListener mImageDownloadListener;
    private boolean mIsHdr;

    public VRT360Image(ReactContext context) {
        super(context);
        mMainHandler = new Handler(Looper.getMainLooper());
        mImageNeedsDownload = false;
        mIsHdr = false;
    }

    public void setStereoMode(String mode){
        mStereoMode = mode;
    }

    public void setIsHdr(boolean hdr){
        mIsHdr = hdr;
    }

    public void setSource(ReadableMap source) {
        mSourceMap = source;
        mImageNeedsDownload = true;
    }

    public void setRotation(ReadableArray rotation) {
        if (rotation == null) {
            mRotation = sDefaultRotation;
        } else {
            float[] rotationArr = {(float) rotation.getDouble(0),
                    (float) rotation.getDouble(1), (float) rotation.getDouble(2)};
            mRotation = rotationArr;
        }
        if (getNodeJni() != null) {
            PortalScene portal = getNodeJni().getParentPortalScene();
            if (portal != null) {
                portal.setBackgroundRotation(Helper.toRadiansVector(mRotation));
            }
        }
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if (!mImageNeedsDownload || mSourceMap == null || mScene == null) {
            return;
        }

        imageDownloadDidStart();
        invalidateImageDownloadListeners();

        final Context context = getContext();
        if (mIsHdr){
            mHDRDownloadListener = new HDRImageDownloadListener();
            HdrImageDownloader.getHdrTextureAsync(mSourceMap, mHDRDownloadListener, context);
        } else {
            ImageDownloader downloader = new ImageDownloader(context);
            downloader.setTextureFormat(mFormat);
            mImageDownloadListener = new Image360DownloadListener();
            downloader.getImageAsync(mSourceMap, mImageDownloadListener);
        }

        mImageNeedsDownload = false;
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        invalidateImageDownloadListeners();

        if (mLatestImage != null) {
            mLatestImage.destroy();
            mLatestImage = null;
        }

        if (mLatestTexture != null) {
            mLatestTexture.dispose();
            mLatestTexture = null;
        }
    }

    private void invalidateImageDownloadListeners(){
        if (mImageDownloadListener != null) {
            mImageDownloadListener.invalidate();
            mImageDownloadListener = null;
        }

        if (mHDRDownloadListener != null){
            mHDRDownloadListener.invalidate();
            mHDRDownloadListener = null;
        }
    }

    @Override
    public void setScene(VRTScene scene) {
        super.setScene(scene);
        this.onPropsSet();
    }

    public void setFormat(String format) {
        mFormat = Texture.Format.forString(format);
        mImageNeedsDownload = true;
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

    private class Image360DownloadListener implements ImageDownloadListener {
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
                public void run() {
                    if (!isValid()) {
                        return;
                    }

                    if (mLatestImage != null) {
                        mLatestImage.destroy();
                    }

                    if (mLatestTexture != null) {
                        mLatestTexture.dispose();
                    }

                    mLatestImage = new Image(result, mFormat);
                    setBackgroundTexture(new Texture(mLatestImage, true, false, mStereoMode));

                    imageDownloadDidFinish();
                    mImageDownloadListener = null;
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

    private void setBackgroundTexture(Texture texture){
        mLatestTexture = texture;

        if (getNodeJni() != null) {
            PortalScene portal = getNodeJni().getParentPortalScene();
            if (portal != null) {
                portal.setBackgroundTexture(mLatestTexture);
                portal.setBackgroundRotation(Helper.toRadiansVector(mRotation));
            }
        }
    }

    private class HDRImageDownloadListener implements HdrImageDownloader.DownloadListener {
        private boolean mIsValid = true;
        public void invalidate() {
            mIsValid = false;
        }

        @Override
        public boolean isValid() {
            return mIsValid;
        }

        @Override
        public void completed(Texture result) {
            if (result == null){
                onError("Viro: Error loading hdr file.");
                return;
            } else {
                if (mLatestImage != null) {
                    mLatestImage.destroy();
                    mLatestImage = null;
                }

                if (mLatestTexture != null) {
                    mLatestTexture.dispose();
                    mLatestTexture = null;
                }

                setBackgroundTexture(result);
                imageDownloadDidFinish();
            }

            mImageDownloadListener = null;
        }
    }
}
