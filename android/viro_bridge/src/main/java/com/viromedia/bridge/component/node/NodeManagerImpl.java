/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;

/*
 The ViewManager for the ViroNode JS Component
 */
public class NodeManagerImpl extends NodeManager<Node> {
    public NodeManagerImpl(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTViewContainer";
    }

    @Override
    protected Node createViewInstance(ThemedReactContext reactContext) {
        return new Node(getContext());
    }
}
