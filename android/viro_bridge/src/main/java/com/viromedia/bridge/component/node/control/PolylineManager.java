/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.NodeManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class PolylineManager extends NodeManager<Polyline> {

    public PolylineManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTPolyline";
    }

    @Override
    protected Polyline createViewInstance(ThemedReactContext reactContext) {
        return new Polyline(getContext());
    }

    @ReactProp(name = "points")
    public void setPoints(Polyline polyline, ReadableArray points) {
        polyline.setPoints(points);
    }

    @ReactProp(name = "thickness", defaultFloat = 0.1f)
    public void setThickness(Polyline polyline, float thickness) {
        polyline.setThickness(thickness);
    }

    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START),
                ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH));
    }
}
