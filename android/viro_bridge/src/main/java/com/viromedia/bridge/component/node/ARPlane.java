/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.content.Context;
import android.util.AttributeSet;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.ARPlaneJni;
import com.viro.renderer.jni.NodeJni;

public class ARPlane extends Node {

    private static final float DEFAULT_WIDTH = 0f;
    private static final float DEFAULT_HEIGHT = 0f;

    private boolean mDimensionsUpdated = false;

    public ARPlane(ReactApplicationContext context) {
        super(context);
    }

    protected NodeJni createNodeJni() {
        ARPlaneJni arPlaneJni = new ARPlaneJni(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        return arPlaneJni;
    }

    public void setMinWidth(float minWidth) {
        ((ARPlaneJni) getNodeJni()).setMinWidth(minWidth);
        mDimensionsUpdated = true;
    }

    public void setMinHeight(float minHeight) {
        ((ARPlaneJni) getNodeJni()).setMinHeight(minHeight);
        mDimensionsUpdated = true;
    }

    @Override
    protected void onPropsSet() {
        if (mDimensionsUpdated && mScene != null) {
            ((ARScene) mScene).updateARPlane((ARPlaneJni) getNodeJni());
            mDimensionsUpdated = false;
        }
    }
}
