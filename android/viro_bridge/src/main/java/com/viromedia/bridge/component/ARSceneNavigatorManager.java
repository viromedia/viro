/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;

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
        return "VRTARSceneNavigator";
    }

    @Override
    protected ARSceneNavigator createViewInstance(ThemedReactContext reactContext) {
        return new ARSceneNavigator(getContext());
    }

    @ReactProp(name = "currentSceneIndex")
    public void setCurrentSceneIndex(ARSceneNavigator view, int selectedIndex) {
        view.setCurrentSceneIndex(selectedIndex);
    }

    @ReactProp(name = "apiKey")
    public void setApiKey(ARSceneNavigator view, String apiKey) {
        view.setApiKey(apiKey);
    }
}
