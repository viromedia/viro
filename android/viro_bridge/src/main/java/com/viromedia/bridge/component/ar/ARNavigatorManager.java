/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.component.ar;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.ViroViewGroupManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

/**
 * ARNavigatorManager for building a {@link ARNavigator}
 * corresponding to the ViroARNavigator.js control.
 */
public class ARNavigatorManager extends ViroViewGroupManager<ARNavigator> {

    public ARNavigatorManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTARNavigator";
    }

    @Override
    protected ARNavigator createViewInstance(ThemedReactContext reactContext) {
        return new ARNavigator(reactContext);
    }

    @ReactProp(name = "currentSceneIndex")
    public void setCurrentSceneIndex(ARNavigator view, int selectedIndex) {
        // no-op
    }

    @ReactProp(name = "apiKey")
    public void setApiKey(ARNavigator view, String apiKey) {
        // no-op
    }

    @ReactProp(name = "hasOnExitViroCallback", defaultBoolean = false)
    public void setHasOnExitViroCallback(ARNavigator navigator, boolean hasOnExitViroCallback) {
        // no-op
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_EXIT_VIRO, MapBuilder.of("registrationName", ViroEvents.ON_EXIT_VIRO));
    }
}
