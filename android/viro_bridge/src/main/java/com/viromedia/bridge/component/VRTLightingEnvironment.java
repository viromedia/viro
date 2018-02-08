/**
 * Copyright © 2018 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Looper;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.PortalScene;
import com.viro.core.Texture;
import com.viro.core.internal.Image;
import com.viromedia.bridge.component.node.VRTNode;
import com.viromedia.bridge.component.node.VRTScene;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;
import com.viromedia.bridge.utility.ViroEvents;

public class VRTLightingEnvironment extends VRTNode {
    private ReadableMap mSourceMap;
    private Image mLatestImage;
    private Texture mLatestTexture;
    private Handler mMainHandler;
    private boolean mImageNeedsDownload;
    private ImageHDRDownloadListener mDownloadListener;

    public VRTLightingEnvironment(ReactApplicationContext context) {
        super(context);
        mMainHandler = new Handler(Looper.getMainLooper());
        mImageNeedsDownload = false;
    }

    public void setSource(ReadableMap source) {
        mSourceMap = source;
        mImageNeedsDownload = true;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if (!mImageNeedsDownload || mSourceMap == null) {
            return;
        }

        if (mDownloadListener != null){
            mDownloadListener.invalidate();
        }

        ImageDownloader downloader = new ImageDownloader(getContext());
        downloader.setTextureFormat(Texture.Format.RGB9_E5);
        imageDownloadDidStart();

        mDownloadListener = new ImageHDRDownloadListener();
        downloader.getImageAsync(mSourceMap, mDownloadListener);

        mImageNeedsDownload = false;
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mDownloadListener != null) {
            mDownloadListener.invalidate();
        }

        if (mLatestImage != null) {
            mLatestImage.destroy();
            mLatestImage = null;
        }

        if (mLatestTexture != null) {
            mLatestTexture.dispose();
            mLatestTexture = null;
        }
    }

    @Override
    public void setScene(VRTScene scene) {
        super.setScene(scene);
        if (mLatestTexture != null && getNodeJni() != null) {
            PortalScene portal = getNodeJni().getParentPortalScene();
            if (portal != null) {
                portal.setBackgroundTexture(mLatestTexture);
            }
        }
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

    private class ImageHDRDownloadListener implements ImageDownloadListener {
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

                    mLatestImage = new Image(result, Texture.Format.RGB9_E5);
                    mLatestTexture = new Texture(mLatestImage, Texture.Format.RGB9_E5, false, false);

                    if (getNodeJni() != null) {
                        PortalScene portal = getNodeJni().getParentPortalScene();
                        if (portal != null) {
                            portal.setLightingEnvironment(mLatestTexture);
                        }
                    }
                    imageDownloadDidFinish();
                    mDownloadListener = null;
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
