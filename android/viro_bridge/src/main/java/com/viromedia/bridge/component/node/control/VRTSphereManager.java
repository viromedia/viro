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
 * SphereManager for building a {@link VRTSphere}
 * corresponding to the ViroSphere.js control.
 */
public class VRTSphereManager extends VRTControlManager<VRTSphere> {

    public VRTSphereManager(ReactApplicationContext context){
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSphere";
    }

    @Override
    protected VRTSphere createViewInstance(ThemedReactContext reactContext) {
        return new VRTSphere(reactContext);
    }

    @ReactProp(name = "facesOutward", defaultBoolean = VRTSphere.DEFAULT_FACES_OUTWARD)
    public void setFacesOutward(VRTSphere sphere, boolean facesOutward) {
        sphere.setFacesOutward(facesOutward);
    }

    @ReactProp(name = "widthSegmentCount", defaultInt = VRTSphere.DEFAULT_WIDTH_SEGMENT)
    public void setWidthSegmentCount(VRTSphere sphere, int widthSegmentCount) {
        sphere.setWidthSegmentCount(widthSegmentCount);
    }

    @ReactProp(name = "heightSegmentCount", defaultInt = VRTSphere.DEFAULT_HEIGHT_SEGMENT)
    public void setHeightSegmentCount(VRTSphere sphere, int heightSegmentCount) {
        sphere.setHeightSegmentCount(heightSegmentCount);
    }

    @ReactProp(name = "radius", defaultFloat = VRTSphere.DEFAULT_RADIUS)
    public void setRadius(VRTSphere sphere, float radius) {
        sphere.setRadius(radius);
    }

    @ReactProp(name = "lightReceivingBitMask", defaultInt = 1)
    public void setLightReceivingBitMask(VRTSphere sphere, int bitMask) {sphere.setLightReceivingBitMask(bitMask); }

    @ReactProp(name = "shadowCastingBitMask", defaultInt = 1)
    public void setShadowCastingBitMask(VRTSphere sphere, int bitMask) {sphere.setShadowCastingBitMask(bitMask); }

}