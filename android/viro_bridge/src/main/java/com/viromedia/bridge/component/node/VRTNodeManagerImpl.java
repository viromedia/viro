/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;

/*
 The ViewManager for the ViroNode JS Component
 */
public class VRTNodeManagerImpl extends VRTNodeManager<VRTNode> {
    public VRTNodeManagerImpl(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTViewContainer";
    }

    @Override
    protected VRTNode createViewInstance(ThemedReactContext reactContext) {
        return new VRTNode(getContext());
    }
}
