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


public class VRTDirectionalLightManager extends VRTViroViewGroupManager<VRTDirectionalLight> {

    public VRTDirectionalLightManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTDirectionalLight";
    }

    @Override
    protected VRTDirectionalLight createViewInstance(ThemedReactContext reactContext) {
        return new VRTDirectionalLight(reactContext);
    }

    @ReactProp(name = "color", customType = "Color")
    public void setColor(VRTDirectionalLight directionalLight, @Nullable Integer color) {
        if(color == null) {
            directionalLight.setColor(Color.WHITE);
        }else {
            directionalLight.setColor(color);
        }
    }

    @ReactProp(name = "intensity", defaultFloat = 1000.0f)
    public void setIntensity(VRTDirectionalLight directionalLight, float intensity) {
        directionalLight.setIntensity(intensity);
    }

    @ReactProp(name = "temperature", defaultFloat = 6500f)
    public void setTemperature(VRTAmbientLight ambientLight, float temperature) {
        ambientLight.setTemperature(temperature);
    }

    @ReactProp(name = "direction")
    public void setDirection(VRTDirectionalLight directionalLight, ReadableArray direction) {
        directionalLight.setDirection(Helper.toFloatArray(direction));
    }

    @ReactProp(name = "influenceBitMask")
    public void setInfluenceBitMask(VRTDirectionalLight directionalLight, int bitMask) {
        directionalLight.setInfluenceBitMask(bitMask);
    }

    @ReactProp(name = "castsShadow")
    public void setCastsShadow(VRTDirectionalLight directionalLight, boolean castsShadow) {
        directionalLight.setCastsShadow(castsShadow);
    }

    @ReactProp(name = "shadowOpacity")
    public void setShadowOpacity(VRTDirectionalLight directionalLight, float opacity) {
        directionalLight.setShadowOpacity(opacity);
    }

    @ReactProp(name = "shadowOrthographicPosition")
    public void setShadowOrthographicPosition(VRTDirectionalLight directionalLight, ReadableArray position) {
        directionalLight.setShadowOrthographicPosition(Helper.toFloatArray(position));
    }

    @ReactProp(name = "shadowOrthographicSize")
    public void setShadowOrthographicSize(VRTDirectionalLight directionalLight, float size) {
        directionalLight.setShadowOrthographicSize(size);
    }

    @ReactProp(name = "shadowMapSize")
    public void setShadowMapSize(VRTDirectionalLight directionalLight, int mapSize) {
        directionalLight.setShadowMapSize(mapSize);
    }

    @ReactProp(name = "shadowBias")
    public void setShadowBias(VRTDirectionalLight directionalLight, float bias) {
        directionalLight.setShadowBias(bias);
    }

    @ReactProp(name = "shadowNearZ")
    public void setShadowNearZ(VRTDirectionalLight directionalLight, float nearZ) {
        directionalLight.setShadowNearZ(nearZ);
    }

    @ReactProp(name = "shadowFarZ")
    public void setShadowFarZ(VRTDirectionalLight directionalLight, float farZ) {
        directionalLight.setShadowFarZ(farZ);
    }
}
