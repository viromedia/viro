package com.viromedia.bridge.component.node;

import androidx.annotation.Nullable;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */

public class VRTPortalSceneManager extends VRTNodeManager<VRTPortalScene> {
    public VRTPortalSceneManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTPortalScene";
    }

    @Override
    protected VRTPortalScene createViewInstance(ThemedReactContext reactContext) {
        return new VRTPortalScene(reactContext);
    }

    @ReactProp(name = "passable")
    public void setPassable(VRTPortalScene view, boolean passable) {view.setPassable(passable); }

    @Override
    public @Nullable
    Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_PORTAL_ENTER, MapBuilder.of("registrationName", ViroEvents.ON_PORTAL_ENTER),
                ViroEvents.ON_PORTAL_EXIT, MapBuilder.of("registrationName", ViroEvents.ON_PORTAL_EXIT)
        );
    }

}
