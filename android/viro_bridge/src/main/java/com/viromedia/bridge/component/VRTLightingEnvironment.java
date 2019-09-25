//  Copyright © 2018 Viro Media. All rights reserved.
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

import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.PortalScene;
import com.viro.core.Texture;
import com.viro.core.internal.Image;
import com.viromedia.bridge.component.node.VRTNode;
import com.viromedia.bridge.component.node.VRTScene;
import com.viromedia.bridge.utility.HdrImageDownloader;
import com.viromedia.bridge.utility.ViroEvents;

public class VRTLightingEnvironment extends VRTNode {
    private ReadableMap mSourceMap;
    private Image mLatestImage;
    private Texture mLatestTexture;
    private boolean mImageNeedsDownload;
    private IBLImageDownloadListener mHdrImageDownloadListener;
    private PortalScene mTargetedPortalScene = null;
    private boolean mHasSetScene;

    public VRTLightingEnvironment(ReactContext context) {
        super(context);
        mImageNeedsDownload = false;
        mHasSetScene = false;
    }

    public void setSource(ReadableMap source) {
        mSourceMap = source;
        mImageNeedsDownload = true;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if (!mImageNeedsDownload || mSourceMap == null || !mHasSetScene) {
            return;
        }

        if (mHdrImageDownloadListener != null){
            mHdrImageDownloadListener.invalidate();
        }

        imageDownloadDidStart();

        mHdrImageDownloadListener = new IBLImageDownloadListener();
        HdrImageDownloader.getHdrTextureAsync(mSourceMap, mHdrImageDownloadListener, getContext());
        mImageNeedsDownload = false;
    }

    @Override
    public void onTearDown() {
        if (mTargetedPortalScene != null && mTargetedPortalScene.getNativeRef() != 0L) {
            mTargetedPortalScene.setLightingEnvironment(null);
            mTargetedPortalScene = null;
        }

        super.onTearDown();
        if (mHdrImageDownloadListener != null) {
            mHdrImageDownloadListener.invalidate();
            mHdrImageDownloadListener = null;
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
        mHasSetScene = true;
        this.onPropsSet();
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

    private class IBLImageDownloadListener implements HdrImageDownloader.DownloadListener {
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
                if (mLatestTexture != null) {
                    mLatestTexture.dispose();
                }
                mLatestTexture = result;

                // Set the loaded image onto the lighting environment
                if (getNodeJni() != null) {
                    PortalScene portal = getNodeJni().getParentPortalScene();
                    mTargetedPortalScene = portal;

                    if (portal != null) {
                        portal.setLightingEnvironment(mLatestTexture);
                    }
                }

                // Notify callbacks
                imageDownloadDidFinish();
            }

            mHdrImageDownloadListener = null;
        }
    }
}
