/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.ViroViewGroupManager;

/**
 * SceneManager for building a {@link Scene}
 * corresponding to the ViroScene.js control.
 */
public class SceneManager extends ViroViewGroupManager<Scene> {

    public SceneManager(ReactApplicationContext context){
        super(context);
    }

    @Override
    public String getName() {
        return "VRTScene";
    }

    @Override
    protected Scene createViewInstance(ThemedReactContext reactContext) {
        return new Scene(getContext());
    }

    @ReactProp(name = "reticleEnabled", defaultBoolean = true)
    public void setReticleEnabled(Scene scene, boolean enable) {
        scene.setReticleEnabled(enable);
    }

    @ReactProp(name = "canTap", defaultBoolean = Node.DEFAULT_CAN_TAP)
    public void setHitable(Node view, boolean canTap) {
        view.setCanTap(canTap);
    }

    @ReactProp(name = "canGaze", defaultBoolean = Node.DEFAULT_CAN_GAZE)
    public void setCanHover(Node view, boolean canGaze) {
        view.setCanGaze(canGaze);
    }

}
