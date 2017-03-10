/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.support.annotation.Nullable;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.NodeManager;
import com.viromedia.bridge.utility.ViroEvents;


import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Object3dManager for building a {@link Object3d}
 * corresponding to Viro3dObject.js control.
 */
public class Object3dManager extends NodeManager<Object3d> {
    private static final String URI_KEY = "uri";

    public Object3dManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRT3DObject";
    }

    @Override
    protected Object3d createViewInstance(ThemedReactContext reactContext) {
        return new Object3d(getContext());
    }

    @ReactProp(name = "source")
    public void setSource(Object3d object3d, @Nullable ReadableMap map) {
        if (!map.hasKey(URI_KEY)) {
            throw new IllegalArgumentException("Unable to find \"uri\" key in given source map.");
        }

        object3d.setSource(map.getString(URI_KEY));
    }

    @ReactProp(name = "resources")
    public void setResources(Object3d object3d, @Nullable ReadableArray resources) {
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

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_LOAD_START, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_START),
                ViroEvents.ON_LOAD_END, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_END));
    }
}
