/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import android.content.Context;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.BaseSoundJni;
import com.viro.renderer.jni.SoundDataJni;
import com.viro.renderer.jni.SoundJni;

public class Sound extends BaseSound {

    public Sound(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    public Sound(Context context) {
        super(context);
    }

    @Override
    protected BaseSoundJni getNativeSound(String path, boolean local) {
        return new SoundJni(path, mRenderContext, this, local);
    }

    @Override
    protected BaseSoundJni getNativeSound(SoundDataJni data) {
        return new SoundJni(data, mRenderContext, this);
    }
}
