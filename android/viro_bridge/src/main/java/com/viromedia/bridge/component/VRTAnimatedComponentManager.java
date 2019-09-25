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
        return new VRTAnimatedComponent(reactContext);
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
        Map events = super.getExportedCustomDirectEventTypeConstants();

        events.put(ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START));
        events.put(ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH));

        return events;
    }
}