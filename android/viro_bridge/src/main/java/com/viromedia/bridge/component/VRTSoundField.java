/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.BaseSoundJni;
import com.viro.renderer.jni.SoundDataJni;
import com.viro.renderer.jni.SoundFieldJni;

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
    protected BaseSoundJni getNativeSound(String path, boolean local) {
        return new SoundFieldJni(path, mRenderContext, this, local);
    }

    @Override
    protected BaseSoundJni getNativeSound(SoundDataJni data) {
        return new SoundFieldJni(data, mRenderContext, this);
    }
}
