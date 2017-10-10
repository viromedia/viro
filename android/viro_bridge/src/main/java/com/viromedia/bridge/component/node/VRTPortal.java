package com.viromedia.bridge.component.node;

import com.viro.renderer.jni.Portal;
import com.viro.renderer.jni.Node;
import com.viromedia.bridge.component.node.control.VRT3DObject;
import android.view.View;

import com.facebook.react.bridge.ReactApplicationContext;
/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */

public class VRTPortal extends VRTNode {

    public VRTPortal(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public void addView(View child, int index) {
        if(!child.getClass().equals(VRT3DObject.class)) {
            throw new IllegalStateException("VRTPortal can only have a Viro3DObject as a child.");
        }
        super.addView(child, index);
    }

    protected Node createNodeJni() {
        Portal portal = new Portal();
        return portal;
    }
}
