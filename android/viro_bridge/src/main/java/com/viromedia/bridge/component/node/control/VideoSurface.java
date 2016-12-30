/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.RenderContextJni;
import com.viro.renderer.jni.SurfaceJni;
import com.viro.renderer.jni.VideoTextureJni;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.ViroLog;

public class VideoSurface extends Control {
    private final float UNSET = -1f;
    private float mWidth = UNSET;
    private float mHeight = UNSET;
    private boolean mPaused;
    private boolean mLoop;
    private boolean mMuted;
    private float mVolume;
    private String mSource;
    private SurfaceJni mSurfaceJni = null;
    private VideoTextureJni mVideoTextureJni = null;

    public VideoSurface(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    protected void onTearDown(){
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
        if (mRenderContext == null || mWidth == UNSET || mHeight == UNSET || mSource == null) {
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
        mVideoTextureJni = new VideoTextureJni();
        mVideoTextureJni.loadSource(mSource, mRenderContext);
        mVideoTextureJni.setVideoDelegate(new VideoTextureJni.VideoDelegate() {
            @Override
            public void onVideoFinish() {
                playerDidFinishPlaying();
            }
        });

        // Create surface and apply video texture
        mSurfaceJni = new SurfaceJni(mWidth, mHeight);
        mSurfaceJni.setVideoTexture(mVideoTextureJni);
        getNodeJni().setGeometry(mSurfaceJni);
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
        resetVideo();
    }

    public void setHeight(float height) {
        mHeight = height;
        resetVideo();
    }

    public void setSource(String source) {
        mSource = source;
        resetVideo();
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
        }
    }

    private void playerDidFinishPlaying() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_FINISH,
                null);
    }

    @Override
    public void sceneWillDisappear() {
        if (mVideoTextureJni != null){
            mVideoTextureJni.pause();
        }
    }

}
