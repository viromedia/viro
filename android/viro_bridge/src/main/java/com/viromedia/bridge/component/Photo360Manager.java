/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class Photo360Manager extends ViroViewGroupManager<Photo360> {

    public Photo360Manager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRO360Photo";
    }

    @Override
    protected Photo360 createViewInstance(ThemedReactContext reactContext) {
        return new Photo360(getContext());
    }

    @ReactProp(name = "source")
    public void setSource(Photo360 view, ReadableMap source) {
        view.setSource(source);
    }

    @ReactProp(name = "rotation")
    public void setRotation(Photo360 view, ReadableArray rotation) {
        view.setRotation(rotation);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_LOAD_START, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_START),
                ViroEvents.ON_LOAD_END, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_END));
    }
}
