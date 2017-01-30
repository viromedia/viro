/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import android.content.Context;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.BaseSoundJni;
import com.viro.renderer.jni.RenderContextJni;
import com.viro.renderer.jni.SoundDelegate;
import com.viro.renderer.jni.SoundJni;
import com.viromedia.bridge.utility.ViroEvents;

public abstract class BaseSound extends Component implements SoundDelegate {

    protected static final String NAME = "name";
    protected static final String URI = "uri";
    protected static final String LOCAL_PREFIX = "file";
    protected static final String WEB_PREFIX = "http";

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

    public BaseSound(Context context) {
        super(context, null, -1, -1, null);
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
            // TODO: VIRO-749 only destroy if your original source isn't preloaded.
            mNativeSound.destroy();
        }

        mShouldResetSound = false;

        // figure out what type of audio I have
        if (mSource.hasKey(NAME)) {
            mNativeSound = getSoundForName(mSource.getString(NAME));
            if (mNativeSound == null) {
                throw new IllegalArgumentException("Unknown Sound source with name: ["
                        + mSource.getString(NAME) + "]");
            }
            mNativeSound.setDelegate(this);
        } else if (mSource.hasKey(URI)) {
            String uri = mSource.getString(URI);
            if (uri.startsWith(LOCAL_PREFIX)) {
                // TODO: VIRO-756 validate on release builds that the local prefix is correct
                mNativeSound = getNativeSound(uri, true);
            } else if (uri.startsWith(WEB_PREFIX)) {
                mNativeSound = getNativeSound(uri, false);
            } else {
                throw new IllegalArgumentException("Unknown path to sound: [" + uri + "]");
            }
        } else {
            throw new IllegalArgumentException("Unknown sound source.");
        }

        setNativeProps();
    }

    private SoundJni getSoundForName(String name) {
        // TODO: VIRO-749 do this when you enable preloading
        return null;
    }

    @Override
    public void onSoundReady() {
        mReady = true;
        if (mNativeSound != null && !mPaused) {
            mNativeSound.play();
        }
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
}
