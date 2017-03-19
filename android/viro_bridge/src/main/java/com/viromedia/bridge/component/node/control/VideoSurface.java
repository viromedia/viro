/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.RenderContextJni;
import com.viro.renderer.jni.SurfaceJni;
import com.viro.renderer.jni.VideoTextureJni;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;

import java.lang.ref.WeakReference;

public class VideoSurface extends Control {

    private static class VideoSurfaceDelegate implements VideoTextureJni.VideoDelegate {

        private WeakReference<VideoSurface> mSurface;

        public VideoSurfaceDelegate(VideoSurface surface) {
            mSurface = new WeakReference<VideoSurface>(surface);
        }

        @Override
        public void onVideoFinish() {
            VideoSurface surface = mSurface.get();
            if (surface == null || surface.isTornDown()) {
                return;
            }
            surface.playerDidFinishPlaying();
        }

        @Override
        public void onReady() {
            VideoSurface surface = mSurface.get();
            if (surface == null || surface.isTornDown()) {
                return;
            }
            surface.loadVideo();
        }

        @Override
        public void onVideoUpdatedTime(int currentTime, int totalVideoTime) {
            VideoSurface surface = mSurface.get();
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
    private SurfaceJni mSurfaceJni = null;
    private VideoTextureJni mVideoTextureJni = null;
    private VideoTextureJni.VideoDelegate mDelegate = null;
    private boolean mGeometryNeedsUpdate = false;

    public VideoSurface(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    public void onTearDown(){
        if (mSurfaceJni != null) {
            mSurfaceJni.destroy();
            mSurfaceJni = null;
        }
        if (mVideoTextureJni != null){
            mVideoTextureJni.delete();
            mVideoTextureJni = null;
        }
        super.onTearDown();
    }

    private void resetVideo() {
        if (mRenderContext == null || mSource == null) {
            return;
        }

        if (mVideoTextureJni != null) {
            mVideoTextureJni.delete();
            mVideoTextureJni = null;
        }

        if (mSurfaceJni != null) {
            mSurfaceJni.destroy();
            mSurfaceJni = null;
        }

        // Create Texture
        mSurfaceJni = new SurfaceJni(mWidth, mHeight);
        getNodeJni().setGeometry(mSurfaceJni);
        mVideoTextureJni = new VideoTextureJni();

        mDelegate = new VideoSurfaceDelegate(this);
        mVideoTextureJni.setVideoDelegate(mDelegate);
    }

    private void loadVideo(){
        mSurfaceJni.setVideoTexture(mVideoTextureJni);
        mVideoTextureJni.loadSource(mSource, mRenderContext);
        setVolume(mVolume);
        setLoop(mLoop);
        setMuted(mMuted);
        setVolume(mVolume);
        setPaused(mPaused);
    }

    @Override
    public void setRenderContext(RenderContextJni context) {
        super.setRenderContext(context);
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

    public void setSource(String source) {
        mSource = Helper.parseUri(source, getContext()).toString();
        mGeometryNeedsUpdate = true;
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

    public void seekToTime(int time) {
        if (mVideoTextureJni != null) {
            mVideoTextureJni.seekToTime(time);
            if (!mPaused) {
                mVideoTextureJni.play();
            }
        }
    }

    private void playerDidFinishPlaying() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_FINISH,
                null);
    }

    private void playerOnUpdateTime(int currentTime, int totalTime) {
        WritableMap event = Arguments.createMap();
        event.putInt("currentTime", currentTime);
        event.putInt("totalTime", totalTime);

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
