/**
 * Copyright © 2018 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class VRTLightingEnvironmentManager extends VRTViroViewGroupManager<VRTLightingEnvironment> {

    public VRTLightingEnvironmentManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTLightingEnvironment";
    }

    @Override
    protected VRTLightingEnvironment createViewInstance(ThemedReactContext reactContext) {
        return new VRTLightingEnvironment(getContext());
    }

    @ReactProp(name = "source")
    public void setSource(VRTLightingEnvironment view, ReadableMap source) {
        view.setSource(source);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_LOAD_START, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_START),
                ViroEvents.ON_LOAD_END, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_END),
                ViroEvents.ON_ERROR, MapBuilder.of("registrationName", ViroEvents.ON_ERROR));
    }
}
