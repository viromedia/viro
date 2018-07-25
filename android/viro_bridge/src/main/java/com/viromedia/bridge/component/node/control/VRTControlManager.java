/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.VRTNode;
import com.viromedia.bridge.component.node.VRTNodeManager;


public abstract class VRTControlManager<T extends VRTControl> extends VRTNodeManager<T> {

    public VRTControlManager(ReactApplicationContext context) {
        super(context);
    }

    @ReactProp(name = "highAccuracyEvents", defaultBoolean = VRTNode.DEFAULT_HIGH_ACCURACY_EVENTS)
    public void setHighAccuracyEvents(VRTControl control, boolean highAccuracyEvents) {
        control.setHighAccuracyEvents(highAccuracyEvents);
    }

}
