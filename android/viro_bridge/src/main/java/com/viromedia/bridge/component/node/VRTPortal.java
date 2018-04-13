package com.viromedia.bridge.component.node;

import com.viro.core.Portal;
import com.viro.core.Node;
import com.viromedia.bridge.component.node.control.VRT3DObject;
import android.view.View;

import com.facebook.react.bridge.ReactContext;
/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */

public class VRTPortal extends VRTNode {

    public VRTPortal(ReactContext context) {
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
