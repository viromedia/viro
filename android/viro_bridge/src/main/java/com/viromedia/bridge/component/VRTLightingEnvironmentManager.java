//  Copyright © 2018 Viro Media. All rights reserved.
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

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class VRTLightingEnvironmentManager extends VRTViroViewGroupManager<VRTLightingEnvironment> {

    public VRTLightingEnvironmentManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTLightingEnvironment";
    }

    @Override
    protected VRTLightingEnvironment createViewInstance(ThemedReactContext reactContext) {
        return new VRTLightingEnvironment(reactContext);
    }

    @ReactProp(name = "source")
    public void setSource(VRTLightingEnvironment view, ReadableMap source) {
        view.setSource(source);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map events = super.getExportedCustomDirectEventTypeConstants();
        events.put(ViroEvents.ON_LOAD_START, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_START));
        events.put(ViroEvents.ON_LOAD_END, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_END));
        events.put(ViroEvents.ON_ERROR, MapBuilder.of("registrationName", ViroEvents.ON_ERROR));
        return events;
    }
}
