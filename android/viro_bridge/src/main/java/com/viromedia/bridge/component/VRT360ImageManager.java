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

public class VRT360ImageManager extends VRTViroViewGroupManager<VRT360Image> {

    public VRT360ImageManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRT360Image";
    }

    @Override
    protected VRT360Image createViewInstance(ThemedReactContext reactContext) {
        return new VRT360Image(reactContext);
    }

    @ReactProp(name = "source")
    public void setSource(VRT360Image view, ReadableMap source) {
        view.setSource(source);
    }

    @ReactProp(name = "rotation")
    public void setRotation(VRT360Image view, ReadableArray rotation) {
        view.setRotation(rotation);
    }

    @ReactProp(name = "format")
    public void setFormat(VRT360Image view, String format) { view.setFormat(format); }

    @ReactProp(name = "stereoMode")
    public void setStereoMode(VRT360Image view, String mode) { view.setStereoMode(mode); }

    @ReactProp(name = "isHdr", defaultBoolean = false)
    public void setIsHdr(VRT360Image view, boolean hdr) { view.setIsHdr(hdr); }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_LOAD_START, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_START),
                ViroEvents.ON_LOAD_END, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_END),
                ViroEvents.ON_ERROR, MapBuilder.of("registrationName", ViroEvents.ON_ERROR));
    }
}
