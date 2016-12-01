/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;

/**
 * SceneManager for building a {@link Scene}
 * corresponding to the ViroScene.js control.
 */
public class SceneManager extends ViroViewGroupManager<Scene> {
    protected final ReactApplicationContext mContext;

    public SceneManager(ReactApplicationContext context){
        mContext = context;
    }

    @Override
    public String getName() {
        return "VRTScene";
    }

    @Override
    protected Scene createViewInstance(ThemedReactContext reactContext) {
        return new Scene(mContext);
    }

}
