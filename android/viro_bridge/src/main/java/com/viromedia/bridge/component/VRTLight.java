/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.component;

import android.content.Context;

import com.viro.renderer.jni.NodeJni;

public abstract class VRTLight extends VRTComponent {

    protected int mColor;
    protected float mIntensity = 1000.0f; // default intensity is 1000 lumens (same as renderer)

    protected boolean mCastsShadow = false;
    protected float mShadowOpacity = 1.0f;
    protected float[] mShadowOrthographicPosition;
    protected float mShadowOrthographicSize = 20;
    protected int mShadowMapSize = 1024;
    protected float mShadowBias = 0.005f;
    protected float mShadowNearZ = 0.1f;
    protected float mShadowFarZ = 20;
    protected int mInfluenceBitMask = 1;

    public VRTLight(Context context) {
        super(context);
    }

    public int getColor() {
        return mColor;
    }

    public void setColor(int mColor) {
        this.mColor = mColor;
    }

    public void setIntensity(float intensity) {
        this.mIntensity = intensity;
    }

    public void setCastsShadow(boolean castsShadow) {
        this.mCastsShadow = castsShadow;
    }

    public void setShadowOpacity(float opacity) {
        this.mShadowOpacity = opacity;
    }

    public void setShadowOrthographicPosition(float position[]) {
        mShadowOrthographicPosition = position;
    }

    public void setShadowOrthographicSize(float size) {
        this.mShadowOrthographicSize = size;
    }

    public void setShadowMapSize(int mapSize) {
        this.mShadowMapSize = mapSize;
    }

    public void setShadowBias(float bias) {
        this.mShadowBias = bias;
    }

    public void setShadowNearZ(float nearZ) {
        this.mShadowNearZ = nearZ;
    }

    public void setShadowFarZ(float farZ) {
        this.mShadowFarZ = farZ;
    }

    public void setInfluenceBitMask(int influenceBitMask) {
        this.mInfluenceBitMask = influenceBitMask;
    }

    public abstract void addToNode(NodeJni nodeJni);

    public abstract void removeFromNode(NodeJni nodeJni);
}
