/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.Node;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class ControllerManager extends ViroViewGroupManager<Controller> {

    public ControllerManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTController";
    }

    @Override
    public Controller createViewInstance(ThemedReactContext reactContext) {
        return new Controller(getContext());
    }

    @ReactProp(name = "reticleVisibility", defaultBoolean = Controller.DEFAULT_RETICLE_VISIBILITY)
    public void setReticleVisibility(Controller view, boolean reticleVisibility) {
        view.setReticleVisibility(reticleVisibility);
    }

    @ReactProp(name = "controllerVisibility", defaultBoolean = Controller.DEFAULT_CONTROLLER_VISIBILITY)
    public void setControllerVisibility(Controller view, boolean controllerVisibility) {
        view.setControllerVisibility(controllerVisibility);
    }

    @ReactProp(name = "canClick", defaultBoolean = Controller.DEFAULT_CAN_CLICK)
    public void setCanClick(Controller view, boolean canClick) {
        view.setCanClick(canClick);
    }

    @ReactProp(name = "canTouch", defaultBoolean = Controller.DEFAULT_CAN_TOUCH)
    public void setCanTouch(Controller view, boolean canTouch) {
        view.setCanTouch(canTouch);
    }

    @ReactProp(name = "canSwipe", defaultBoolean = Controller.DEFAULT_CAN_SWIPE)
    public void setCanSwipe(Controller view, boolean canSwipe) {
        view.setCanSwipe(canSwipe);
    }

    @ReactProp(name = "canScroll", defaultBoolean = Controller.DEFAULT_CAN_SCROLL)
    public void setCanScroll(Controller view, boolean canScroll) {
        view.setCanScroll(canScroll);
    }

    @ReactProp(name = "canFuse", defaultBoolean = Controller.DEFAULT_CAN_FUSE)
    public void setCanFuse(Controller view, boolean canFuse) {
        view.setCanFuse(canFuse);
    }

    @ReactProp(name = "canGetControllerStatus", defaultBoolean = Controller.DEFAULT_CAN_GET_CONTROLLER_STATUS)
    public void setCanGetControllerStatus(Controller view, boolean canGetControllerStatus) {
        view.setCanGetControllerStatus(canGetControllerStatus);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_CLICK, MapBuilder.of("registrationName", ViroEvents.ON_CLICK),
                ViroEvents.ON_TOUCH, MapBuilder.of("registrationName", ViroEvents.ON_TOUCH),
                ViroEvents.ON_SWIPE, MapBuilder.of("registrationName", ViroEvents.ON_SWIPE),
                ViroEvents.ON_SCROLL, MapBuilder.of("registrationName", ViroEvents.ON_SCROLL),
                ViroEvents.ON_FUSE, MapBuilder.of("registrationName", ViroEvents.ON_FUSE),
                ViroEvents.ON_CONTROLLER_STATUS, MapBuilder.of("registrationName", ViroEvents.ON_CONTROLLER_STATUS));
    }
}
