/**
 * Copyright © 2018 Viro Media. All rights reserved.
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
 * SceneNavigatorManager for building a {@link VRT3DSceneNavigator}
 * corresponding to the Viro3DSceneNavigator.js control.
 */
public class VRT3DSceneNavigatorManager extends VRTViroViewGroupManager<VRT3DSceneNavigator> {

    private final ReactViroPackage.ViroPlatform mPlatform;

    public VRT3DSceneNavigatorManager(ReactApplicationContext context,
                                      ReactViroPackage.ViroPlatform platform) {
        super(context);
        mPlatform = platform;
    }

    @Override
    public String getName() {
        return "VRT3DSceneNavigator";
    }

    @Override
    protected VRT3DSceneNavigator createViewInstance(ThemedReactContext reactContext) {
        return new VRT3DSceneNavigator(reactContext, mPlatform);
    }

    @ReactProp(name = "debug", defaultBoolean = false)
    public void setDebug(VRT3DSceneNavigator navigator, boolean debug) {
        navigator.setDebug(debug);
    }

    @ReactProp(name = "hdrEnabled", defaultBoolean = true)
    public void setHDREnabled(VRT3DSceneNavigator navigator, boolean enabled) {
        navigator.setHDREnabled(enabled);
    }

    @ReactProp(name = "pbrEnabled", defaultBoolean = true)
    public void setPBREnabled(VRT3DSceneNavigator navigator, boolean enabled) {
        navigator.setPBREnabled(enabled);
    }

    @ReactProp(name = "bloomEnabled", defaultBoolean = true)
    public void setBloomEnabled(VRT3DSceneNavigator navigator, boolean enabled) {
        navigator.setBloomEnabled(enabled);
    }

    @ReactProp(name = "shadowsEnabled", defaultBoolean = true)
    public void setShadowsEnabled(VRT3DSceneNavigator navigator, boolean enabled) {
        navigator.setShadowsEnabled(enabled);
    }

    @ReactProp(name = "currentSceneIndex")
    public void setCurrentSceneIndex(VRT3DSceneNavigator view, int selectedIndex) {
        view.setCurrentSceneIndex(selectedIndex);
    }

    @ReactProp(name = "apiKey")
    public void setApiKey(VRT3DSceneNavigator view, String apiKey) {
        view.setApiKey(apiKey);
    }

    @Override
    protected void onAfterUpdateTransaction(VRT3DSceneNavigator view) {
        super.onAfterUpdateTransaction(view);
        if (view instanceof VRT3DSceneNavigator) {
            view.onPropsSet();
        }
    }

    @ReactProp(name = "hasOnExitViroCallback", defaultBoolean = false)
    public void setHasOnExitViroCallback(VRT3DSceneNavigator navigator, boolean hasOnExitViroCallback) {
        navigator.setHasOnExitViroCallback(hasOnExitViroCallback);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {

        Map events = super.getExportedCustomDirectEventTypeConstants();

        events.put(ViroEvents.ON_EXIT_VIRO, MapBuilder.of("registrationName", ViroEvents.ON_EXIT_VIRO));

        return events;
    }
}
