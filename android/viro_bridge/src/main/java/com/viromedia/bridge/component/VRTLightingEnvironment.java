/**
 * Copyright © 2018 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.net.Uri;
import android.os.AsyncTask;
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
import com.viromedia.bridge.utility.ViroEvents;

public class VRTLightingEnvironment extends VRTNode {
    private ReadableMap mSourceMap;
    private Image mLatestImage;
    private Texture mLatestTexture;
    private Handler mMainHandler;
    private boolean mImageNeedsDownload;
    private DownloadFilesTask mImageDownloaderTask;
    private PortalScene mTargetedPortalScene = null;
    private boolean mHasSetScene;

    public VRTLightingEnvironment(ReactApplicationContext context) {
        super(context);
        mMainHandler = new Handler(Looper.getMainLooper());
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

        if (mImageDownloaderTask != null){
            mImageDownloaderTask.invalidate();
        }

        imageDownloadDidStart();
        mImageDownloaderTask = new DownloadFilesTask();
        mImageDownloaderTask.execute(Uri.parse(mSourceMap.getString("uri")));
        mImageNeedsDownload = false;
    }

    @Override
    public void onTearDown() {
        if (getNodeJni() != null) {
            mTargetedPortalScene.setLightingEnvironment(null);
            mTargetedPortalScene = null;
        }

        super.onTearDown();
        if (mImageDownloaderTask != null) {
            mImageDownloaderTask.invalidate();
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

    private class DownloadFilesTask extends AsyncTask<Uri, Integer, Texture> {
        private boolean mIsValid = true;
        private String mTargetUri;

        public void invalidate() {
            mIsValid = false;
        }

        protected Texture doInBackground(Uri... urls) {
            mTargetUri = mSourceMap.getString("uri");
            return Texture.loadRadianceHDRTexture(Uri.parse(mTargetUri));
        }

        protected void onPostExecute(Texture result) {
            if (!mIsValid) {
                return;
            }


            if (result == null){
                onError("Viro: There was an error loading hdr file: " + mTargetUri);
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

            mImageDownloaderTask = null;
        }
    }
}
