/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.util.Log;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.WritableMap;
import com.viro.renderer.ARAnchor;
import com.viro.renderer.jni.ARPlaneJni;
import com.viro.renderer.jni.NodeJni;

public class ARPlane extends ARNode {

    private static final float DEFAULT_WIDTH = 0f;
    private static final float DEFAULT_HEIGHT = 0f;

    private boolean mDimensionsUpdated = false;

    public ARPlane(ReactApplicationContext context) {
        super(context);
    }

    @Override
    WritableMap mapFromARAnchor(ARAnchor arAnchor) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putArray("position", Arguments.makeNativeArray(arAnchor.getPosition()));
        returnMap.putArray("rotation", Arguments.makeNativeArray(arAnchor.getRotation()));
        returnMap.putArray("center", Arguments.makeNativeArray(arAnchor.getCenter()));
        returnMap.putDouble("width", arAnchor.getExtent()[0]);
        returnMap.putDouble("height", arAnchor.getExtent()[2]);
        return returnMap;
    }

    protected NodeJni createNodeJni() {
        ARPlaneJni arPlaneJni = new ARPlaneJni(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        arPlaneJni.registerARNodeDelegate(this);
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
    public void setScene(Scene scene) {
        super.setScene(scene);
        if (scene != null) {
            ((ARScene) scene).addARPlane((ARPlaneJni) getNodeJni());
        }
    }

    @Override
    public void parentDidDisappear() {
        if (mScene != null) {
            ((ARScene) mScene).removeARPlane((ARPlaneJni) getNodeJni());
        }
    }

    @Override
    protected void onPropsSet() {
        if (mDimensionsUpdated && mScene != null) {
            ((ARScene) mScene).updateARPlane((ARPlaneJni) getNodeJni());
            mDimensionsUpdated = false;
        }
    }
}
