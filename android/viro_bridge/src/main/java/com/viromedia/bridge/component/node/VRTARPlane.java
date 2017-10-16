/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.WritableMap;
import com.viro.renderer.ARAnchor;
import com.viro.renderer.jni.ARPlane;
import com.viro.renderer.jni.Node;

public class VRTARPlane extends VRTARNode {

    private static final float DEFAULT_WIDTH = 0f;
    private static final float DEFAULT_HEIGHT = 0f;

    private boolean mNeedsUpdate = false;

    public VRTARPlane(ReactApplicationContext context) {
        super(context);
    }

    protected Node createNodeJni() {
        ARPlane arPlaneJni = new ARPlane(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        arPlaneJni.registerARNodeDelegate(this);
        return arPlaneJni;
    }

    public void setMinWidth(float minWidth) {
        ((ARPlane) getNodeJni()).setMinWidth(minWidth);
        mNeedsUpdate = true;
    }

    public void setMinHeight(float minHeight) {
        ((ARPlane) getNodeJni()).setMinHeight(minHeight);
        mNeedsUpdate = true;
    }

    public void setAnchorId(String anchorId) {
        ((ARPlane) getNodeJni()).setAnchorId(anchorId);
        mNeedsUpdate = true;
    }

    @Override
    public void setScene(VRTScene scene) {
        super.setScene(scene);
        if (scene != null) {
            ((VRTARScene) scene).addARPlane((ARPlane) getNodeJni());
        }
    }

    @Override
    public void parentDidDisappear() {
        if (mScene != null) {
            ((VRTARScene) mScene).removeARPlane((ARPlane) getNodeJni());
        }
    }

    @Override
    protected void onPropsSet() {
        if (mNeedsUpdate && mScene != null) {
            ((VRTARScene) mScene).updateARPlane((ARPlane) getNodeJni());
            mNeedsUpdate = false;
        }
    }
}
