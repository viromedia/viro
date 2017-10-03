/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;

import com.viro.renderer.jni.AmbientLightJni;
import com.viro.renderer.jni.NodeJni;

public class AmbientLight extends Light {

    private AmbientLightJni mNativeLight;

    public AmbientLight(Context context) {
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

    @Override
    public void onPropsSet() {
        super.onPropsSet();

        if (mNativeLight == null) {

            mNativeLight = new AmbientLightJni(mColor, mIntensity);
        } else {
            mNativeLight.setColor(mColor);
            mNativeLight.setIntensity(mIntensity);
        }

        mNativeLight.setInfluenceBitMask(mInfluenceBitMask);
    }
}
