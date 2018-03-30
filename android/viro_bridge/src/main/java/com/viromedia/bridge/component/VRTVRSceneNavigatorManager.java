/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.ReactViroPackage;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

/**
 * SceneNavigatorManager for building a {@link VRTSceneNavigator}
 * corresponding to the ViroSceneNavigator.js control.
 */
public class VRTVRSceneNavigatorManager extends VRTViroViewGroupManager<VRTSceneNavigator> {

    private final ReactViroPackage.ViroPlatform mPlatform;

    public VRTVRSceneNavigatorManager(ReactApplicationContext context,
                                      ReactViroPackage.ViroPlatform platform) {
        super(context);
        mPlatform = platform;
    }

    @Override
    public String getName() {
        return "VRTVRSceneNavigator";
    }

    @Override
    protected VRTSceneNavigator createViewInstance(ThemedReactContext reactContext) {
        return new VRTSceneNavigator(getContext(), mPlatform);
    }

    @ReactProp(name = "vrModeEnabled", defaultBoolean = true)
    public void setVrModeEnabled(VRTSceneNavigator navigator, boolean vrModeEnabled) {
        navigator.setVrModeEnabled(vrModeEnabled);
    }

    @ReactProp(name = "debug", defaultBoolean = false)
    public void setDebug(VRTSceneNavigator navigator, boolean debug) {
        navigator.setDebug(debug);
    }

    @ReactProp(name = "currentSceneIndex")
    public void setCurrentSceneIndex(VRTSceneNavigator view, int selectedIndex) {
        view.setCurrentSceneIndex(selectedIndex);
    }

    @ReactProp(name = "apiKey")
    public void setApiKey(VRTSceneNavigator view, String apiKey) {
        view.setApiKey(apiKey);
    }

    @Override
    protected void onAfterUpdateTransaction(VRTSceneNavigator view) {
        super.onAfterUpdateTransaction(view);
        if (view instanceof VRTSceneNavigator) {
            ((VRTSceneNavigator) view).onPropsSet();
        }
    }

    @ReactProp(name = "hasOnExitViroCallback", defaultBoolean = false)
    public void setHasOnExitViroCallback(VRTSceneNavigator navigator, boolean hasOnExitViroCallback) {
        navigator.setHasOnExitViroCallback(hasOnExitViroCallback);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_EXIT_VIRO, MapBuilder.of("registrationName", ViroEvents.ON_EXIT_VIRO));
    }
}
