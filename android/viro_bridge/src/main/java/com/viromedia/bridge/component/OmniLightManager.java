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

public class OmniLightManager extends ViroViewGroupManager<OmniLight> {

    public OmniLightManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTOmniLight";
    }

    @Override
    protected OmniLight createViewInstance(ThemedReactContext reactContext) {
        return new OmniLight(reactContext);
    }

    @ReactProp(name = "color", customType = "Color")
    public void setColor(OmniLight omniLight, @Nullable Integer color) {
        if(color == null) {
            omniLight.setColor(Color.WHITE);
        } else {
            omniLight.setColor(color);
        }
    }

    @ReactProp(name = "position")
    public void setPosition(OmniLight omniLight, ReadableArray position) {
        omniLight.setPosition(Helper.toFloatArray(position));
    }

    @ReactProp(name = "attenuationStartDistance", defaultFloat = 2f)
    public void setAttenuationStartDistance(OmniLight omniLight, float attenuationStartDistance) {
        omniLight.setAttenuationStartDistance(attenuationStartDistance);
    }

    @ReactProp(name = "attenuationEndDistance", defaultFloat = 10f)
    public void setAttenuationEndDistance(OmniLight omniLight, float attenuationEndDistance) {
        omniLight.setAttenuationEndDistance(attenuationEndDistance);
    }
}
