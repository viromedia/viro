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
    public void setIntensity(DirectionalLight directionalLight, float intensity) {
        directionalLight.setIntensity(intensity);
    }

    @ReactProp(name = "direction")
    public void setDirection(DirectionalLight directionalLight, ReadableArray direction) {
        directionalLight.setDirection(Helper.toFloatArray(direction));
    }

    @ReactProp(name = "influenceBitMask")
    public void setInfluenceBitMask(DirectionalLight directionalLight, int bitMask) {
        directionalLight.setInfluenceBitMask(bitMask);
    }

    @ReactProp(name = "castsShadow")
    public void setCastsShadow(DirectionalLight directionalLight, boolean castsShadow) {
        directionalLight.setCastsShadow(castsShadow);
    }

    @ReactProp(name = "shadowOpacity")
    public void setShadowOpacity(DirectionalLight directionalLight, float opacity) {
        directionalLight.setShadowOpacity(opacity);
    }

    @ReactProp(name = "shadowOrthographicPosition")
    public void setShadowOrthographicPosition(DirectionalLight directionalLight, ReadableArray position) {
        directionalLight.setShadowOrthographicPosition(Helper.toFloatArray(position));
    }

    @ReactProp(name = "shadowOrthographicSize")
    public void setShadowOrthographicSize(DirectionalLight directionalLight, float size) {
        directionalLight.setShadowOrthographicSize(size);
    }

    @ReactProp(name = "shadowMapSize")
    public void setShadowMapSize(DirectionalLight directionalLight, int mapSize) {
        directionalLight.setShadowMapSize(mapSize);
    }

    @ReactProp(name = "shadowBias")
    public void setShadowBias(DirectionalLight directionalLight, float bias) {
        directionalLight.setShadowBias(bias);
    }

    @ReactProp(name = "shadowNearZ")
    public void setShadowNearZ(DirectionalLight directionalLight, float nearZ) {
        directionalLight.setShadowNearZ(nearZ);
    }

    @ReactProp(name = "shadowFarZ")
    public void setShadowFarZ(DirectionalLight directionalLight, float farZ) {
        directionalLight.setShadowFarZ(farZ);
    }
}
