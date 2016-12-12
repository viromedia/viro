/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ViewGroupManager;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;

/**
 * SceneNavigatorManager for building a {@link SceneNavigator}
 * corresponding to the ViroSceneNavigator.js control.
 */
public class SceneNavigatorManager extends ViroViewGroupManager<SceneNavigator> {

    public SceneNavigatorManager(ReactApplicationContext context){
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSceneNavigator";
    }

    @Override
    protected SceneNavigator createViewInstance(ThemedReactContext reactContext) {
        return new SceneNavigator(getContext());
    }

    @ReactProp(name = "vrModeEnabled", defaultBoolean = true)
    public void setVrModeEnabled(SceneNavigator navigator, boolean vrModeEnabled) {
        navigator.setVrModeEnabled(vrModeEnabled);
    }
}
