/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.NodeManager;

public class SurfaceManager extends NodeManager<Surface> {

    public SurfaceManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSurface";
    }

    @Override
    protected Surface createViewInstance(ThemedReactContext reactContext) {
        return new Surface(getContext());
    }

    @ReactProp(name = "width", defaultFloat = 1)
    public void setWidth(Surface view, float width) {
        view.setWidth(width);
    }

    @ReactProp(name = "height", defaultFloat = 1)
    public void setHeight(Surface view, float height) {
        view.setHeight(height);
    }

}
