/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.ViroViewGroupManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

/**
 * The base class for all SceneManagers
 */
public abstract class SceneManager<T extends Scene> extends ViroViewGroupManager<T> {

    public SceneManager(ReactApplicationContext context){
        super(context);
    }

    @ReactProp(name = "canClick", defaultBoolean = Node.DEFAULT_CAN_CLICK)
    public void setCanClick(Scene scene, boolean canClick) {
        scene.setCanClick(canClick);
    }

    @ReactProp(name = "canHover", defaultBoolean = Node.DEFAULT_CAN_HOVER)
    public void setCanHover(Scene scene, boolean canHover) {
        scene.setCanHover(canHover);
    }

    @ReactProp(name = "canFuse", defaultBoolean = Node.DEFAULT_CAN_FUSE)
    public void setCanFuse(Scene scene, boolean canFuse) {
        scene.setCanFuse(canFuse);
    }

    @ReactProp(name = "timeToFuse", defaultFloat = Node.DEFAULT_TIME_TO_FUSE_MILLIS)
    public void setTimeToFuse(Scene scene, float durationMillis) {
        scene.setTimeToFuse(durationMillis);
    }
    @ReactProp(name = "soundRoom")
    public void setSoundRoom(Scene scene, ReadableMap soundRoom) {
        scene.setSoundRoom(soundRoom);
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
    public void setPhysicsWorld(Scene scene, ReadableMap map) {
        scene.setPhysicsWorld(map);
    }
}
