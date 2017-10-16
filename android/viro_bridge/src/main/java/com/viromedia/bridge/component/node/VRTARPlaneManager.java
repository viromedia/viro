/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class VRTARPlaneManager extends VRTNodeManager<VRTARPlane> {

    public VRTARPlaneManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTARPlane";
    }

    @Override
    protected VRTARPlane createViewInstance(ThemedReactContext reactContext) {
        return new VRTARPlane(getContext());
    }

    @ReactProp(name="minWidth", defaultFloat = 0f)
    public void setMinWidth(VRTARPlane arPlane, float minWidth) {
        arPlane.setMinWidth(minWidth);
    }

    @ReactProp(name="minHeight", defaultFloat = 0f)
    public void setMinHeight(VRTARPlane arPlane, float minHeight) {
        arPlane.setMinHeight(minHeight);
    }

    @ReactProp(name="anchorId")
    public void setAnchorId(VRTARPlane arPlane, String anchorId) {
        arPlane.setAnchorId(anchorId);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map map = super.getExportedCustomDirectEventTypeConstants();
        map.put(ViroEvents.ON_ANCHOR_FOUND, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_FOUND));
        map.put(ViroEvents.ON_ANCHOR_UPDATED, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_UPDATED));
        map.put(ViroEvents.ON_ANCHOR_REMOVED, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_REMOVED));
        return map;
    }
}
