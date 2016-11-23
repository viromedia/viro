/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.view;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.SimpleViewManager;
import com.facebook.react.uimanager.ThemedReactContext;

public class SceneManager extends SimpleViewManager<Scene> {
    private final ReactApplicationContext mContext;

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
