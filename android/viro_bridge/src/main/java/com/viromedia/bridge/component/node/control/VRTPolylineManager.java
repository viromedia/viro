/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.VRTNodeManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class VRTPolylineManager extends VRTNodeManager<VRTPolyline> {

    public VRTPolylineManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTPolyline";
    }

    @Override
    protected VRTPolyline createViewInstance(ThemedReactContext reactContext) {
        return new VRTPolyline(reactContext);
    }

    @ReactProp(name = "points")
    public void setPoints(VRTPolyline polyline, ReadableArray points) {
        polyline.setPoints(points);
    }

    @ReactProp(name = "thickness", defaultFloat = 0.1f)
    public void setThickness(VRTPolyline polyline, float thickness) {
        polyline.setThickness(thickness);
    }

    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START),
                ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH));
    }
}
