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
import android.net.Uri;
import android.util.Log;
import com.facebook.react.bridge.ReactContext;
import com.viro.core.AnimatedTexture;
import com.viro.core.Texture;
import com.viro.core.ViroContext;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;
import com.viromedia.bridge.utility.ViroLog;

public class VRTAnimatedImage extends VRTImage {
    private static final String TAG = ViroLog.getTag(VRTAnimatedImage.class);
    private AnimatedTextureLoadCallback mMainAnimatedTextureDownloadListener;
    private AnimatedImagePlaceholderCallback mAnimatedImagePlaceholderListener;
    private boolean mPaused;
    private boolean mLoop;
    private boolean mPendingTextureLoad;

    public VRTAnimatedImage(ReactContext context) {
        super(context);
        mLatestImageTexture = null;
        mPendingTextureLoad = false;
        mPaused = false;
        mLoop = true;
    }

    public void setPaused(boolean paused) {
        mPaused = paused;
        if (mLatestImageTexture == null) {
            return;
        }

        AnimatedTexture animTexture = (AnimatedTexture) mLatestImageTexture;
        if (mPaused || !shouldAppear()) {
            animTexture.pause();
        } else {
            animTexture.play();
        }
    }

    public void setLoop(boolean loop) {
        mLoop = loop;
        if (mLatestImageTexture == null) {
            return;
        }

        AnimatedTexture animTexture = (AnimatedTexture) mLatestImageTexture;
        animTexture.setLoop(loop);
    }

    @Override
    protected void handleAppearanceChange() {
        setPaused(mPaused);
        super.handleAppearanceChange();
    }

    @Override
    public void onPropsSet() {
        // Because props are set from the bottom of the scene tree up, if props are set on this
        // node, then we know its the top-most changed element.
        mIsTopMostChangedElement = true;
        setChildrenNotTopMostChangedElement();

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

    @Override
    public void updateImage() {
        final ImageDownloader downloader = new ImageDownloader(getContext());
        downloader.setTextureFormat(Texture.Format.RGBA8);

        // If an image isn't already set, then first fetch the placeholder (which should be on disk)
        // before downloading/fetching the source image. Otherwise, just immediately get the source.
        if (!mIsImageSet && mPlaceholderSourceMap != null && mSourceMap != null) {
            mAnimatedImagePlaceholderListener = new AnimatedImagePlaceholderCallback(downloader);
            downloader.getImageAsync(mPlaceholderSourceMap, mAnimatedImagePlaceholderListener);
        } else {
            mPendingTextureLoad = true;
            downloadSourceImage(downloader);
        }
    }

    @Override
    void downloadSourceImage(ImageDownloader downloader) {
        // If no source was provided, just set the material
        if (mSourceMap == null) {
            setMaterialOnQuad();
            return;
        }

        if (mViroContext == null) {
            return;
        }

        // Else, start downloading the animated image source.
        imageDownloadDidStart();

        // Grab and validate the uri from the source map.
        final String URI_KEY = "uri";
        if (!mSourceMap.hasKey(URI_KEY)) {
            throw new IllegalArgumentException("Unable to find \"uri\" key in given source map.");
        }

        Uri sourceUri = Helper.parseUri(mSourceMap.getString(URI_KEY), getContext());
        mMainAnimatedTextureDownloadListener = new AnimatedTextureLoadCallback();
        mLatestImageTexture = new AnimatedTexture(mViroContext, sourceUri, mMainAnimatedTextureDownloadListener);
        mPendingTextureLoad = false;
    }

    @Override
    public void setViroContext(ViroContext context) {
        super.setViroContext(context);

        if (mPendingTextureLoad) {
            downloadSourceImage(null);
        }
    }

    @Override
    public void onTearDown() {
        if (isTornDown()) {
            return;
        }

        if (mMainAnimatedTextureDownloadListener != null) {
            mMainAnimatedTextureDownloadListener.invalidate();
        }

        if (mAnimatedImagePlaceholderListener != null) {
            mAnimatedImagePlaceholderListener.invalidate();
        }

        super.onTearDown();
    }

    @Override
    void setImageOnQuad(Bitmap image) {
        Log.w(TAG,"Unable to set Bitmaps for AnimatedImages!");
    }

    @Override
    public void setMipmap(boolean mipmap) {
        Log.w(TAG,"Unable to set mipmap levels for AnimatedImages!");
    }

    @Override
    public void setFormat(String format) {
        Log.w(TAG,"Unable to set image formats for AnimatedImages!");
    }

    /**
     * This is the ImageDownloadListener for the placeholder image
     */
    class AnimatedImagePlaceholderCallback implements ImageDownloadListener {
        private boolean mIsValid = true;
        private ImageDownloader mDownloader;

        public AnimatedImagePlaceholderCallback(ImageDownloader downloader) {
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
                    mPendingTextureLoad = true;
                    downloadSourceImage(mDownloader);
                    mAnimatedImagePlaceholderListener = null;
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

    private class AnimatedTextureLoadCallback implements AnimatedTexture.OnLoadComplete {
        private boolean mIsValid = true;

        public boolean isValid() {
            return mIsValid;
        }

        public void invalidate() {
            mIsValid = false;
        }

        @Override
        public void onSuccess(final AnimatedTexture animatedTexture) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    if (!mIsValid) {
                        return;
                    }

                    mIsImageSet = true;
                    updateMainImageDimensions(animatedTexture.getWidth(), animatedTexture.getHeight());

                    setMaterialOnQuad();
                    if (mNativeQuad != null) {
                        mNativeQuad.setImageTexture(animatedTexture);
                    }
                    imageDownloadDidFinish();
                    mMainAnimatedTextureDownloadListener = null;

                    setLoop(mLoop);
                    setPaused(mPaused);
                }
            });
        }

        @Override
        public void onFailure(final String error) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    if (!mIsValid) {
                        return;
                    }
                    onError(error);
                }
            });
        }
    }
}
