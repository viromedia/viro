/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.internal.BaseSound;
import com.viro.core.SoundData;
import com.viro.core.SoundField;
import com.viro.core.Vector;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;

public class VRTSoundField extends VRTBaseSound implements SoundField.PlaybackListener {

    private static float[] DEFAULT_ROTATION = {0f,0f,0f};

    protected float[] mRotation = DEFAULT_ROTATION;

    public VRTSoundField(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    public void setRotation(float[] rotation) {
        mRotation = rotation == null ? DEFAULT_ROTATION : rotation;
    }

    @Override
    protected void setNativeProps() {
        super.setNativeProps();
        if (mNativeSound == null) {
            return;
        }

        ((SoundField) mNativeSound).setRotation(Helper.toRadiansVector(mRotation));
    }

    @Override
    protected BaseSound getNativeSound(String path, boolean local) {
        SoundField sound = new SoundField(path, mViroContext, this, local);
        sound.setPlaybackListener(this);
        return sound;
    }

    @Override
    protected BaseSound getNativeSound(SoundData data) {
        SoundField sound = new SoundField(data, mViroContext, this);
        sound.setPlaybackListener(this);
        return sound;
    }

    @Override
    public void onSoundReady(SoundField sound) {
        mReady = true;
        if (mNativeSound != null && !mPaused) {
            mNativeSound.play();
        }
    }

    @Override
    public void onSoundFail(String error) {
        onError(error);
    }

}
