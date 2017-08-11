/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.graphics.Color;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.Helper;

import javax.annotation.Nullable;


public class DirectionalLightManager extends ViroViewGroupManager<DirectionalLight> {

    public DirectionalLightManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTDirectionalLight";
    }

    @Override
    protected DirectionalLight createViewInstance(ThemedReactContext reactContext) {
        return new DirectionalLight(reactContext);
    }

    @ReactProp(name = "color", customType = "Color")
    public void setColor(DirectionalLight directionalLight, @Nullable Integer color) {
        if(color == null) {
            directionalLight.setColor(Color.WHITE);
        }else {
            directionalLight.setColor(color);
        }
    }

    @ReactProp(name = "intensity", defaultFloat = 1000.0f)
    public void setIntensity(AmbientLight ambientLight, float intensity) {
        ambientLight.setIntensity(intensity);
    }

    @ReactProp(name = "direction")
    public void setDirection(DirectionalLight directionalLight, ReadableArray direction) {
        directionalLight.setDirection(Helper.toFloatArray(direction));
    }
}
