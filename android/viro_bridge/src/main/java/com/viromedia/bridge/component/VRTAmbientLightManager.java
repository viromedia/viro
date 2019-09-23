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
