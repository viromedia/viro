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

import android.graphics.Color;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.VRTNodeManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

import javax.annotation.Nullable;

public class VRTTextManager extends VRTControlManager<VRTText> {
    public VRTTextManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTText";
    }

    @Override
    protected VRTText createViewInstance(ThemedReactContext reactContext) {
        return new VRTText(reactContext);
    }

    @ReactProp(name = "width", defaultFloat = VRTText.DEFAULT_WIDTH)
    public void setWidth(VRTText text, float width) {
        text.setWidth(width);
    }

    @ReactProp(name = "height", defaultFloat = VRTText.DEFAULT_HEIGHT)
    public void setHeight(VRTText text, float height) {
        text.setHeight(height);
    }

    @ReactProp(name = "extrusionDepth", defaultFloat = 0)
    public void setExtrusionDepth(VRTText text, float extrusionDepth) { text.setExtrusionDepth(extrusionDepth); }

    @ReactProp(name = "text")
    public void setText(VRTText text, String textString) {
        text.setText(textString);
    }

    @ReactProp(name = "color", customType = "Color")
    public void setColor(VRTText text, @Nullable Integer color) {
        if(color == null) {
            text.setColor(Color.WHITE);
        } else {
            text.setColor(color);
        }
    }

    @ReactProp(name = "fontFamily")
    public void setFontFamily(VRTText text, String fontFamily) {
        text.setFontFamilyName(fontFamily);
    }

    @ReactProp(name = "fontSize", defaultInt = VRTText.DEFAULT_FONT_SIZE)
    public void setFontSize(VRTText text, int size) {
        text.setSize(size);
    }

    @ReactProp(name = "fontStyle")
    public void setFontStyle(VRTText text, String fontStyle) {
        text.setFontStyle(fontStyle);
    }

    @ReactProp(name = "fontWeight")
    public void setFontWeight(VRTText text, String fontWeight) {
        text.setFontWeight(fontWeight);
    }

    @ReactProp(name = "outerStroke")
    public void setOuterStroke(VRTText text, @androidx.annotation.Nullable ReadableMap map) {
        text.setOuterStroke(map);
    }

    @ReactProp(name = "maxLines", defaultInt = VRTText.DEFAULT_MAX_LINES)
    public void setMaxLines(VRTText text, int maxLines) {
        text.setMaxLines(maxLines);
    }

    @ReactProp(name = "textClipMode")
    public void setTextClipMode(VRTText text, String textClipMode) {
        text.setTextClipMode(textClipMode);
    }

    @ReactProp(name = "textAlign")
    public void setHorizontalAlignment(VRTText text, String textAlign) {
        text.setHorizontalAlignment(textAlign);
    }

    @ReactProp(name = "textAlignVertical")
    public void setVerticalAlignment(VRTText text, String textAlignVertical) {
        text.setVerticalAlignment(textAlignVertical);
    }

    @ReactProp(name = "textLineBreakMode")
    public void setTextLineBreakMode(VRTText text, String textLineBreakMode) {
        text.setTextLineBreakMode(textLineBreakMode);
    }

    @ReactProp(name = "lightReceivingBitMask", defaultInt = 1)
    public void setLightReceivingBitMask(VRTText text, int bitMask) {text.setLightReceivingBitMask(bitMask); }

    @ReactProp(name = "shadowCastingBitMask", defaultInt = 1)
    public void setShadowCastingBitMask(VRTText text, int bitMask) {text.setShadowCastingBitMask(bitMask); }

}
