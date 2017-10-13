/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.BaseSound;
import com.viro.renderer.jni.Node;
import com.viro.renderer.jni.SoundData;
import com.viro.renderer.jni.SpatialSound;

public class VRTSpatialSound extends VRTBaseSound {

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

        mNativeSound.setPosition(mPosition);
        mNativeSound.setDistanceRolloff(mRolloffModel, mMinDistance, mMaxDistance);
    }

    @Override
    protected void resetSound() {
        // only set the node if we have to reset the sound (recreate it)
        boolean shouldSetNode = mShouldResetSound;
        super.resetSound();
        if (mNativeSound != null && shouldSetNode) {
            ((SpatialSound) mNativeSound).attachToNode(mParentNode);
        }
    }

    @Override
    protected BaseSound getNativeSound(String path, boolean local) {
        return new SpatialSound(path, mViroContext, this, local);
    }

    @Override
    protected BaseSound getNativeSound(SoundData data) {
        return new SpatialSound(data, mViroContext, this);
    }
}
