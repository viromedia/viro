/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.component;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

/**
 * ARSceneNavigatorManager for building a {@link ARSceneNavigator}
 * corresponding to the ViroARNavigator.js control.
 */
public class ARSceneNavigatorManager extends ViroViewGroupManager<ARSceneNavigator> {

    public ARSceneNavigatorManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTARNavigator";
    }

    @Override
    protected ARSceneNavigator createViewInstance(ThemedReactContext reactContext) {
        return new ARSceneNavigator(reactContext);
    }

    @ReactProp(name = "currentSceneIndex")
    public void setCurrentSceneIndex(ARSceneNavigator view, int selectedIndex) {
        // no-op
    }

    @ReactProp(name = "apiKey")
    public void setApiKey(ARSceneNavigator view, String apiKey) {
        // no-op
    }

    @ReactProp(name = "hasOnExitViroCallback", defaultBoolean = false)
    public void setHasOnExitViroCallback(ARSceneNavigator navigator, boolean hasOnExitViroCallback) {
        // no-op
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_EXIT_VIRO, MapBuilder.of("registrationName", ViroEvents.ON_EXIT_VIRO));
    }
}
