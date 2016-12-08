/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;


import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.ViewGroupManager;

public class FlexViewManager extends ViewGroupManager<FlexView> {
    @Override
    public String getName() {
        return "VRTFlexView";
    }

    @Override
    protected FlexView createViewInstance(ThemedReactContext reactContext) {
        return new FlexView(reactContext);
    }
}
