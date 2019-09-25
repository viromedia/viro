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
import com.viro.core.OmniLight;
import com.viro.core.Vector;

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

        mNativeLight.setTemperature(mTemperature);
        mNativeLight.setInfluenceBitMask(mInfluenceBitMask);
    }

}
