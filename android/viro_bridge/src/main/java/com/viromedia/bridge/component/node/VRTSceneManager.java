//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

package com.viromedia.bridge.component.node;

import androidx.annotation.Nullable;

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

    @ReactProp(name = "canCameraTransformUpdate", defaultBoolean = VRTNode.DEFAULT_CAN_CAMERA_TRANSFORM_UPDATE)
    public void setCanCameraTransformUpdate(VRTScene scene, boolean canCameraTransformUpdate) {
        scene.setCanCameraTransformUpdate(canCameraTransformUpdate);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map map = MapBuilder.of(ViroEvents.ON_FUSE, MapBuilder.of("registrationName", ViroEvents.ON_FUSE));
        map.put(ViroEvents.ON_HOVER, MapBuilder.of("registrationName", ViroEvents.ON_HOVER));
        map.put(ViroEvents.ON_CLICK, MapBuilder.of("registrationName", ViroEvents.ON_CLICK));
        map.put(ViroEvents.ON_SWIPE, MapBuilder.of("registrationName", ViroEvents.ON_SWIPE));
        map.put(ViroEvents.ON_SCROLL, MapBuilder.of("registrationName", ViroEvents.ON_SCROLL));
        map.put(ViroEvents.ON_COLLIDED, MapBuilder.of("registrationName", ViroEvents.ON_COLLIDED));
        map.put(ViroEvents.ON_PLATFORM_UPDATE, MapBuilder.of("registrationName", ViroEvents.ON_PLATFORM_UPDATE));
        map.put(ViroEvents.ON_CAMERA_TRANSFORM_UPDATE, MapBuilder.of("registrationName", ViroEvents.ON_CAMERA_TRANSFORM_UPDATE));
        return map;
    }

    @ReactProp(name = "physicsWorld")
    public void setPhysicsWorld(VRTScene scene, ReadableMap map) {
        scene.setPhysicsWorld(map);
    }
}
