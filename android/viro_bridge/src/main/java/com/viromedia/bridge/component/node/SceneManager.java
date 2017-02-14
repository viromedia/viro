/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.ViroViewGroupManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

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

    @ReactProp(name = "canClick", defaultBoolean = Node.DEFAULT_CAN_CLICK)
    public void setCanClick(Node view, boolean canClick) {
        view.setCanClick(canClick);
    }

    @ReactProp(name = "canHover", defaultBoolean = Node.DEFAULT_CAN_HOVER)
    public void setCanHover(Node view, boolean canHover) {
        view.setCanHover(canHover);
    }

    @ReactProp(name = "soundRoom")
    public void setSoundRoom(Scene scene, ReadableMap soundRoom) {
        scene.setSoundRoom(soundRoom);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_PLATFORM_UPDATE, MapBuilder.of("registrationName", ViroEvents.ON_PLATFORM_UPDATE));
    }
}
