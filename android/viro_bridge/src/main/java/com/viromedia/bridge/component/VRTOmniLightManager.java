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

public class VRTOmniLightManager extends VRTViroViewGroupManager<VRTOmniLight> {

    public VRTOmniLightManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTOmniLight";
    }

    @Override
    protected VRTOmniLight createViewInstance(ThemedReactContext reactContext) {
        return new VRTOmniLight(reactContext);
    }

    @ReactProp(name = "color", customType = "Color")
    public void setColor(VRTOmniLight omniLight, @Nullable Integer color) {
        if(color == null) {
            omniLight.setColor(Color.WHITE);
        } else {
            omniLight.setColor(color);
        }
    }

    @ReactProp(name = "intensity", defaultFloat = 1000.0f)
    public void setIntensity(VRTOmniLight omniLight, float intensity) {
        omniLight.setIntensity(intensity);
    }

    @ReactProp(name = "temperature", defaultFloat = 6500f)
    public void setTemperature(VRTAmbientLight ambientLight, float temperature) {
        ambientLight.setTemperature(temperature);
    }

    @ReactProp(name = "position")
    public void setPosition(VRTOmniLight omniLight, ReadableArray position) {
        omniLight.setPosition(Helper.toFloatArray(position));
    }

    @ReactProp(name = "attenuationStartDistance", defaultFloat = 2f)
    public void setAttenuationStartDistance(VRTOmniLight omniLight, float attenuationStartDistance) {
        omniLight.setAttenuationStartDistance(attenuationStartDistance);
    }

    @ReactProp(name = "attenuationEndDistance", defaultFloat = 10f)
    public void setAttenuationEndDistance(VRTOmniLight omniLight, float attenuationEndDistance) {
        omniLight.setAttenuationEndDistance(attenuationEndDistance);
    }

    @ReactProp(name = "influenceBitMask")
    public void setInfluenceBitMask(VRTOmniLight omniLight, int bitMask) {
        omniLight.setInfluenceBitMask(bitMask);
    }
}
