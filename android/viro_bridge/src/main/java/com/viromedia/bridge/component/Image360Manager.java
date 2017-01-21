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

public class Image360Manager extends ViroViewGroupManager<Image360> {

    public Image360Manager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRT360Image";
    }

    @Override
    protected Image360 createViewInstance(ThemedReactContext reactContext) {
        return new Image360(getContext());
    }

    @ReactProp(name = "source")
    public void setSource(Image360 view, ReadableMap source) {
        view.setSource(source);
    }

    @ReactProp(name = "rotation")
    public void setRotation(Image360 view, ReadableArray rotation) {
        view.setRotation(rotation);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_LOAD_START, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_START),
                ViroEvents.ON_LOAD_END, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_END));
    }
}
