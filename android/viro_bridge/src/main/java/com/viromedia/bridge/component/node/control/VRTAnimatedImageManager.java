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
