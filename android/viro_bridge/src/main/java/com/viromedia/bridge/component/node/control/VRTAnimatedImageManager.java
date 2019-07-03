/**
 * Copyright © 2018 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import androidx.annotation.Nullable;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.ViroEvents;
import java.util.Map;

/**
 * VRTAnimatedImageManager for building a {@link VRTAnimatedImage}
 * corresponding to the ViroAnimatedImage.js control.
 */
public class VRTAnimatedImageManager extends VRTControlManager<VRTAnimatedImage> {

    public VRTAnimatedImageManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTAnimatedImage";
    }

    @Override
    protected VRTAnimatedImage createViewInstance(ThemedReactContext reactContext) {
        VRTAnimatedImage animImage =  new VRTAnimatedImage(reactContext);
        return animImage;
    }

    @ReactProp(name = "width", defaultFloat = 1f)
    public void setWidth(VRTAnimatedImage view, float width) {
        view.setWidth(width);
    }

    @ReactProp(name = "height", defaultFloat = 1f)
    public void setHeight(VRTAnimatedImage view, float height) {
        view.setHeight(height);
    }

    @ReactProp(name = "source")
    public void setSource(VRTAnimatedImage view, ReadableMap map) {
        view.setSource(map);
    }

    @ReactProp(name = "placeholderSource")
    public void setPlaceholderSource(VRTAnimatedImage view, ReadableMap map) {
        view.setPlaceholderSource(map);
    }

    @ReactProp(name = "resizeMode")
    public void setResizeMode(VRTAnimatedImage view, String resizeMode) {
        view.setResizeMode(resizeMode);
    }

    @ReactProp(name = "imageClipMode")
    public void setImageClipMode(VRTAnimatedImage view, String imageClipMode) {
        view.setImageClipMode(imageClipMode);
    }

    @ReactProp(name = "stereoMode")
    public void setStereoMode(VRTAnimatedImage view, String mode) {
        view.setStereoMode(mode);
    }

    @ReactProp(name = "paused", defaultBoolean = false)
    public void setPaused(VRTAnimatedImage view, boolean paused) {
        view.setPaused(paused);
    }

    @ReactProp(name = "loop", defaultBoolean = true)
    public void setLoop(VRTAnimatedImage view, boolean loop) {
        view.setLoop(loop);
    }


    @ReactProp(name = "lightReceivingBitMask", defaultInt = 1)
    public void setLightReceivingBitMask(VRTAnimatedImage view, int bitMask) {
        view.setLightReceivingBitMask(bitMask);
    }

    @ReactProp(name = "shadowCastingBitMask", defaultInt = 1)
    public void setShadowCastingBitMask(VRTAnimatedImage view, int bitMask) {
        view.setShadowCastingBitMask(bitMask);
    }

    @Override
    public @Nullable Map getExportedCustomDirectEventTypeConstants() {
        Map events = super.getExportedCustomDirectEventTypeConstants();

        events.put(ViroEvents.ON_LOAD_START, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_START));
        events.put(ViroEvents.ON_LOAD_END, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_END));
        events.put(ViroEvents.ON_ERROR, MapBuilder.of("registrationName", ViroEvents.ON_ERROR));

        return events;
    }
}
