/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
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
public class VRTSphereManager extends VRTNodeManager<VRTSphere> {

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

    @ReactProp(name = "highAccuracyGaze", defaultBoolean = VRTNode.DEFAULT_HIGH_ACCURACY_GAZE)
    public void setHighAccuracyGaze(VRTSphere sphere, boolean highAccuracyGaze) {
        sphere.setHighAccuracyGaze(highAccuracyGaze);
    }

    @ReactProp(name = "lightReceivingBitMask", defaultInt = 1)
    public void setLightReceivingBitMask(VRTSphere sphere, int bitMask) {sphere.setLightReceivingBitMask(bitMask); }

    @ReactProp(name = "shadowCastingBitMask", defaultInt = 1)
    public void setShadowCastingBitMask(VRTSphere sphere, int bitMask) {sphere.setShadowCastingBitMask(bitMask); }

}