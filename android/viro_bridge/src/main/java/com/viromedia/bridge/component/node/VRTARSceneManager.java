/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class VRTARSceneManager extends VRTSceneManager<VRTARScene> {

    public VRTARSceneManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTARScene";
    }

    @Override
    protected VRTARScene createViewInstance(ThemedReactContext reactContext) {
        return new VRTARScene(getContext());
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map map = super.getExportedCustomDirectEventTypeConstants();
        map.put(ViroEvents.ON_AMBIENT_LIGHT_UPDATE, MapBuilder.of("registrationName", ViroEvents.ON_AMBIENT_LIGHT_UPDATE));
        map.put(ViroEvents.ON_TRACKING_INITIALIZED, MapBuilder.of("registrationName", ViroEvents.ON_TRACKING_INITIALIZED));
        map.put(ViroEvents.ON_ANCHOR_FOUND, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_FOUND));
        map.put(ViroEvents.ON_ANCHOR_UPDATED, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_UPDATED));
        map.put(ViroEvents.ON_ANCHOR_REMOVED, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_REMOVED));
        return map;
    }

}