/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.net.Uri;

import com.facebook.react.bridge.JSApplicationCausedNativeException;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.BaseSoundJni;
import com.viro.renderer.jni.RenderContextJni;
import com.viro.renderer.jni.SoundDataJni;
import com.viro.renderer.jni.SoundDelegate;
import com.viromedia.bridge.module.SoundModule;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.ViroLog;

public abstract class BaseSound extends Component implements SoundDelegate {

    private static final String TAG = ViroLog.getTag(BaseSound.class);
    protected static final String NAME = "name";
    protected static final String URI = "uri";

    protected BaseSoundJni mNativeSound;
    protected ReadableMap mSource;
    protected boolean mPaused = false;
    protected float mVolume = 1.0f;
    protected boolean mMuted = false;
    protected boolean mLoop = false;

    protected boolean mShouldResetSound = false;
    protected boolean mReady = false;

    public BaseSound(ReactApplicationContext reactContext) {
        super(reactContext.getBaseContext(), null, -1, -1, reactContext);
    }

    public void setSource(ReadableMap source) {
        mSource = source;
        mShouldResetSound = true;
    }

    public void setPaused(boolean paused) {
        mPaused = paused;
    }

    public void setVolume(float volume) {
        mVolume = volume;
    }

    public void setMuted(boolean muted) {
        mMuted = muted;
    }

    public void setLoop(boolean loop) {
        mLoop = loop;
    }

    public void seekToTime(int seconds) {
        if (mNativeSound != null) {
            mNativeSound.seekToTime(seconds);
            if (!mPaused) {
                mNativeSound.play();
            }
        } else {
            // We should *NEVER* get in this case, so let's log in case.
            ViroLog.warn(TAG, "seekToTime called before nativeSound was created!");
        }
    }

    @Override
    public void onPropsSet() {
        if (mShouldResetSound) {
            resetSound();
        }
        setNativeProps();
    }

    protected void setNativeProps() {
        if (mNativeSound == null) {
            return;
        }

        if (mReady) {
            if (mPaused) {
                mNativeSound.pause();
            } else {
                mNativeSound.play();
            }
        }

        mNativeSound.setVolume(mVolume);
        mNativeSound.setMuted(mMuted);
        mNativeSound.setLoop(mLoop);
    }

    @Override
    public void setRenderContext(RenderContextJni context) {
        super.setRenderContext(context);
        resetSound();
    }

    protected void resetSound() {
        if (mRenderContext == null) {
            return;
        }
        if (mNativeSound != null) {
            mNativeSound.pause();
            mNativeSound.destroy();
            mNativeSound = null;
        }

        mShouldResetSound = false;

        // figure out what type of audio I have
        if (mSource.hasKey(NAME)) {
            SoundDataJni data = getSoundDataForName(mSource.getString(NAME));
            if (data == null) {
                throw new JSApplicationCausedNativeException("Unknown Sound source with name: ["
                        + mSource.getString(NAME) + "]");
            }

            mNativeSound = getNativeSound(data);
            mNativeSound.setDelegate(this);
        } else if (mSource.hasKey(URI)) {
            Uri uri = Helper.parseUri(mSource.getString(URI), getContext());
            mNativeSound = getNativeSound(uri.toString(), Helper.isResourceUri(uri));
        } else {
            throw new IllegalArgumentException("Unknown sound source.");
        }

        setNativeProps();
    }

    private SoundDataJni getSoundDataForName(String name) {
        SoundModule soundModule = mReactContext.getNativeModule(SoundModule.class);
        return soundModule.getSoundData(name);
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mNativeSound != null) {
            mNativeSound.destroy();
            mNativeSound = null;
        }
    }

    @Override
    public void onSoundReady() {
        mReady = true;
        if (mNativeSound != null && !mPaused) {
            mNativeSound.play();
        }
    }

    @Override
    public void onHostPause(){
        super.onHostPause();
        if (mNativeSound != null && !mPaused) {
            mNativeSound.pause();
        }
    }

    @Override
    public void sceneWillDisappear() {
        if (mNativeSound != null && !mPaused) {
            mNativeSound.pause();
        }
    }

    @Override
    public void onHostResume(){
        super.onHostResume();
        onPropsSet();
    }

    @Override
    public void sceneWillAppear() {
        onPropsSet();
    }

    @Override
    public void onSoundFinish() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(getId(),
                ViroEvents.ON_FINISH,
                null);
    }

    /**
     * Child classes should implement this method to return their own type of native Sound object
     *
     * @param path - path/url to the sound file
     * @param local - whether or not the file is local
     * @return the native sound object
     */
    protected abstract BaseSoundJni getNativeSound(String path, boolean local);

    protected abstract BaseSoundJni getNativeSound(SoundDataJni data);
}
