/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.WritableMap;
import com.viro.core.ARAnchor;
import com.viro.core.ARPlaneAnchor;
import com.viro.core.internal.ARDeclarativeNode;
import com.viro.core.internal.ARDeclarativePlane;
import com.viro.core.Node;

public class VRTARPlane extends VRTARNode {

    private static final float DEFAULT_WIDTH = 0f;
    private static final float DEFAULT_HEIGHT = 0f;
    private static final ARPlaneAnchor.Alignment DEFAULT_ALIGNMENT = ARPlaneAnchor.Alignment.HORIZONTAL;

    private boolean mNeedsUpdate = false;

    public VRTARPlane(ReactApplicationContext context) {
        super(context);
    }

    protected Node createNodeJni() {
        ARDeclarativePlane arPlaneJni = new ARDeclarativePlane(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_ALIGNMENT);
        arPlaneJni.setDelegate(this);
        return arPlaneJni;
    }

    public void setMinWidth(float minWidth) {
        ((ARDeclarativePlane) getNodeJni()).setMinWidth(minWidth);
        mNeedsUpdate = true;
    }

    public void setMinHeight(float minHeight) {
        ((ARDeclarativePlane) getNodeJni()).setMinHeight(minHeight);
        mNeedsUpdate = true;
    }

    public void setAlignment(String alignment) {
        if (alignment.equalsIgnoreCase("Horizontal")) {
            ((ARDeclarativePlane) getNodeJni()).setAlignment(ARPlaneAnchor.Alignment.HORIZONTAL);
        } else if (alignment.equalsIgnoreCase("HorizontalUpward")) {
            ((ARDeclarativePlane) getNodeJni()).setAlignment(ARPlaneAnchor.Alignment.HORIZONTAL_UPWARD);
        } else if (alignment.equalsIgnoreCase("HorizontalDownward")) {
            ((ARDeclarativePlane) getNodeJni()).setAlignment(ARPlaneAnchor.Alignment.HORIZONTAL_DOWNWARD);
        }

        mNeedsUpdate = true;
    }

    public void setAnchorId(String anchorId) {
        ((ARDeclarativeNode) getNodeJni()).setAnchorId(anchorId);
        mNeedsUpdate = true;
    }

    public void setPauseUpdates(boolean pauseUpdates) {
        ((ARDeclarativeNode) getNodeJni()).setPauseUpdates(pauseUpdates);
    }

    @Override
    public void setScene(VRTScene scene) {
        super.setScene(scene);
        if (scene != null) {
            ((VRTARScene) scene).addARNode((ARDeclarativeNode) getNodeJni());
        }
    }

    @Override
    public void parentDidDisappear() {
        if (mScene != null && getNodeJni() != null) {
            ((VRTARScene) mScene).removeARNode((ARDeclarativeNode) getNodeJni());
        }
    }

    @Override
    protected void onPropsSet() {
        if (mNeedsUpdate && mScene != null) {
            ((VRTARScene) mScene).updateARNode((ARDeclarativeNode) getNodeJni());
            mNeedsUpdate = false;
        }
    }
}
