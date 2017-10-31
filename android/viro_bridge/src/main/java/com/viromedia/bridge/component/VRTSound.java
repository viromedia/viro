/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.BaseSound;
import com.viro.renderer.jni.SoundData;
import com.viro.renderer.jni.Sound;
import com.viromedia.bridge.utility.ViroEvents;

public class VRTSound extends VRTBaseSound implements Sound.Delegate {

    public VRTSound(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    protected BaseSound getNativeSound(String path, boolean local) {
        Sound sound = new Sound(path, mViroContext, this);
        sound.setDelegate(this);
        return sound;
    }

    @Override
    protected BaseSound getNativeSound(SoundData data) {
        Sound sound = new Sound(data, mViroContext, this);
        sound.setDelegate(this);
        return sound;
    }

    @Override
    public void onSoundReady(Sound sound) {
        mReady = true;
        if (mNativeSound != null && !mPaused) {
            mNativeSound.play();
        }
    }

    @Override
    public void onSoundFinish(Sound sound) {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(getId(),
                ViroEvents.ON_FINISH,
                null);
    }

    public void onSoundFail(String error) {
        onError(error);
    }

}
