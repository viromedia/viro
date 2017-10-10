/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;

import com.viro.renderer.jni.Node;
import com.viro.renderer.jni.Spotlight;

public class VRTSpotLight extends VRTLight {
    private static final float[] DEFAULT_POSITION = {0, 0, 0};

    private float[] mDirection;
    private float[] mPosition = DEFAULT_POSITION;
    private float mAttenuationStartDistance;
    private float mAttenuationEndDistance;
    private float mInnerAngle;
    private float mOuterAngle;

    private Spotlight mNativeLight;

    public VRTSpotLight(Context context) {
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

    public float[] getDirection() {
        return mDirection;
    }

    public void setDirection(float[] direction) {
        if (direction == null) {
            throw new IllegalArgumentException("Direction cannot be null for SpotLight");
        }
        this.mDirection = direction;
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

    public float getInnerAngle() {
        return mInnerAngle;
    }

    public void setInnerAngle(float innerAngle) {
        this.mInnerAngle = innerAngle;
    }

    public float getOuterAngle() {
        return mOuterAngle;
    }

    public void setOuterAngle(float outerAngle) {
        this.mOuterAngle = outerAngle;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();

        if (mNativeLight == null) {
            mNativeLight = new Spotlight(mColor, mIntensity, mAttenuationStartDistance, mAttenuationEndDistance,
                    mPosition, mDirection, mInnerAngle, mOuterAngle);
        } else {
            mNativeLight.setColor(mColor);
            mNativeLight.setIntensity(mIntensity);
            mNativeLight.setAttenuationStartDistance(mAttenuationStartDistance);
            mNativeLight.setAttenuationEndDistance(mAttenuationEndDistance);
            mNativeLight.setPosition(mPosition);
            mNativeLight.setDirection(mDirection);
            mNativeLight.setInnerAngle(mInnerAngle);
            mNativeLight.setOuterAngle(mOuterAngle);
        }
        mNativeLight.setCastsShadow(mCastsShadow);
        mNativeLight.setShadowOpacity(mShadowOpacity);
        mNativeLight.setShadowMapSize(mShadowMapSize);
        mNativeLight.setShadowBias(mShadowBias);
        mNativeLight.setShadowNearZ(mShadowNearZ);
        mNativeLight.setShadowFarZ(mShadowFarZ);
        mNativeLight.setInfluenceBitMask(mInfluenceBitMask);
    }
}
