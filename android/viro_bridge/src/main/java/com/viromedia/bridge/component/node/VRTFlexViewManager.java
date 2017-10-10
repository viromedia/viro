/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;


import android.graphics.Color;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;

public class VRTFlexViewManager extends VRTNodeManager<VRTFlexView> {
    public VRTFlexViewManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTFlexView";
    }

    @Override
    protected VRTFlexView createViewInstance(ThemedReactContext reactContext) {
        return new VRTFlexView(getContext());
    }

    @ReactProp(name = "width", defaultFloat = 1)
    public void setWidth(VRTFlexView view, float width) {
        view.setWidth(width);
    }

    @ReactProp(name = "height", defaultFloat = 1)
    public void setHeight(VRTFlexView view, float height) {
        view.setHeight(height);
    }

    @ReactProp(name = "backgroundColor", customType = "Color")
    public void setBackgroundColor(VRTFlexView view, Integer color) {
        if(color == null){
            view.setBackgroundColor(Color.TRANSPARENT);
        } else {
            view.setBackgroundColor((long) color);
        }
    }
}
