/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.graphics.Color;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;

import javax.annotation.Nullable;


public class VRTAmbientLightManager extends VRTViroViewGroupManager<VRTAmbientLight> {

    public VRTAmbientLightManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTAmbientLight";
    }

    @Override
    protected VRTAmbientLight createViewInstance(ThemedReactContext reactContext) {
        return new VRTAmbientLight(reactContext);
    }

    @ReactProp(name = "color", customType = "Color")
    public void setColor(VRTAmbientLight ambientLight, @Nullable Integer color) {
        if(color == null) {
            ambientLight.setColor(Color.WHITE);
        } else {
            ambientLight.setColor(color);
        }
    }

    @ReactProp(name = "intensity", defaultFloat = 1000.0f)
    public void setIntensity(VRTAmbientLight ambientLight, float intensity) {
        ambientLight.setIntensity(intensity);
    }

    @ReactProp(name = "temperature", defaultFloat = 6500f)
    public void setTemperature(VRTAmbientLight ambientLight, float temperature) {
        ambientLight.setTemperature(temperature);
    }

    @ReactProp(name = "influenceBitMask")
    public void setInfluenceBitMask(VRTAmbientLight ambientLight, int bitMask) {
        ambientLight.setInfluenceBitMask(bitMask);
    }

}
