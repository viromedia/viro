/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import android.content.Context;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.BaseSoundJni;
import com.viro.renderer.jni.NodeJni;
import com.viro.renderer.jni.SpatialSoundJni;

public class SpatialSound extends BaseSound {

    private static String DEFAULT_ROLLOFF_MODEL = "None";
    private static float[] DEFAULT_POSITION = {0f,0f,0f};

    private final NodeJni mParentNode;

    protected float[] mPosition = DEFAULT_POSITION;
    protected String mRolloffModel = DEFAULT_ROLLOFF_MODEL;
    protected float mMinDistance = 0f;
    protected float mMaxDistance = 10f;

    public SpatialSound(ReactApplicationContext reactContext, NodeJni parentNode) {
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
            ((SpatialSoundJni) mNativeSound).attachToNode(mParentNode);
        }
    }

    @Override
    protected BaseSoundJni getNativeSound(String path, boolean local) {
        return new SpatialSoundJni(path, mRenderContext, this, local);
    }

}
