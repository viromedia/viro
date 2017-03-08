/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.graphics.Color;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;

import javax.annotation.Nullable;


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

    @ReactProp(name = "color", customType = "Color")
    public void setColor(AmbientLight ambientLight, @Nullable Integer color) {
        if(color == null) {
            ambientLight.setColor(Color.WHITE);
        } else {
            ambientLight.setColor(color);
        }
    }

}
