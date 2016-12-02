/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.view.View;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.RenderContextJni;
import com.viro.renderer.jni.VideoSurfaceJni;

public class VideoSurface extends Control {
    private final float UNSET = -1f;
    private float mWidth = UNSET;
    private float mHeight = UNSET;
    private boolean mPaused;
    private boolean mLoop;
    private boolean mMuted;
    private float mVolume;
    private String mSource;
    private VideoSurfaceJni mVideoSurfaceJni = null;
    private ReactApplicationContext mContext;

    public VideoSurface(ReactApplicationContext reactContext) {
        super(reactContext.getBaseContext());
        mContext = reactContext;
    }

    public void tearDown() {
        if (mVideoSurfaceJni != null) {
            mVideoSurfaceJni.delete();
        }
    }

    private void resetVideo() {
        if (mRenderContext == null || mWidth == UNSET || mHeight == UNSET || mSource == null) {
            return;
        }

        if (mVideoSurfaceJni != null) {
            mVideoSurfaceJni.delete();
        }

        mVideoSurfaceJni = new VideoSurfaceJni(mWidth, mHeight, mSource, mRenderContext);
        getNodeJni().setGeometry(mVideoSurfaceJni);
        setLoop(mLoop);
        setMuted(mMuted);
        setVolume(mVolume);
        setPaused(mPaused);
        mVideoSurfaceJni.setVideoDelegate(new VideoSurfaceJni.VideoDelegate() {
            @Override
            public void onVideoFinish() {
                reactVideoFinishedCallback();
            }
        });
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

        if (mVideoSurfaceJni == null) {
            return;
        }

        if (mPaused) {
            mVideoSurfaceJni.pause();
        } else {
            mVideoSurfaceJni.play();
        }
    }

    public void setLoop(boolean loop) {
        mLoop = loop;
        if (mVideoSurfaceJni != null) {
            mVideoSurfaceJni.setLoop(loop);
        }
    }

    public void setMuted(boolean muted) {
        mMuted = muted;
        if (mVideoSurfaceJni != null) {
            mVideoSurfaceJni.setMuted(muted);
        }
    }

    public void setVolume(float volume) {
        mVolume = volume;
        if (mVideoSurfaceJni != null) {
            mVideoSurfaceJni.setVolume(volume);
        }
    }

    public void seekToTime(int time) {
        if (mVideoSurfaceJni != null) {
            mVideoSurfaceJni.seekToTime(time);
        }
    }

    private void reactVideoFinishedCallback() {
        mContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                VideoSurfaceManager.VIDEO_FINISHED_CALLBACK,
                null);
    }
}
