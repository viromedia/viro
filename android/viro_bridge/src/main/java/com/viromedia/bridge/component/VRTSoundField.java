/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.BaseSound;
import com.viro.renderer.jni.SoundData;
import com.viro.renderer.jni.SoundField;

public class VRTSoundField extends VRTBaseSound {

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

        mNativeSound.setRotation(mRotation);
    }

    @Override
    protected BaseSound getNativeSound(String path, boolean local) {
        return new SoundField(path, mViroContext, this, local);
    }

    @Override
    protected BaseSound getNativeSound(SoundData data) {
        return new SoundField(data, mViroContext, this);
    }
}
