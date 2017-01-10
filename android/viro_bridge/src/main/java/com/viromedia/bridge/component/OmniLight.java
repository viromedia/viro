/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;
import android.util.Log;

import com.viro.renderer.jni.NodeJni;
import com.viro.renderer.jni.OmniLightJni;
import com.viro.renderer.jni.SpotLightJni;
import com.viromedia.bridge.component.node.Scene;

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
    protected void onTearDown(){
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
            mNativeLight = new OmniLightJni(mColor, mAttenuationStartDistance, mAttenuationEndDistance,
                    mPosition);
        } else {
            mNativeLight.setColor(mColor);
            mNativeLight.setAttenuationStartDistance(mAttenuationStartDistance);
            mNativeLight.setAttenuationEndDistance(mAttenuationEndDistance);
            mNativeLight.setPosition(mPosition);
        }
    }

}
