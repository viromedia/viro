/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;

public class NodeContainerManager extends NodeManager<NodeContainer> {
    public NodeContainerManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTViewContainer";
    }

    @Override
    protected NodeContainer createViewInstance(ThemedReactContext reactContext) {
        return new NodeContainer(getContext());
    }
}
