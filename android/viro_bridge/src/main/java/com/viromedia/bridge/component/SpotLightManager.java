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

public class SpotLightManager extends ViroViewGroupManager<SpotLight> {

    public SpotLightManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSpotLight";
    }

    @Override
    protected SpotLight createViewInstance(ThemedReactContext reactContext) {
        return new SpotLight(reactContext);
    }

    @ReactProp(name = "color", customType = "Color")
    public void setColor(SpotLight spotLight, @Nullable Integer color) {
        if(color == null) {
            spotLight.setColor(Color.WHITE);
        } else {
            spotLight.setColor(color);
        }
    }

    @ReactProp(name = "intensity", defaultFloat = 1000.0f)
    public void setIntensity(SpotLight spotLight, float intensity) {
        spotLight.setIntensity(intensity);
    }

    @ReactProp(name = "direction")
    public void setDirection(SpotLight spotLight, ReadableArray direction) {
        spotLight.setDirection(Helper.toFloatArray(direction));
    }

    @ReactProp(name = "position")
    public void setPosition(SpotLight spotLight, ReadableArray position) {
        spotLight.setPosition(Helper.toFloatArray(position));
    }

    @ReactProp(name = "attenuationStartDistance", defaultFloat = 2f)
    public void setAttenuationStartDistance(SpotLight spotLight, float attenuationStartDistance) {
        spotLight.setAttenuationStartDistance(attenuationStartDistance);
    }

    @ReactProp(name = "attenuationEndDistance", defaultFloat = 10f)
    public void setAttenuationEndDistance(SpotLight spotLight, float attenuationEndDistance) {
        spotLight.setAttenuationEndDistance(attenuationEndDistance);
    }

    @ReactProp(name = "innerAngle", defaultFloat = 0f)
    public void setInnerAngle(SpotLight spotLight, float innerAngle) {
        spotLight.setInnerAngle(innerAngle);
    }

    @ReactProp(name = "outerAngle", defaultFloat = 45f)
    public void setOuterAngle(SpotLight spotLight, float outerAngle) {
        spotLight.setOuterAngle(outerAngle);
    }
}
