/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;

import com.viro.renderer.jni.Node;
import com.viro.renderer.jni.OmniLight;

public class VRTOmniLight extends VRTLight {

    private static final float[] DEFAULT_POSITION = {0, 0, 0};

    private float[] mPosition = DEFAULT_POSITION;
    private float mAttenuationStartDistance;
    private float mAttenuationEndDistance;

    private OmniLight mNativeLight;
    public VRTOmniLight(Context context) {
        super(context);
    }

    @Override
    public void addToNode(Node node) {

        mNativeLight.addToNode(node);
    }

    @Override
    public void removeFromNode(Node node) {

        mNativeLight.removeFromNode(node);
    }

    @Override
    public void onTearDown(){
        if (mNativeLight != null){
            mNativeLight.destroy();
            mNativeLight = null;
        }
        super.onTearDown();
    }

    public float[] getPosition() {
        return mPosition;
    }

    public void setPosition(float[] position) {

        this.mPosition = position;
    }

    public float getAttenuationStartDistance() {
        return mAttenuationStartDistance;
    }

    public void setAttenuationStartDistance(float attenuationStartDistance) {
        this.mAttenuationStartDistance = attenuationStartDistance;
    }

    public float getAttenuationEndDistance() {
        return mAttenuationEndDistance;
    }

    public void setAttenuationEndDistance(float attenuationEndDistance) {
        this.mAttenuationEndDistance = attenuationEndDistance;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();

        if (mNativeLight == null) {
            mNativeLight = new OmniLight(mColor, mIntensity, mAttenuationStartDistance,
                    mAttenuationEndDistance, mPosition);
        } else {
            mNativeLight.setColor(mColor);
            mNativeLight.setIntensity(mIntensity);
            mNativeLight.setAttenuationStartDistance(mAttenuationStartDistance);
            mNativeLight.setAttenuationEndDistance(mAttenuationEndDistance);
            mNativeLight.setPosition(mPosition);
        }
        mNativeLight.setInfluenceBitMask(mInfluenceBitMask);
    }

}
