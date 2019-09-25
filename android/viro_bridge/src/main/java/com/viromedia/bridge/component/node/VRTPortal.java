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

import com.viro.core.Portal;
import com.viro.core.Node;
import com.viromedia.bridge.component.node.control.VRT3DObject;
import android.view.View;

import com.facebook.react.bridge.ReactContext;

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
