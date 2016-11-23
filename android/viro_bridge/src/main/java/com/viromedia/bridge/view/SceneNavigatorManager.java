/**
 * Copyright @2016 ViroMedia. All rights reserved.
 */
package com.viromedia.bridge.view;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.SimpleViewManager;
import com.facebook.react.uimanager.ThemedReactContext;

/**
 * SceneNavigatorManager for building a {@link SceneNavigator} per VRTSceneNavigator.js control.
 */
public class SceneNavigatorManager extends SimpleViewManager<SceneNavigator> {
    private final ReactApplicationContext mContext;

    public SceneNavigatorManager(ReactApplicationContext context){
        mContext = context;
    }

    @Override
    public String getName() {
        return "VRTSceneNavigator";
    }

    @Override
    protected SceneNavigator createViewInstance(ThemedReactContext reactContext) {
        return new SceneNavigator(mContext);
    }
}
