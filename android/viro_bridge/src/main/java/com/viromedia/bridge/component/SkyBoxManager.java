/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import javax.annotation.Nullable;

import java.util.Map;

public class SkyBoxManager extends ViroViewGroupManager<SkyBox> {

    public SkyBoxManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSkybox";
    }

    @Override
    protected SkyBox createViewInstance(ThemedReactContext reactContext) {
        return new SkyBox(getContext());
    }

    @ReactProp(name = "source")
    public void setSource(SkyBox view, ReadableMap source) {
        view.setSource(source);
    }

    /**
     * Color is expecting a double because ReactNative doesn't support a long prop type
     */
    @ReactProp(name = "color", customType = "Color")
    public void setColor(SkyBox view, @Nullable Integer color) {
        if(color != null) {
            view.setColor((long) color);
        }
    }

    @ReactProp(name = "format")
    public void setFormat(SkyBox view, String format) { view.setFormat(format); }

    protected final static String SKYBOX_LOAD_START = "onViroSkyBoxLoadStart";
    protected final static String SKYBOX_LOAD_END = "onViroSkyBoxLoadEnd";

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                SKYBOX_LOAD_START, MapBuilder.of("registrationName", SKYBOX_LOAD_START),
                SKYBOX_LOAD_END, MapBuilder.of("registrationName", SKYBOX_LOAD_END));

    }
}
