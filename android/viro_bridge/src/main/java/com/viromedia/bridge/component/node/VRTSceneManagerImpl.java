/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;

/**
 * SceneManager for building a {@link VRTScene}
 * corresponding to the ViroScene.js control.
 */
public class VRTSceneManagerImpl extends VRTSceneManager<VRTScene> {

    public VRTSceneManagerImpl(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTScene";
    }

    @Override
    protected VRTScene createViewInstance(ThemedReactContext reactContext) {
        return new VRTScene(getContext());
    }

}
