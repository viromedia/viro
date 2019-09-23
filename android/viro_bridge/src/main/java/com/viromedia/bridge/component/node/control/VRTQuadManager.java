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

package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.VRTNodeManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class VRTQuadManager extends VRTControlManager<VRTQuad> {

    public VRTQuadManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTQuad";
    }

    @Override
    protected VRTQuad createViewInstance(ThemedReactContext reactContext) {
        return new VRTQuad(reactContext);
    }

    @ReactProp(name = "width", defaultFloat = 1)
    public void setWidth(VRTQuad view, float width) {
        view.setWidth(width);
    }

    @ReactProp(name = "height", defaultFloat = 1)
    public void setHeight(VRTQuad view, float height) {
        view.setHeight(height);
    }

    @ReactProp(name = "uvCoordinates")
    public void setUVCoordinates(VRTQuad view, ReadableArray coordinates) {
        float u0 = 0; float v0 = 0; float u1 = 1; float v1 = 1;

        if (coordinates == null) {
            // do-nothing
        } else if (coordinates.size() != 4) {
            throw new IllegalArgumentException("[ViroSurface] Expected 4 uv coordinates, got " + coordinates.size());
        } else { // not null && has 4 elements
            u0 = (float) coordinates.getDouble(0);
            v0 = (float) coordinates.getDouble(1);
            u1 = (float) coordinates.getDouble(2);
            v1 = (float) coordinates.getDouble(3);
        }

        view.setUVCoordinates(u0, v0, u1, v1);
    }

    @ReactProp(name = "lightReceivingBitMask", defaultInt = 1)
    public void setLightReceivingBitMask(VRTQuad view, int bitMask) {view.setLightReceivingBitMask(bitMask); }

    @ReactProp(name = "shadowCastingBitMask", defaultInt = 1)
    public void setShadowCastingBitMask(VRTQuad view, int bitMask) {view.setShadowCastingBitMask(bitMask); }

    @ReactProp(name = "arShadowReceiver", defaultBoolean = false)
    public void setARShadowReceiver(VRTQuad view, boolean arShadowReceiver) { view.setARShadowReceiver(arShadowReceiver); }

}
