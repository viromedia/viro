/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;

import com.viro.renderer.jni.NodeJni;
import com.viro.renderer.jni.OmniLightJni;

public class OmniLight extends Light {

    private static final float[] DEFAULT_POSITION = {0, 0, 0};

    private float[] mPosition = DEFAULT_POSITION;
    private float mAttenuationStartDistance;
    private float mAttenuationEndDistance;

    private OmniLightJni mNativeLight;
    public OmniLight(Context context) {
        super(context);
    }

    @Override
    public void addToNode(NodeJni nodeJni) {

        mNativeLight.addToNode(nodeJni);
    }

    @Override
    public void removeFromNode(NodeJni nodeJni) {

        mNativeLight.removeFromNode(nodeJni);
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
            mNativeLight = new OmniLightJni(mColor, mIntensity, mAttenuationStartDistance,
                    mAttenuationEndDistance, mPosition);
        } else {
            mNativeLight.setColor(mColor);
            mNativeLight.setIntensity(mIntensity);
            mNativeLight.setAttenuationStartDistance(mAttenuationStartDistance);
            mNativeLight.setAttenuationEndDistance(mAttenuationEndDistance);
            mNativeLight.setPosition(mPosition);
        }
    }

}
