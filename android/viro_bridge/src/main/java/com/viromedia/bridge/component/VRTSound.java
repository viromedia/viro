/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import android.net.Uri;

import com.facebook.react.bridge.ReactContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.internal.BaseSound;
import com.viro.core.SoundData;
import com.viro.core.Sound;
import com.viromedia.bridge.utility.ViroEvents;

public class VRTSound extends VRTBaseSound implements Sound.PlaybackListener {

    public VRTSound(ReactContext reactContext) {
        super(reactContext);
    }

    @Override
    protected BaseSound getNativeSound(String path) {
        Sound sound = new Sound(mViroContext, Uri.parse(path), this);
        sound.setPlaybackListener(this);
        return sound;
    }

    @Override
    protected BaseSound getNativeSound(SoundData data) {
        Sound sound = new Sound(data, mViroContext, this);
        sound.setPlaybackListener(this);
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
