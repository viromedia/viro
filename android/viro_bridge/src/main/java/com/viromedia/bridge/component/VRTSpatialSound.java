/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.util.Log;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.internal.BaseSound;
import com.viro.core.Node;
import com.viro.core.Sound;
import com.viro.core.SoundData;
import com.viro.core.SpatialSound;
import com.viro.core.Vector;
import com.viromedia.bridge.utility.ViroEvents;

public class VRTSpatialSound extends VRTBaseSound implements SpatialSound.Delegate {

    private static String DEFAULT_ROLLOFF_MODEL = "None";
    private static float[] DEFAULT_POSITION = {0f,0f,0f};

    private final Node mParentNode;

    protected float[] mPosition = DEFAULT_POSITION;
    protected String mRolloffModel = DEFAULT_ROLLOFF_MODEL;
    protected float mMinDistance = 0f;
    protected float mMaxDistance = 10f;

    public VRTSpatialSound(ReactApplicationContext reactContext, Node parentNode) {
        super(reactContext);
        mParentNode = parentNode;
    }

    public void setPosition(float[] position) {
        mPosition = position == null ? DEFAULT_POSITION : position;
    }

    public void setRolloffModel(String rolloffModel) {
        mRolloffModel = rolloffModel == null ? DEFAULT_ROLLOFF_MODEL : rolloffModel ;
    }

    public void setMinDistance(float minDistance) {
        mMinDistance = minDistance;
    }

    public void setMaxDistance(float maxDistance) {
        mMaxDistance = maxDistance;
    }

    @Override
    protected void setNativeProps() {
        super.setNativeProps();
        if (mNativeSound == null) {
            return;
        }

        ((SpatialSound) mNativeSound).setPosition(new Vector(mPosition));
        ((SpatialSound) mNativeSound).setDistanceRolloff(SpatialSound.Rolloff.valueFromString(mRolloffModel), mMinDistance, mMaxDistance);
    }

    @Override
    protected void resetSound() {
        // only set the node if we have to reset the sound (recreate it)
        boolean shouldSetNode = mShouldResetSound;
        super.resetSound();
        if (mNativeSound != null && shouldSetNode) {
            mParentNode.addSound((SpatialSound) mNativeSound);
        }
    }

    @Override
    protected BaseSound getNativeSound(String path, boolean local) {
        SpatialSound sound = new SpatialSound(path, mViroContext, this, local);
        sound.setDelegate(this);
        return sound;
    }

    @Override
    protected BaseSound getNativeSound(SoundData data) {
        SpatialSound sound = new SpatialSound(data, mViroContext, this);
        sound.setDelegate(this);
        return sound;
    }

    @Override
    public void onSoundReady(SpatialSound sound) {
        mReady = true;
        if (mNativeSound != null && !mPaused) {
            mNativeSound.play();
        }
    }

    public void onSoundFail(String error) {
        onError(error);
    }
}
