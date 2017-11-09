/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;

import com.viro.core.DirectionalLight;
import com.viro.core.Node;
import com.viro.core.Vector;

public class VRTDirectionalLight extends VRTLight {

    private float[] mDirection;

    private DirectionalLight mNativeLight;

    public VRTDirectionalLight(Context context) {
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

    public float[] getDirection() {
        return mDirection;
    }

    public void setDirection(float[] mDirection) {
        if (mDirection == null) {
            throw new IllegalArgumentException("Direction cannot be null for DirectionalLight");
        }
        this.mDirection = mDirection;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();

        if (mNativeLight == null) {
            mNativeLight = new DirectionalLight(mColor, mIntensity, new Vector(mDirection));
        } else {
            mNativeLight.setColor(mColor);
            mNativeLight.setIntensity(mIntensity);
            mNativeLight.setDirection(new Vector(mDirection));
        }

        mNativeLight.setCastsShadow(mCastsShadow);
        mNativeLight.setShadowOpacity(mShadowOpacity);
        if (mShadowOrthographicPosition != null) {
            mNativeLight.setShadowOrthographicPosition(new Vector(mShadowOrthographicPosition));
        }
        mNativeLight.setShadowOrthographicSize(mShadowOrthographicSize);
        mNativeLight.setShadowMapSize(mShadowMapSize);
        mNativeLight.setShadowBias(mShadowBias);
        mNativeLight.setShadowNearZ(mShadowNearZ);
        mNativeLight.setShadowFarZ(mShadowFarZ);
        mNativeLight.setInfluenceBitMask(mInfluenceBitMask);
    }
}