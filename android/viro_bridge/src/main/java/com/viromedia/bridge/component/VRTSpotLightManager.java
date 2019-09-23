//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

package com.viromedia.bridge.component;


import android.graphics.Color;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.Helper;

import javax.annotation.Nullable;

public class VRTSpotLightManager extends VRTViroViewGroupManager<VRTSpotLight> {

    public VRTSpotLightManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSpotLight";
    }

    @Override
    protected VRTSpotLight createViewInstance(ThemedReactContext reactContext) {
        return new VRTSpotLight(reactContext);
    }

    @ReactProp(name = "color", customType = "Color")
    public void setColor(VRTSpotLight spotLight, @Nullable Integer color) {
        if(color == null) {
            spotLight.setColor(Color.WHITE);
        } else {
            spotLight.setColor(color);
        }
    }

    @ReactProp(name = "intensity", defaultFloat = 1000.0f)
    public void setIntensity(VRTSpotLight spotLight, float intensity) {
        spotLight.setIntensity(intensity);
    }

    @ReactProp(name = "temperature", defaultFloat = 6500f)
    public void setTemperature(VRTAmbientLight ambientLight, float temperature) {
        ambientLight.setTemperature(temperature);
    }

    @ReactProp(name = "direction")
    public void setDirection(VRTSpotLight spotLight, ReadableArray direction) {
        spotLight.setDirection(Helper.toFloatArray(direction));
    }

    @ReactProp(name = "position")
    public void setPosition(VRTSpotLight spotLight, ReadableArray position) {
        spotLight.setPosition(Helper.toFloatArray(position));
    }

    @ReactProp(name = "attenuationStartDistance", defaultFloat = 2f)
    public void setAttenuationStartDistance(VRTSpotLight spotLight, float attenuationStartDistance) {
        spotLight.setAttenuationStartDistance(attenuationStartDistance);
    }

    @ReactProp(name = "attenuationEndDistance", defaultFloat = 10f)
    public void setAttenuationEndDistance(VRTSpotLight spotLight, float attenuationEndDistance) {
        spotLight.setAttenuationEndDistance(attenuationEndDistance);
    }

    @ReactProp(name = "innerAngle", defaultFloat = 0f)
    public void setInnerAngle(VRTSpotLight spotLight, float innerAngle) {
        spotLight.setInnerAngle(innerAngle);
    }

    @ReactProp(name = "outerAngle", defaultFloat = 45f)
    public void setOuterAngle(VRTSpotLight spotLight, float outerAngle) {
        spotLight.setOuterAngle(outerAngle);
    }


    @ReactProp(name = "influenceBitMask")
    public void setInfluenceBitMask(VRTSpotLight spotLight, int bitMask) {
        spotLight.setInfluenceBitMask(bitMask);
    }

    @ReactProp(name = "castsShadow")
    public void setCastsShadow(VRTSpotLight spotLight, boolean castsShadow) {
        spotLight.setCastsShadow(castsShadow);
    }

    @ReactProp(name = "shadowOpacity")
    public void setShadowOpacity(VRTSpotLight spotLight, float opacity) {
        spotLight.setShadowOpacity(opacity);
    }


    @ReactProp(name = "shadowMapSize")
    public void setShadowMapSize(VRTSpotLight spotLight, int mapSize) {
        spotLight.setShadowMapSize(mapSize);
    }

    @ReactProp(name = "shadowBias")
    public void setShadowBias(VRTSpotLight spotLight, float bias) {
        spotLight.setShadowBias(bias);
    }

    @ReactProp(name = "shadowNearZ")
    public void setShadowNearZ(VRTSpotLight spotLight, float nearZ) {
        spotLight.setShadowNearZ(nearZ);
    }

    @ReactProp(name = "shadowFarZ")
    public void setShadowFarZ(VRTSpotLight spotLight, float farZ) {
        spotLight.setShadowFarZ(farZ);
    }
}
