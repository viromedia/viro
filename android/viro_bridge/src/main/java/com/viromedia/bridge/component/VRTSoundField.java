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

import com.facebook.react.bridge.ReactContext;
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

    public VRTSoundField(ReactContext reactContext) {
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
    protected BaseSound getNativeSound(String path) {
        SoundField sound = new SoundField(mViroContext, Uri.parse(path), this);
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
