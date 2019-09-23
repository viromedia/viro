//  Copyright © 2016 Viro Media. All rights reserved.
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
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.VRTNode;
import com.viromedia.bridge.component.node.VRTNodeManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

/**
 * BoxManager for building a {@link VRTBox}
 * corresponding to the ViroBox.js control.
 */
public class VRTBoxManager extends VRTControlManager<VRTBox> {

    public VRTBoxManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTBox";
    }

    @Override
    protected VRTBox createViewInstance(ThemedReactContext reactContext) {
        return new VRTBox(reactContext);
    }

    @ReactProp(name = "width", defaultFloat = 1.0f)
    public void setWidth(VRTBox box, float width) {
        box.setWidth(width);
    }

    @ReactProp(name = "height", defaultFloat = 1.0f)
    public void setHeight(VRTBox box, float height) {
        box.setHeight(height);
    }

    @ReactProp(name = "length", defaultFloat = 1.0f)
    public void setLength(VRTBox box, float length) {
        box.setLength(length);
    }

    @ReactProp(name = "lightReceivingBitMask", defaultInt = 1)
    public void setLightReceivingBitMask(VRTBox box, int bitMask) {box.setLightReceivingBitMask(bitMask); }

    @ReactProp(name = "shadowCastingBitMask", defaultInt = 1)
    public void setShadowCastingBitMask(VRTBox box, int bitMask) {box.setShadowCastingBitMask(bitMask); }

}
