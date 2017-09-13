/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.graphics.Color;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.NodeManager;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

import javax.annotation.Nullable;

public class TextManager extends NodeManager<Text> {
    public TextManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTText";
    }

    @Override
    protected Text createViewInstance(ThemedReactContext reactContext) {
        return new Text(getContext());
    }

    @ReactProp(name = "width", defaultFloat = Text.DEFAULT_WIDTH)
    public void setWidth(Text text, float width) {
        text.setWidth(width);
    }

    @ReactProp(name = "height", defaultFloat = Text.DEFAULT_HEIGHT)
    public void setHeight(Text text, float height) {
        text.setHeight(height);
    }

    @ReactProp(name = "text")
    public void setText(Text text, String textString) {
        text.setText(textString);
    }

    @ReactProp(name = "color", customType = "Color")
    public void setColor(Text text, @Nullable Integer color) {
        if(color == null) {
            text.setColor(Color.WHITE);
        } else {
            text.setColor(color);
        }
    }

    @ReactProp(name = "fontFamily")
    public void setFontFamily(Text text, String fontFamily) {
        text.setFontFamilyName(fontFamily);
    }

    @ReactProp(name = "fontSize", defaultInt = Text.DEFAULT_FONT_SIZE)
    public void setFontSize(Text text, int size) {
        text.setSize(size);
    }

    @ReactProp(name = "maxLines", defaultInt = Text.DEFAULT_MAX_LINES)
    public void setMaxLines(Text text, int maxLines) {
        text.setMaxLines(maxLines);
    }

    @ReactProp(name = "textClipMode")
    public void setTextClipMode(Text text, String textClipMode) {
        text.setTextClipMode(textClipMode);
    }

    @ReactProp(name = "textAlign")
    public void setHorizontalAlignment(Text text, String textAlign) {
        text.setHorizontalAlignment(textAlign);
    }

    @ReactProp(name = "textAlignVertical")
    public void setVerticalAlignment(Text text, String textAlignVertical) {
        text.setVerticalAlignment(textAlignVertical);
    }

    @ReactProp(name = "textLineBreakMode")
    public void setTextLineBreakMode(Text text, String textLineBreakMode) {
        text.setTextLineBreakMode(textLineBreakMode);
    }

    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START),
                ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH));
    }

}
