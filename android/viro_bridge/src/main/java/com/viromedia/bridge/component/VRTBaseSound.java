//  Copyright © 2017 Viro Media. All rights reserved.
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
import android.util.Log;

import com.facebook.react.bridge.JSApplicationCausedNativeException;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.internal.BaseSound;
import com.viro.core.ViroContext;
import com.viro.core.SoundData;
import com.viromedia.bridge.module.SoundModule;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.ViroLog;

public abstract class VRTBaseSound extends VRTComponent {

    private static final String TAG = ViroLog.getTag(VRTBaseSound.class);
    protected static final String NAME = "name";
    protected static final String URI = "uri";

    protected BaseSound mNativeSound;
    protected ReadableMap mSource;
    protected boolean mPaused = false;
    protected float mVolume = 1.0f;
    protected boolean mMuted = false;
    protected boolean mLoop = false;

    protected boolean mShouldResetSound = false;
    protected boolean mReady = false;

    public VRTBaseSound(ReactContext reactContext) {
        super(reactContext.getBaseContext(), null, -1, -1, reactContext);
    }

    public void setSource(ReadableMap source) {
        mSource = source;
        mShouldResetSound = true;
    }

    public void setPaused(boolean paused) {
        mPaused = paused;
        if (mReady && mNativeSound != null) {
            if (mPaused || !shouldAppear()) {
                mNativeSound.pause();
            } else {
                mNativeSound.play();
            }
        }
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

        // re-run the setPaused logic to start/stop playback.
        setPaused(mPaused);

        mNativeSound.setVolume(mVolume);
        mNativeSound.setMuted(mMuted);
        mNativeSound.setLoop(mLoop);
    }

    @Override
    protected void handleAppearanceChange() {
        // re-run the setPaused logic to start/stop playback.
        setPaused(mPaused);
        super.handleAppearanceChange();
    }

    @Override
    public void setViroContext(ViroContext context) {
        super.setViroContext(context);
        resetSound();
    }

    protected void resetSound() {
        if (mViroContext == null) {
            return;
        }
        if (mNativeSound != null) {
            mNativeSound.pause();
            mNativeSound.dispose();
            mNativeSound = null;
        }

        mShouldResetSound = false;

        // figure out what type of audio I have
        if (mSource.hasKey(NAME)) {
            SoundData data = getSoundDataForName(mSource.getString(NAME));
            if (data == null) {
                onError("Unknown Sound source with name: [" + mSource.getString(NAME) + "]");
                return;
            }
            mNativeSound = getNativeSound(data);
        } else if (mSource.hasKey(URI)) {
            Uri uri = Helper.parseUri(mSource.getString(URI), getContext());
            mNativeSound = getNativeSound(uri.toString());
        } else {
            throw new IllegalArgumentException("Unknown sound source.");
        }

        setNativeProps();
    }

    private SoundData getSoundDataForName(String name) {
        SoundModule soundModule = mReactContext.getNativeModule(SoundModule.class);
        return soundModule.getSoundData(name);
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mNativeSound != null) {
            mNativeSound.dispose();
            mNativeSound = null;
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

    /**
     * Child classes should implement this method to return their own type of native Sound object
     *
     * @param path - path/url to the sound file
     * @return the native sound object
     */
    protected abstract BaseSound getNativeSound(String path);

    protected abstract BaseSound getNativeSound(SoundData data);
}
