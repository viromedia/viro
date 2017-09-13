/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.NodeManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;


public class ImageManager extends NodeManager<Image> {

    public ImageManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTImage";
    }

    @Override
    protected Image createViewInstance(ThemedReactContext reactContext) {
        return new Image(getContext());
    }

    @ReactProp(name = "source")
    public void setSource(Image view, ReadableMap map) {
        view.setSource(map);
    }

    @ReactProp(name = "placeholderSource")
    public void setPlaceholderSource(Image view, ReadableMap map) {
        view.setPlaceholderSource(map);
    }

    @ReactProp(name = "width", defaultFloat = 1)
    public void setWidth(Image view, float width) {
        view.setWidth(width);
    }

    @ReactProp(name = "height", defaultFloat = 1)
    public void setHeight(Image view, float height) {
        view.setHeight(height);
    }

    @ReactProp(name = "resizeMode")
    public void setResizeMode(Image view, String resizeMode) {
        view.setResizeMode(resizeMode);
    }

    @ReactProp(name = "imageClipMode")
    public void setImageClipMode(Image view, String imageClipMode) {
        view.setImageClipMode(imageClipMode);
    }

    @ReactProp(name = "mipmap")
    public void setMipmap(Image view, boolean mipmap) { view.setMipmap(mipmap); }

    @ReactProp(name = "format")
    public void setFormat(Image view, String format) { view.setFormat(format); }

    @ReactProp(name = "stereoMode")
    public void setStereoMode(Image view, String mode) { view.setStereoMode(mode); }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_LOAD_START, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_START),
                ViroEvents.ON_LOAD_END, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_END),
                ViroEvents.ON_ERROR, MapBuilder.of("registrationName", ViroEvents.ON_ERROR),
                ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START),
                ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH));
    }

}
