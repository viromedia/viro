/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.RenderContextJni;
import com.viro.renderer.jni.VideoTextureJni;
import com.viromedia.bridge.component.node.Scene;

/**
 * Contains a VideoTexture that is set as a background video on the scene.
 */
public class Video360 extends Component {
    private String mSource;
    private boolean mPaused;
    private boolean mLoop;
    private boolean mMuted;
    private float mVolume;

    private VideoTextureJni videoTextureJni = null;
    private ReactApplicationContext mContext;

    public Video360(ReactApplicationContext reactContext) {
        super(reactContext.getBaseContext());
        mContext = reactContext;
    }

    @Override
    protected void onTearDown(){
        if (videoTextureJni != null){
            videoTextureJni.delete();
            videoTextureJni = null;
        }
        super.onTearDown();
    }

    private void resetVideo() {
        if (mRenderContext == null || mSource == null) {
            return;
        }

        if (videoTextureJni != null) {
            videoTextureJni.delete();
            videoTextureJni = null;
        }

        // Create Texture
        videoTextureJni = new VideoTextureJni();
        videoTextureJni.loadSource(mSource, mRenderContext);
        videoTextureJni.setVideoDelegate(new VideoTextureJni.VideoDelegate() {
            @Override
            public void onVideoFinish() {
                reactVideoFinishedCallback();
            }
        });
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

    @Override
    public void setScene(Scene scene){
        super.setScene(scene);
        mScene.setBackgroundVideoTexture(videoTextureJni);
    }

    public void setSource(String source) {
        mSource = source;
        resetVideo();
    }

    public void setPaused(boolean paused) {
        mPaused = paused;

        if (videoTextureJni == null) {
            return;
        }

        if (mPaused) {
            videoTextureJni.pause();
        } else {
            videoTextureJni.play();
        }
    }

    public void setLoop(boolean loop) {
        mLoop = loop;
        if (videoTextureJni != null) {
            videoTextureJni.setLoop(loop);
        }
    }

    public void setMuted(boolean muted) {
        mMuted = muted;
        if (videoTextureJni != null) {
            videoTextureJni.setMuted(muted);
        }
    }

    public void setVolume(float volume) {
        mVolume = volume;
        if (videoTextureJni != null) {
            videoTextureJni.setVolume(volume);
        }
    }

    public void seekToTime(int time) {
        if (videoTextureJni != null) {
            videoTextureJni.seekToTime(time);
        }
    }

    private void reactVideoFinishedCallback() {
        mContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                Video360Manager.VIDEO_FINISHED_CALLBACK,
                null);
    }
}
