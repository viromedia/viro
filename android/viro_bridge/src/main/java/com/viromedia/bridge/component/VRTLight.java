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

public abstract class VRTLight extends VRTComponent {

    protected int mColor;
    protected float mIntensity = 1000.0f; // default intensity is 1000 lumens (same as renderer)
    protected float mTemperature = 6500f; // default temperature is 6500

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

    public void setTemperature(float temperature) {
        this.mTemperature = temperature;
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

    public abstract void addToNode(Node nodeJni);

    public abstract void removeFromNode(Node nodeJni);
}
