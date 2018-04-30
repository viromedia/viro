/*
 * Copyright © 2018 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class VRTARImageMarkerManager extends VRTNodeManager<VRTARImageMarker> {

    public VRTARImageMarkerManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTARImageMarker";
    }

    @Override
    protected VRTARImageMarker createViewInstance(ThemedReactContext reactContext) {
        return new VRTARImageMarker(reactContext);
    }

    @ReactProp(name="pauseUpdates", defaultBoolean = false)
    public void setPauseUdpates(VRTARImageMarker arImageMarker, boolean pauseUpdates) {
        arImageMarker.setPauseUpdates(pauseUpdates);
    }

    @ReactProp(name="target")
    public void setTarget(VRTARImageMarker arImageMarker, String targetName) {
        arImageMarker.setTarget(targetName);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map events = super.getExportedCustomDirectEventTypeConstants();

        events.put(ViroEvents.ON_ANCHOR_FOUND, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_FOUND));
        events.put(ViroEvents.ON_ANCHOR_UPDATED, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_UPDATED));
        events.put(ViroEvents.ON_ANCHOR_REMOVED, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_REMOVED));

        return events;
    }

}
