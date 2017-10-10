/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.BaseSound;
import com.viro.renderer.jni.SoundData;
import com.viro.renderer.jni.Sound;

public class VRTSound extends VRTBaseSound {

    public VRTSound(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    protected BaseSound getNativeSound(String path, boolean local) {
        return new Sound(path, mRenderContext, this, local);
    }

    @Override
    protected BaseSound getNativeSound(SoundData data) {
        return new Sound(data, mRenderContext, this);
    }
}
