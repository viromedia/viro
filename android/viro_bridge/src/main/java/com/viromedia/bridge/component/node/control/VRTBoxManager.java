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
 * BoxManager for building a {@link VRTBox}
 * corresponding to the ViroBox.js control.
 */
public class VRTBoxManager extends VRTNodeManager<VRTBox> {

    public VRTBoxManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTBox";
    }

    @Override
    protected VRTBox createViewInstance(ThemedReactContext reactContext) {
        return new VRTBox(getContext());
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

    @ReactProp(name = "highAccuracyGaze", defaultBoolean = VRTNode.DEFAULT_HIGH_ACCURACY_GAZE)
    public void setHighAccuracyGaze(VRTBox box, boolean highAccuracyGaze) {
        box.setHighAccuracyGaze(highAccuracyGaze);
    }

    @ReactProp(name = "lightReceivingBitMask", defaultInt = 1)
    public void setLightReceivingBitMask(VRTBox box, int bitMask) {box.setLightReceivingBitMask(bitMask); }

    @ReactProp(name = "shadowCastingBitMask", defaultInt = 1)
    public void setShadowCastingBitMask(VRTBox box, int bitMask) {box.setShadowCastingBitMask(bitMask); }

    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START),
                ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH));
    }
}
