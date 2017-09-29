/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class ARSceneManager extends SceneManager<ARScene> {

    public ARSceneManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTARScene";
    }

    @Override
    protected ARScene createViewInstance(ThemedReactContext reactContext) {
        return new ARScene(getContext());
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map map = super.getExportedCustomDirectEventTypeConstants();
        map.put(ViroEvents.ON_AMBIENT_LIGHT_UPDATE, MapBuilder.of("registrationName", ViroEvents.ON_AMBIENT_LIGHT_UPDATE));
        map.put(ViroEvents.ON_TRACKING_INITIALIZED, MapBuilder.of("registrationName", ViroEvents.ON_TRACKING_INITIALIZED));
        return map;
    }

}