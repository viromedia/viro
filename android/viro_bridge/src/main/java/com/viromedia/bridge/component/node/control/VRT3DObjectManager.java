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

package com.viromedia.bridge.component.node.control;

import android.util.Pair;
import androidx.annotation.Nullable;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.VRTNodeManager;
import com.viromedia.bridge.utility.ViroEvents;


import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Object3dManager for building a {@link VRT3DObject}
 * corresponding to Viro3dObject.js control.
 */
public class VRT3DObjectManager extends VRTControlManager<VRT3DObject> {
    private static final String URI_KEY = "uri";

    public VRT3DObjectManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRT3DObject";
    }

    @Override
    protected VRT3DObject createViewInstance(ThemedReactContext reactContext) {
        return new VRT3DObject(reactContext);
    }

    @ReactProp(name = "source")
    public void setSource(VRT3DObject object3d, @Nullable ReadableMap map) {
        if (!map.hasKey(URI_KEY)) {
            throw new IllegalArgumentException("Unable to find \"uri\" key in given source map.");
        }

        object3d.setSource(map.getString(URI_KEY));
    }

    @ReactProp(name = "resources")
    public void setResources(VRT3DObject object3d, @Nullable ReadableArray resources) {
        List<String> resourceList = null;
        if (resources != null) {
            resourceList = new ArrayList<>();
            for (int i = 0; i < resources.size(); i++) {
                ReadableMap resourceMap = resources.getMap(i);
                if (!resourceMap.hasKey(URI_KEY)) {
                    throw new IllegalArgumentException("Received object resource map without URI key.");
                }
                resourceList.add(resourceMap.getString(URI_KEY));
            }
        }
        object3d.setResources(resourceList);
    }

    @ReactProp(name = "morphTargets")
    public void setMorphTargets(VRT3DObject object3d, @Nullable ReadableArray targets) {
        if (targets == null || targets.size() <=0) {
            return;
        }

        List<Pair<String, Float>> targetList = new ArrayList<>();
        for (int i = 0; i < targets.size(); i++) {
            ReadableMap resourceMap = targets.getMap(i);
            if (!resourceMap.hasKey("target") || !resourceMap.hasKey("weight")) {
                throw new IllegalArgumentException("Received invalid morph target map.");
            }

            String key = resourceMap.getString("target");
            Float weight = new Float(resourceMap.getDouble("weight"));
            targetList.add(new Pair<String, Float>(key, weight));
        }
        object3d.setMorphTargets(targetList);
    }

    @ReactProp(name = "type")
    public void setType(VRT3DObject object3d, @Nullable String type) {
        object3d.setType(type);
    }

    @ReactProp(name = "lightReceivingBitMask", defaultInt = 1)
    public void setLightReceivingBitMask(VRT3DObject object3d, int bitMask) {object3d.setLightReceivingBitMask(bitMask); }

    @ReactProp(name = "shadowCastingBitMask", defaultInt = 1)
    public void setShadowCastingBitMask(VRT3DObject object3d, int bitMask) {object3d.setShadowCastingBitMask(bitMask); }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map events = super.getExportedCustomDirectEventTypeConstants();

        events.put(ViroEvents.ON_LOAD_START, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_START));
        events.put(ViroEvents.ON_LOAD_END, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_END));
        events.put(ViroEvents.ON_ERROR, MapBuilder.of("registrationName", ViroEvents.ON_ERROR));
        events.put(ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START));
        events.put(ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH));

        return events;
    }
}
