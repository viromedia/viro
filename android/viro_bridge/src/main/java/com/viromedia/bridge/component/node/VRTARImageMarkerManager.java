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

package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class VRTARImageMarkerManager extends VRTNodeManager<VRTARImageMarker> {

    public VRTARImageMarkerManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTARImageMarker";
    }

    @Override
    protected VRTARImageMarker createViewInstance(ThemedReactContext reactContext) {
        return new VRTARImageMarker(reactContext);
    }

    @ReactProp(name="pauseUpdates", defaultBoolean = false)
    public void setPauseUpdates(VRTARImageMarker arImageMarker, boolean pauseUpdates) {
        arImageMarker.setPauseUpdates(pauseUpdates);
    }

    @ReactProp(name="target")
    public void setTarget(VRTARImageMarker arImageMarker, String targetName) {
        arImageMarker.setTarget(targetName);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map events = super.getExportedCustomDirectEventTypeConstants();

        events.put(ViroEvents.ON_ANCHOR_FOUND, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_FOUND));
        events.put(ViroEvents.ON_ANCHOR_UPDATED, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_UPDATED));
        events.put(ViroEvents.ON_ANCHOR_REMOVED, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_REMOVED));

        return events;
    }

}
