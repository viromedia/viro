/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.net.Uri;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.Texture;
import com.viro.renderer.jni.ViroContext;
import com.viro.renderer.jni.Surface;
import com.viro.renderer.jni.VideoTexture;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;

import java.lang.ref.WeakReference;

public class VRTVideoSurface extends VRTControl {

    private static class VideoSurfaceDelegate implements VideoTexture.Delegate {

        private WeakReference<VRTVideoSurface> mSurface;

        public VideoSurfaceDelegate(VRTVideoSurface surface) {
            mSurface = new WeakReference<VRTVideoSurface>(surface);
        }

        @Override
        public void onVideoBufferStart(VideoTexture video) {
            VRTVideoSurface surface = mSurface.get();
            if (surface == null || surface.isTornDown()) {
                return;
            }
            surface.playerBufferStart();
        }

        @Override
        public void onVideoBufferEnd(VideoTexture video) {
            VRTVideoSurface surface = mSurface.get();
            if (surface == null || surface.isTornDown()) {
                return;
            }
            surface.playerBufferEnd();
        }

        @Override
        public void onVideoFinish(VideoTexture video) {
            VRTVideoSurface surface = mSurface.get();
            if (surface == null || surface.isTornDown()) {
                return;
            }
            surface.playerDidFinishPlaying();
        }

        @Override
        public void onReady(VideoTexture video) {

        }

        @Override
        public void onVideoFailed(String error) {
            VRTVideoSurface surface = mSurface.get();
            if (surface == null || surface.isTornDown()) {
                return;
            }
            surface.onError(error);
        }

        @Override
        public void onVideoUpdatedTime(VideoTexture video, float currentTime, float totalVideoTime) {
            VRTVideoSurface surface = mSurface.get();
            if (surface == null || surface.isTornDown()) {
                return;
            }
            surface.playerOnUpdateTime(currentTime, totalVideoTime);
        }
    }

    private float mWidth = 1;
    private float mHeight = 1;
    private boolean mPaused = false;
    private boolean mLoop = false;
    private boolean mMuted = false;
    private float mVolume = 1;
    private String mSource;
    private Surface mSurface = null;
    private VideoTexture mVideoTexture = null;
    private VideoTexture.Delegate mDelegate = null;
    private String mStereoMode;
    private boolean mGeometryNeedsUpdate = false;

    public VRTVideoSurface(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    public void onTearDown(){
        if (mSurface != null) {
            mSurface.dispose();
            mSurface = null;
        }
        if (mVideoTexture != null){
            mVideoTexture.dispose();
            mVideoTexture = null;
        }
        super.onTearDown();
    }

    private void resetVideo() {
        if (mViroContext == null || mSource == null) {
            return;
        }

        if (mVideoTexture != null) {
            mVideoTexture.dispose();
            mVideoTexture = null;
        }

        if (mSurface != null) {
            mSurface.dispose();
            mSurface = null;
        }

        // Create Texture
        mSurface = new Surface(mWidth, mHeight, 0, 0, 1, 1);
        getNodeJni().setGeometry(mSurface);
        mDelegate = new VideoSurfaceDelegate(this);

        mVideoTexture = new VideoTexture(mViroContext, Uri.parse(mSource), mDelegate,
                Texture.StereoMode.valueFromString(mStereoMode));
        loadVideo();

        mVideoTexture.setDelegate(mDelegate);
    }

    private void loadVideo(){
        mSurface.setVideoTexture(mVideoTexture);
        setVolume(mVolume);
        setLoop(mLoop);
        setMuted(mMuted);
        setVolume(mVolume);
        setPaused(mPaused);
    }

    @Override
    public void setViroContext(ViroContext context) {
        super.setViroContext(context);
        resetVideo();
    }

    public void setWidth(float width) {
        mWidth = width;
        mGeometryNeedsUpdate = true;
    }

    public void setHeight(float height) {
        mHeight = height;
        mGeometryNeedsUpdate = true;
    }

    public void setStereoMode(String mode){
        mStereoMode = mode;
    }

    public void setSource(String source) {
        mSource = Helper.parseUri(source, getContext()).toString();
        mGeometryNeedsUpdate = true;
    }

    public void setPaused(boolean paused) {
        mPaused = paused;
        if (mVideoTexture == null) {
            return;
        }

        if (mPaused || !shouldAppear()) {
            mVideoTexture.pause();
        } else {
            mVideoTexture.play();
        }
    }

    public void setLoop(boolean loop) {
        mLoop = loop;
        if (mVideoTexture != null) {
            mVideoTexture.setLoop(loop);

            if (!mPaused) {
                mVideoTexture.play();
            }
        }
    }

    public void setMuted(boolean muted) {
        mMuted = muted;
        if (mVideoTexture != null) {
            mVideoTexture.setMuted(muted);
        }
    }

    public void setVolume(float volume) {
        mVolume = volume;
        if (mVideoTexture != null) {
            mVideoTexture.setVolume(volume);
        }
    }

    public void seekToTime(float time) {
        if (mVideoTexture != null) {
            mVideoTexture.seekToTime(time);
            if (!mPaused) {
                mVideoTexture.play();
            }
        }
    }

    @Override
    protected void handleAppearanceChange() {
        setPaused(mPaused);
        super.handleAppearanceChange();
    }

    private void playerBufferStart() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_BUFFER_START,
                null);
    }

    private void playerBufferEnd() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_BUFFER_END,
                null);
    }

    private void playerDidFinishPlaying() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_FINISH,
                null);
    }

    private void playerOnUpdateTime(float currentTime, float totalTime) {
        WritableMap event = Arguments.createMap();
        event.putDouble("currentTime", (double) currentTime);
        event.putDouble("totalTime", (double) totalTime);

        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_UPDATE_TIME,
                event);
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if(mGeometryNeedsUpdate) {
            resetVideo();
        }
        mGeometryNeedsUpdate = false;
    }

    @Override
    public void onHostPause(){
        super.onHostPause();
        if (mVideoTexture != null) {
            mVideoTexture.pause();
        }
    }

    @Override
    public void sceneWillDisappear() {
        if (mVideoTexture != null){
            mVideoTexture.pause();
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
