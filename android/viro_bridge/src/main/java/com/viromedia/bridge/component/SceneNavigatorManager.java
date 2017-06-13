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
 * SceneNavigatorManager for building a {@link SceneNavigator}
 * corresponding to the ViroSceneNavigator.js control.
 */
public class SceneNavigatorManager extends ViroViewGroupManager<SceneNavigator> {

    private final ReactViroPackage.ViroPlatform mPlatform;

    public SceneNavigatorManager(ReactApplicationContext context,
                                 ReactViroPackage.ViroPlatform platform) {
        super(context);
        mPlatform = platform;
    }

    @Override
    public String getName() {
        return "VRTSceneNavigator";
    }

    @Override
    protected SceneNavigator createViewInstance(ThemedReactContext reactContext) {
        return new SceneNavigator(getContext(), mPlatform);
    }

    @ReactProp(name = "vrModeEnabled", defaultBoolean = true)
    public void setVrModeEnabled(SceneNavigator navigator, boolean vrModeEnabled) {
        navigator.setVrModeEnabled(vrModeEnabled);
    }

    @ReactProp(name = "debug", defaultBoolean = false)
    public void setDebug(SceneNavigator navigator, boolean debug) {
        navigator.setDebug(debug);
    }

    @ReactProp(name = "currentSceneIndex")
    public void setCurrentSceneIndex(SceneNavigator view, int selectedIndex) {
        view.setCurrentSceneIndex(selectedIndex);
    }

    @ReactProp(name = "apiKey")
    public void setApiKey(SceneNavigator view, String apiKey) {
        view.setApiKey(apiKey);
    }

    @Override
    protected void onAfterUpdateTransaction(SceneNavigator view) {
        super.onAfterUpdateTransaction(view);
        if (view instanceof SceneNavigator) {
            ((SceneNavigator) view).onPropsSet();
        }
    }

    @ReactProp(name = "hasOnExitViroCallback", defaultBoolean = false)
    public void setHasOnExitViroCallback(SceneNavigator navigator, boolean hasOnExitViroCallback) {
        navigator.setHasOnExitViroCallback(hasOnExitViroCallback);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_EXIT_VIRO, MapBuilder.of("registrationName", ViroEvents.ON_EXIT_VIRO));
    }
}
