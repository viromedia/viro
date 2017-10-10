/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;


import android.support.annotation.Nullable;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.VRTViroViewGroupManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

/**
 * The base class for all SceneManagers
 */
public abstract class VRTSceneManager<T extends VRTScene> extends VRTViroViewGroupManager<T> {

    public VRTSceneManager(ReactApplicationContext context){
        super(context);
    }

    @ReactProp(name = "canClick", defaultBoolean = VRTNode.DEFAULT_CAN_CLICK)
    public void setCanClick(VRTScene scene, boolean canClick) {
        scene.setCanClick(canClick);
    }

    @ReactProp(name = "canHover", defaultBoolean = VRTNode.DEFAULT_CAN_HOVER)
    public void setCanHover(VRTScene scene, boolean canHover) {
        scene.setCanHover(canHover);
    }

    @ReactProp(name = "canFuse", defaultBoolean = VRTNode.DEFAULT_CAN_FUSE)
    public void setCanFuse(VRTScene scene, boolean canFuse) {
        scene.setCanFuse(canFuse);
    }

    @ReactProp(name = "timeToFuse", defaultFloat = VRTNode.DEFAULT_TIME_TO_FUSE_MILLIS)
    public void setTimeToFuse(VRTScene scene, float durationMillis) {
        scene.setTimeToFuse(durationMillis);
    }
    @ReactProp(name = "soundRoom")
    public void setSoundRoom(VRTScene scene, ReadableMap soundRoom) {
        scene.setSoundRoom(soundRoom);
    }

    @ReactProp(name = "postProcessEffects")
    public void setPostProcessEffects(VRTScene scene, @Nullable ReadableArray effects) {
        scene.setPostProcessEffects(effects);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_FUSE, MapBuilder.of("registrationName", ViroEvents.ON_FUSE),
                ViroEvents.ON_HOVER, MapBuilder.of("registrationName", ViroEvents.ON_HOVER),
                ViroEvents.ON_CLICK, MapBuilder.of("registrationName", ViroEvents.ON_CLICK),
                ViroEvents.ON_PLATFORM_UPDATE, MapBuilder.of("registrationName", ViroEvents.ON_PLATFORM_UPDATE));
    }

    @ReactProp(name = "physicsWorld")
    public void setPhysicsWorld(VRTScene scene, ReadableMap map) {
        scene.setPhysicsWorld(map);
    }
}
