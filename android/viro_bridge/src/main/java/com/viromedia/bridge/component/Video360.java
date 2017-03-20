/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.RenderContextJni;
import com.viro.renderer.jni.VideoTextureJni;
import com.viromedia.bridge.component.node.Scene;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;

import java.lang.ref.WeakReference;

/**
 * Contains a VideoTexture that is set as a background video on the scene.
 */
public class Video360 extends Component {
    private static final float[] sDefaultRotation = {0, 0, 0};

    private static class Video360Delegate implements VideoTextureJni.VideoDelegate {

        private WeakReference<Video360> mVideo;

        public Video360Delegate(Video360 video) {
            mVideo = new WeakReference<Video360>(video);
        }

        @Override
        public void onVideoFinish() {
            Video360 video = mVideo.get();
            if (video == null || video.isTornDown()) {
                return;
            }

            video.reactVideoFinishedCallback();
        }

        @Override
        public void onReady() {
            Video360 video = mVideo.get();
            if (video == null || video.isTornDown()) {
                return;
            }

            video.updateVideoTexture();
            video.mVideoTextureJni.loadSource(video.mSource, video.mRenderContext);
            video.setLoop(video.mLoop);
            video.setMuted(video.mMuted);
            video.setVolume(video.mVolume);
            video.setPaused(video.mPaused);
        }

        @Override
        public void onVideoFailed(String error) {
            Video360 video = mVideo.get();
            if (video == null || video.isTornDown()) {
                return;
            }
            video.onError(error);
        }

        @Override
        public void onVideoUpdatedTime(int currentTime, int totalVideoTime) {
            Video360 video = mVideo.get();
            if (video == null || video.isTornDown()) {
                return;
            }
            video.reactPlayerOnUpdateTime(currentTime, totalVideoTime);
        }
    }

    private String mSource;
    private boolean mPaused = false;
    private boolean mLoop = false;
    private boolean mMuted = false;
    private float mVolume = 1;
    private float[] mRotation = sDefaultRotation;

    private VideoTextureJni mVideoTextureJni = null;
    private VideoTextureJni.VideoDelegate mDelegate = null;

    public Video360(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    public void onTearDown() {
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

        // Create Texture
        mVideoTextureJni = new VideoTextureJni(mRenderContext);

        mDelegate = new Video360Delegate(this);
        mVideoTextureJni.setVideoDelegate(mDelegate);

        if (mScene != null) {
            updateVideoTexture();
            mScene.setBackgroundRotation(mRotation);
        }
    }

    @Override
    public void setRenderContext(RenderContextJni context) {
        super.setRenderContext(context);
        resetVideo();
    }

    @Override
    public void setScene(Scene scene){
        super.setScene(scene);
        updateVideoTexture();
        mScene.setBackgroundRotation(mRotation);
    }

    private void updateVideoTexture(){
        if (mScene != null && mVideoTextureJni != null && mVideoTextureJni.isReady()) {
            mScene.setBackgroundVideoTexture(mVideoTextureJni);
        }
    }

    public void setSource(String source) {
        mSource = Helper.parseUri(source, getContext()).toString();
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

    public void setRotation(ReadableArray rotation) {
        if (rotation == null) {
            mRotation = sDefaultRotation;
        } else {
            float[] rotationArr = {(float) rotation.getDouble(0),
                    (float) rotation.getDouble(1), (float) rotation.getDouble(2)};
            mRotation = rotationArr;
        }
        if (mScene != null) {
            mScene.setBackgroundRotation(mRotation);
        }
    }

    private void reactVideoFinishedCallback() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_FINISH,
                null);
    }

    private void reactPlayerOnUpdateTime(int currentTime, int totalTime) {
        WritableMap event = Arguments.createMap();
        event.putInt("currentTime", currentTime);
        event.putInt("totalTime", totalTime);
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
