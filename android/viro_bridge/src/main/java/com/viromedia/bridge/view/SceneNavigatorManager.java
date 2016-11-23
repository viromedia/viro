/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.view;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ViewGroupManager;
import com.facebook.react.uimanager.ThemedReactContext;

/**
 * SceneNavigatorManager for building a {@link SceneNavigator} corresponding to the ViroSceneNavigator.js control.
 */
public class SceneNavigatorManager extends ViewGroupManager<SceneNavigator> {
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
