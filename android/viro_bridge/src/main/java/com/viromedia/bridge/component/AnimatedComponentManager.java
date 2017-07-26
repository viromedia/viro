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

public class AnimatedComponentManager extends ViroViewGroupManager<AnimatedComponent> {

    public AnimatedComponentManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTAnimatedComponent";
    }

    @Override
    protected AnimatedComponent createViewInstance(ThemedReactContext reactContext) {
        return new AnimatedComponent(getContext());
    }

    @ReactProp(name = "animation")
    public void setAnimation(AnimatedComponent animatedComponent, String animationName) {
        animatedComponent.setAnimation(animationName);
    }

    @ReactProp(name = "loop", defaultBoolean = false)
    public void setLoop(AnimatedComponent animatedComponent, boolean loop) {
        animatedComponent.setLoop(loop);
    }

    @ReactProp(name = "delay", defaultFloat = 0)
    public void setDelay(AnimatedComponent animatedComponent, float delay) {
        animatedComponent.setDelay(delay);
    }

    @ReactProp(name = "run", defaultBoolean = true)
    public void setRun(AnimatedComponent animatedComponent, boolean run) {
        animatedComponent.setRun(run);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START),
                ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH));
    }
}