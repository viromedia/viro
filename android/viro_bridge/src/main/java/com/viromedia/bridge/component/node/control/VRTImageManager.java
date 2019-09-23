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

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.VRTNodeManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;


public class VRTImageManager extends VRTControlManager<VRTImage> {

    public VRTImageManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTImage";
    }

    @Override
    protected VRTImage createViewInstance(ThemedReactContext reactContext) {
        return new VRTImage(reactContext);
    }

    @ReactProp(name = "source")
    public void setSource(VRTImage view, ReadableMap map) {
        view.setSource(map);
    }

    @ReactProp(name = "placeholderSource")
    public void setPlaceholderSource(VRTImage view, ReadableMap map) {
        view.setPlaceholderSource(map);
    }

    @ReactProp(name = "width", defaultFloat = 1)
    public void setWidth(VRTImage view, float width) {
        view.setWidth(width);
    }

    @ReactProp(name = "height", defaultFloat = 1)
    public void setHeight(VRTImage view, float height) {
        view.setHeight(height);
    }

    @ReactProp(name = "resizeMode")
    public void setResizeMode(VRTImage view, String resizeMode) {
        view.setResizeMode(resizeMode);
    }

    @ReactProp(name = "imageClipMode")
    public void setImageClipMode(VRTImage view, String imageClipMode) {
        view.setImageClipMode(imageClipMode);
    }

    @ReactProp(name = "mipmap")
    public void setMipmap(VRTImage view, boolean mipmap) { view.setMipmap(mipmap); }

    @ReactProp(name = "format")
    public void setFormat(VRTImage view, String format) { view.setFormat(format); }

    @ReactProp(name = "stereoMode")
    public void setStereoMode(VRTImage view, String mode) { view.setStereoMode(mode); }

    @ReactProp(name = "lightReceivingBitMask", defaultInt = 1)
    public void setLightReceivingBitMask(VRTImage view, int bitMask) {view.setLightReceivingBitMask(bitMask); }

    @ReactProp(name = "shadowCastingBitMask", defaultInt = 1)
    public void setShadowCastingBitMask(VRTImage view, int bitMask) {view.setShadowCastingBitMask(bitMask); }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map events = super.getExportedCustomDirectEventTypeConstants();

        events.put(ViroEvents.ON_LOAD_START, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_START));
        events.put(ViroEvents.ON_LOAD_END, MapBuilder.of("registrationName", ViroEvents.ON_LOAD_END));
        events.put(ViroEvents.ON_ERROR, MapBuilder.of("registrationName", ViroEvents.ON_ERROR));

        return events;
    }

}
