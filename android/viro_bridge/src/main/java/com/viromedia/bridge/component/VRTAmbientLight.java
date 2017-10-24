/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;

import com.viro.renderer.jni.AmbientLight;
import com.viro.renderer.jni.Node;

public class VRTAmbientLight extends VRTLight {

    private AmbientLight mNativeLight;

    public VRTAmbientLight(Context context) {
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

    @Override
    public void onPropsSet() {
        super.onPropsSet();

        if (mNativeLight == null) {

            mNativeLight = new AmbientLight(mColor, mIntensity);
        } else {
            mNativeLight.setColor(mColor);
            mNativeLight.setIntensity(mIntensity);
        }

        mNativeLight.setInfluenceBitMask(mInfluenceBitMask);
    }
}
