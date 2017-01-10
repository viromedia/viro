/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.graphics.Color;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;



public class AmbientLightManager extends ViroViewGroupManager<AmbientLight> {

    public AmbientLightManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTAmbientLight";
    }

    @Override
    protected AmbientLight createViewInstance(ThemedReactContext reactContext) {
        return new AmbientLight(reactContext);
    }

    @ReactProp(name = "color", defaultInt = Color.WHITE)
    public void setColor(AmbientLight ambientLight, int color) {
        ambientLight.setColor(color);
    }

}
