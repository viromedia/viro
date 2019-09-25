//  Copyright © 2017 Viro Media. All rights reserved.
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

public class VRTControllerManager extends VRTViroViewGroupManager<VRTController> {

    public VRTControllerManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTController";
    }

    @Override
    public VRTController createViewInstance(ThemedReactContext reactContext) {
        return new VRTController(reactContext);
    }

    @ReactProp(name = "reticleVisibility", defaultBoolean = VRTController.DEFAULT_RETICLE_VISIBILITY)
    public void setReticleVisibility(VRTController view, boolean reticleVisibility) {
        view.setReticleVisibility(reticleVisibility);
    }

    @ReactProp(name = "controllerVisibility", defaultBoolean = VRTController.DEFAULT_CONTROLLER_VISIBILITY)
    public void setControllerVisibility(VRTController view, boolean controllerVisibility) {
        view.setControllerVisibility(controllerVisibility);
    }

    @ReactProp(name = "canClick", defaultBoolean = VRTController.DEFAULT_CAN_CLICK)
    public void setCanClick(VRTController view, boolean canClick) {
        view.setCanClick(canClick);
    }

    @ReactProp(name = "canTouch", defaultBoolean = VRTController.DEFAULT_CAN_TOUCH)
    public void setCanTouch(VRTController view, boolean canTouch) {
        view.setCanTouch(canTouch);
    }

    @ReactProp(name = "canSwipe", defaultBoolean = VRTController.DEFAULT_CAN_SWIPE)
    public void setCanSwipe(VRTController view, boolean canSwipe) {
        view.setCanSwipe(canSwipe);
    }

    @ReactProp(name = "canScroll", defaultBoolean = VRTController.DEFAULT_CAN_SCROLL)
    public void setCanScroll(VRTController view, boolean canScroll) {
        view.setCanScroll(canScroll);
    }

    @ReactProp(name = "canFuse", defaultBoolean = VRTController.DEFAULT_CAN_FUSE)
    public void setCanFuse(VRTController view, boolean canFuse) {
        view.setCanFuse(canFuse);
    }

    @ReactProp(name = "canGetControllerStatus", defaultBoolean = VRTController.DEFAULT_CAN_GET_CONTROLLER_STATUS)
    public void setCanGetControllerStatus(VRTController view, boolean canGetControllerStatus) {
        view.setCanGetControllerStatus(canGetControllerStatus);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map events = super.getExportedCustomDirectEventTypeConstants();

        events.put(ViroEvents.ON_CLICK, MapBuilder.of("registrationName", ViroEvents.ON_CLICK));
        events.put(ViroEvents.ON_TOUCH, MapBuilder.of("registrationName", ViroEvents.ON_TOUCH));
        events.put(ViroEvents.ON_SWIPE, MapBuilder.of("registrationName", ViroEvents.ON_SWIPE));
        events.put(ViroEvents.ON_SCROLL, MapBuilder.of("registrationName", ViroEvents.ON_SCROLL));
        events.put(ViroEvents.ON_FUSE, MapBuilder.of("registrationName", ViroEvents.ON_FUSE));
        events.put(ViroEvents.ON_DRAG, MapBuilder.of("registrationName", ViroEvents.ON_DRAG));
        events.put(ViroEvents.ON_CONTROLLER_STATUS, MapBuilder.of("registrationName", ViroEvents.ON_CONTROLLER_STATUS));

        return events;
    }
}
