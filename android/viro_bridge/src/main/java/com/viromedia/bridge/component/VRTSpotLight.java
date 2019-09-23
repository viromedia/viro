//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

package com.viromedia.bridge.component;

import android.content.Context;

import com.viro.core.Node;
import com.viro.core.Spotlight;
import com.viro.core.Vector;

public class VRTSpotLight extends VRTLight {
    private static final float[] DEFAULT_POSITION = {0, 0, 0};

    private float[] mDirection;
    private float[] mPosition = DEFAULT_POSITION;
    private float mAttenuationStartDistance = 2.0f;
    private float mAttenuationEndDistance = 10f;
    private float mInnerAngle = 0f;
    private float mOuterAngle = 45f;

    private Spotlight mNativeLight;

    public VRTSpotLight(Context context) {
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
            mNativeLight = new Spotlight(mColor, mIntensity, mAttenuationStartDistance,
                    mAttenuationEndDistance, new Vector(mPosition), new Vector(mDirection),
                    (float) Math.toRadians(mInnerAngle), (float) Math.toRadians(mOuterAngle));
        } else {
            mNativeLight.setColor(mColor);
            mNativeLight.setIntensity(mIntensity);
            mNativeLight.setAttenuationStartDistance(mAttenuationStartDistance);
            mNativeLight.setAttenuationEndDistance(mAttenuationEndDistance);
            mNativeLight.setPosition(new Vector(mPosition));
            mNativeLight.setDirection(new Vector(mDirection));
            mNativeLight.setInnerAngle((float) Math.toRadians(mInnerAngle));
            mNativeLight.setOuterAngle((float) Math.toRadians(mOuterAngle));
        }

        mNativeLight.setTemperature(mTemperature);
        mNativeLight.setCastsShadow(mCastsShadow);
        mNativeLight.setShadowOpacity(mShadowOpacity);
        mNativeLight.setShadowMapSize(mShadowMapSize);
        mNativeLight.setShadowBias(mShadowBias);
        mNativeLight.setShadowNearZ(mShadowNearZ);
        mNativeLight.setShadowFarZ(mShadowFarZ);
        mNativeLight.setInfluenceBitMask(mInfluenceBitMask);
    }
}
