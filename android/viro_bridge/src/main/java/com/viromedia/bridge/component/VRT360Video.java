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

import android.net.Uri;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.PortalScene;
import com.viro.core.Texture;
import com.viro.core.Vector;
import com.viro.core.ViroContext;
import com.viro.core.VideoTexture;
import com.viromedia.bridge.component.node.VRTNode;
import com.viromedia.bridge.component.node.VRTScene;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;

import java.lang.ref.WeakReference;

/**
 * Contains a VideoTexture that is set as a background video on the scene.
 */
public class VRT360Video extends VRTNode {
    private static final float[] sDefaultRotation = {0, 0, 0};

    private static class Video360Delegate implements VideoTexture.PlaybackListener {

        private WeakReference<VRT360Video> mVideo;

        public Video360Delegate(VRT360Video video) {
            mVideo = new WeakReference<VRT360Video>(video);
        }

        public void onVideoBufferStart(VideoTexture videoTexture) {
            VRT360Video video = mVideo.get();
            if (video == null || video.isTornDown()) {
                return;
            }

            video.reactVideoBufferStartCallback();
        }

        @Override
        public void onVideoBufferEnd(VideoTexture videoTexture) {
            VRT360Video video = mVideo.get();
            if (video == null || video.isTornDown()) {
                return;
            }

            video.reactVideoBufferEndCallback();
        }

        @Override
        public void onVideoFinish(VideoTexture videoTexture) {
            VRT360Video video = mVideo.get();
            if (video == null || video.isTornDown()) {
                return;
            }

            video.reactVideoFinishedCallback();
        }

        @Override
        public void onReady(VideoTexture videoTexture) {

        }

        @Override
        public void onVideoFailed(String error) {
            VRT360Video video = mVideo.get();
            if (video == null || video.isTornDown()) {
                return;
            }
            video.onError(error);
        }

        @Override
        public void onVideoUpdatedTime(VideoTexture videoTexture, float currentTime, float totalVideoTime) {
            VRT360Video video = mVideo.get();
            if (video == null || video.isTornDown()) {
                return;
            }
            video.reactPlayerOnUpdateTime(currentTime, totalVideoTime);
        }
    }

    private boolean mNeedsVideoTextureReset = false;
    private String mSource;
    private boolean mPaused = false;
    private boolean mLoop = false;
    private boolean mMuted = false;
    private float mVolume = 1;
    private float[] mRotation = sDefaultRotation;

    private VideoTexture mVideoTextureJni = null;
    private VideoTexture.PlaybackListener mDelegate = null;
    private String mStereoMode;

    public VRT360Video(ReactContext reactContext) {
        super(reactContext);
    }

    @Override
    public void onTearDown() {
        if (mVideoTextureJni != null){
            mVideoTextureJni.dispose();
            mVideoTextureJni = null;
        }
        super.onTearDown();
    }

    private void resetVideo() {
        if (mViroContext == null || mSource == null) {
            return;
        }

        if (mVideoTextureJni != null) {
            mVideoTextureJni.dispose();
            mVideoTextureJni = null;
        }

        // Create Texture
        mDelegate = new Video360Delegate(this);
        mVideoTextureJni = new VideoTexture(mViroContext, Uri.parse(mSource), mDelegate, Texture.StereoMode.valueFromString(mStereoMode));
        mVideoTextureJni.setPlaybackListener(mDelegate);
        updateVideoTexture();
        setLoop(mLoop);
        setMuted(mMuted);
        setVolume(mVolume);
        setPaused(mPaused);

        if (getNodeJni() != null) {
            PortalScene portal = getNodeJni().getParentPortalScene();
            if (portal != null) {
                updateVideoTexture();
                portal.setBackgroundRotation(Helper.toRadiansVector(mRotation));
            }
        }
    }

    @Override
    public void setViroContext(ViroContext context) {
        super.setViroContext(context);
        resetVideo();
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if(mNeedsVideoTextureReset) {
            resetVideo();
        }
        mNeedsVideoTextureReset = false;
    }


    @Override
    public void setScene(VRTScene scene){
        super.setScene(scene);
        updateVideoTexture();

        PortalScene portal = getNodeJni() != null ? getNodeJni().getParentPortalScene() : null;
        if (portal != null) {
            portal.setBackgroundRotation(Helper.toRadiansVector(mRotation));
        }
    }

    private void updateVideoTexture() {
        PortalScene portal = getNodeJni() != null ? getNodeJni().getParentPortalScene() : null;
        if (portal != null && mVideoTextureJni != null) {
            portal.setBackgroundTexture(mVideoTextureJni);
        }
    }

    public void setStereoMode(String mode){
        mStereoMode = mode;
        mNeedsVideoTextureReset = true;
    }

    public void setSource(String source) {
        mSource = Helper.parseUri(source, getContext()).toString();
        mNeedsVideoTextureReset = true;
    }

    public void setPaused(boolean paused) {
        mPaused = paused;
        if (mVideoTextureJni == null) {
            return;
        }

        if (mPaused) {
            mVideoTextureJni.pause();
        } else {
            mVideoTextureJni.play();
        }
    }

    public void setLoop(boolean loop) {
        mLoop = loop;
        if (mVideoTextureJni != null) {
            mVideoTextureJni.setLoop(loop);

            if (!mPaused) {
                mVideoTextureJni.play();
            }
        }
    }

    public void setMuted(boolean muted) {
        mMuted = muted;
        if (mVideoTextureJni != null) {
            mVideoTextureJni.setMuted(muted);
        }
    }

    public void setVolume(float volume) {
        mVolume = volume;
        if (mVideoTextureJni != null) {
            mVideoTextureJni.setVolume(volume);
        }
    }

    public void seekToTime(float time) {
        if (mVideoTextureJni != null) {
            mVideoTextureJni.seekToTime(time);
            if (!mPaused) {
                mVideoTextureJni.play();
            }
        }
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

    private void reactVideoBufferStartCallback() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_BUFFER_START,
                null);
    }

    private void reactVideoBufferEndCallback() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_BUFFER_END,
                null);
    }

    private void reactVideoFinishedCallback() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_FINISH,
                null);
    }

    private void reactPlayerOnUpdateTime(float currentTime, float totalTime) {
        WritableMap event = Arguments.createMap();
        event.putDouble("currentTime", (double) currentTime);
        event.putDouble("totalTime", (double) totalTime);
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_UPDATE_TIME,
                event);
    }

    @Override
    public void onHostPause(){
        super.onHostPause();
        if (mVideoTextureJni != null) {
            mVideoTextureJni.pause();
        }
    }

    @Override
    public void sceneWillDisappear() {
        if (mVideoTextureJni != null){
            mVideoTextureJni.pause();
        }
    }

    @Override
    public void onHostResume(){
        super.onHostResume();
        setPaused(mPaused);
    }

    @Override
    public void sceneWillAppear() {
        setPaused(mPaused);
    }
}
