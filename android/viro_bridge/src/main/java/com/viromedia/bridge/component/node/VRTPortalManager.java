package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;


/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */

public class VRTPortalManager extends VRTNodeManager<VRTPortal> {

    public VRTPortalManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    protected VRTPortal createViewInstance(ThemedReactContext reactContext) {
        return new VRTPortal(getContext());
    }

    @Override
    public String getName() {
        return "VRTPortal";
    }


}
