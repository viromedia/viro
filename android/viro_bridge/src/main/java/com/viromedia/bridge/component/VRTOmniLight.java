/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;

import com.viro.renderer.jni.Node;
import com.viro.renderer.jni.OmniLight;
import com.viro.renderer.jni.Vector;

public class VRTOmniLight extends VRTLight {

    private static final float[] DEFAULT_POSITION = {0, 0, 0};

    private float[] mPosition = DEFAULT_POSITION;
    private float mAttenuationStartDistance = 2f;
    private float mAttenuationEndDistance = 10f;

    private OmniLight mNativeLight;
    public VRTOmniLight(Context context) {
        super(context);
    }

    @Override
    public void addToNode(Node nodeJni) {
        nodeJni.addLight(mNativeLight);
    }

    @Override
    public void removeFromNode(Node nodeJni) {
        nodeJni.removeLight(mNativeLight);
    }

    @Override
    public void onTearDown(){
        if (mNativeLight != null){
            mNativeLight.dispose();
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
                    mAttenuationEndDistance, new Vector(mPosition));
        } else {
            mNativeLight.setColor(mColor);
            mNativeLight.setIntensity(mIntensity);
            mNativeLight.setAttenuationStartDistance(mAttenuationStartDistance);
            mNativeLight.setAttenuationEndDistance(mAttenuationEndDistance);
            mNativeLight.setPosition(new Vector(mPosition));
        }
        mNativeLight.setInfluenceBitMask(mInfluenceBitMask);
    }

}
