//  Copyright © 2017 Viro Media. All rights reserved.
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

package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactContext;
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

    public VRTARPlane(ReactContext context) {
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
        } else if (alignment.equalsIgnoreCase("Vertical")) {
            ((ARDeclarativePlane) getNodeJni()).setAlignment(ARPlaneAnchor.Alignment.VERTICAL);
        }

        mNeedsUpdate = true;
    }

    public void setAnchorId(String anchorId) {
        ((ARDeclarativeNode) getNodeJni()).setAnchorId(anchorId);
        mNeedsUpdate = true;
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
