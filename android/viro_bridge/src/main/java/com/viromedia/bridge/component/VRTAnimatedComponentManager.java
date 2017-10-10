/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class VRTAnimatedComponentManager extends VRTViroViewGroupManager<VRTAnimatedComponent> {

    public VRTAnimatedComponentManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTAnimatedComponent";
    }

    @Override
    protected VRTAnimatedComponent createViewInstance(ThemedReactContext reactContext) {
        return new VRTAnimatedComponent(getContext());
    }

    @ReactProp(name = "animation")
    public void setAnimation(VRTAnimatedComponent animatedComponent, String animationName) {
        animatedComponent.setAnimation(animationName);
    }

    @ReactProp(name = "loop", defaultBoolean = false)
    public void setLoop(VRTAnimatedComponent animatedComponent, boolean loop) {
        animatedComponent.setLoop(loop);
    }

    @ReactProp(name = "delay", defaultFloat = 0)
    public void setDelay(VRTAnimatedComponent animatedComponent, float delay) {
        animatedComponent.setDelay(delay);
    }

    @ReactProp(name = "run", defaultBoolean = true)
    public void setRun(VRTAnimatedComponent animatedComponent, boolean run) {
        animatedComponent.setRun(run);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START),
                ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH));
    }
}