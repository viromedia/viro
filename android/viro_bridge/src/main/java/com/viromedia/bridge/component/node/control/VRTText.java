/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.graphics.Color;

import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReactContext;
import com.viro.core.ViroContext;
import com.viro.core.Text;

public class VRTText extends VRTControl {
    static final String DEFAULT_FONT = "Roboto";
    static final float DEFAULT_WIDTH = 1f;
    static final float DEFAULT_HEIGHT = 1f;
    static final long DEFAULT_COLOR = Color.WHITE;
    static final int DEFAULT_MAX_LINES = 0;
    static final int DEFAULT_FONT_SIZE = 14;
    static final Text.FontStyle DEFAULT_FONT_STYLE = Text.FontStyle.Normal;
    static final Text.FontWeight DEFAULT_FONT_WEIGHT = Text.FontWeight.Regular;

    private Text mNativeText = null;
    private long mColor = DEFAULT_COLOR;
    private int mMaxLines = DEFAULT_MAX_LINES;
    private float mWidth = DEFAULT_WIDTH;
    private float mHeight = DEFAULT_HEIGHT;
    private String mText;
    private String mFontFamilyName = DEFAULT_FONT;
    private Text.FontStyle mFontStyle = DEFAULT_FONT_STYLE;
    private Text.FontWeight mFontWeight = DEFAULT_FONT_WEIGHT;
    private int mSize = DEFAULT_FONT_SIZE;
    private String mHorizontalAlignment = "Left";
    private String mVerticalAlignment = "Top";
    private String mTextLineBreakMode = "WordWrap";
    private String mTextClipMode = "ClipToBounds";
    private boolean mNeedsUpdate = false;

    public VRTText(ReactContext context) {
        super(context);
    }

    @Override
    public void onTearDown() {
        if (mNativeText != null) {
            mNativeText.dispose();
            mNativeText = null;
        }

        super.onTearDown();
    }
    public void setWidth(float width) {
        mWidth = width;
        mNeedsUpdate = true;
    }

    public void setHeight(float height) {
        mHeight = height;
        mNeedsUpdate = true;
    }

    public void setColor(long color) {
        mColor = color;
        mNeedsUpdate = true;
    }

    public void setMaxLines(int maxLines) {
        mMaxLines = maxLines;
        mNeedsUpdate = true;
    }

    public void setText(String text) {
        mText = text;
        mNeedsUpdate = true;
    }

    public void setFontFamilyName(String fontFamilyName) {
        if (fontFamilyName == null || fontFamilyName.trim().isEmpty()) {
            return;
        }
        mFontFamilyName = fontFamilyName;
        mNeedsUpdate = true;
    }

    public void setSize(int size) {
        mSize = size;
        mNeedsUpdate = true;
    }

    public void setFontStyle(String style) {
        mFontStyle = getFontStyleEnum(style);
        mNeedsUpdate = true;
    }

    public void setFontWeight(String weight) {
        mFontWeight = getFontWeightEnum(weight);
        mNeedsUpdate = true;
    }

    public void setHorizontalAlignment(String horizontalAlignment) {
        mHorizontalAlignment = horizontalAlignment;
        mNeedsUpdate = true;
    }

    public void setVerticalAlignment(String verticalAlignment) {
        mVerticalAlignment = verticalAlignment;
        mNeedsUpdate = true;
    }

    public void setTextClipMode(String textClipMode) {
        mTextClipMode = textClipMode;
        mNeedsUpdate = true;
    }

    public void setTextLineBreakMode(String textLineBreakMode) {
        mTextLineBreakMode = textLineBreakMode;
        mNeedsUpdate = true;
    }

    private void updateLabel() {
        if (mViroContext == null || isTornDown() || mText == null) {
            return;
        }

        if (mNativeText != null) {
            mNativeText.dispose();
            mNativeText = null;
        }

        // Create text
        mNativeText = new Text.TextBuilder().viroContext(mViroContext)
                .textString(mText)
                .fontFamilyName(mFontFamilyName)
                .fontSize(mSize)
                .fontStyle(mFontStyle)
                .fontWeight(mFontWeight)
                .color(mColor)
                .width(mWidth).height(mHeight)
                .horizontalAlignment(getHorizontalAlignmentEnum(mHorizontalAlignment))
                .verticalAlignment(getVerticalAlignmentEnum(mVerticalAlignment))
                .lineBreakMode(getLineBreakModeEnum(mTextLineBreakMode))
                .clipMode(getTextClipModeEnum(mTextClipMode))
                .maxLines(mMaxLines).build();

        // Add geometry
        getNodeJni().setGeometry(mNativeText);
    }

    @Override
    public void setViroContext(ViroContext context) {
        super.setViroContext(context);
        updateLabel();
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if (mNeedsUpdate) {
            mNeedsUpdate = false;
            updateLabel();
        }
    }

    private Text.FontStyle getFontStyleEnum(String strName) {
        if (strName.equalsIgnoreCase("Normal")) {
            return Text.FontStyle.Normal;
        } else if (strName.equalsIgnoreCase("Italic")) {
            return Text.FontStyle.Italic;
        } else {
            return Text.FontStyle.Normal;
        }
    }

    private Text.FontWeight getFontWeightEnum(String strName) {
        if (strName.equalsIgnoreCase("Normal")) {
            return Text.FontWeight.Regular;
        } else if (strName.equalsIgnoreCase("Bold")) {
            return Text.FontWeight.Bold;
        } else if (strName.equalsIgnoreCase("100")) {
            return Text.FontWeight.UltraLight;
        } else if (strName.equalsIgnoreCase("200")) {
            return Text.FontWeight.Thin;
        } else if (strName.equalsIgnoreCase("300")) {
            return Text.FontWeight.Light;
        } else if (strName.equalsIgnoreCase("400")) {
            return Text.FontWeight.Regular;
        } else if (strName.equalsIgnoreCase("500")) {
            return Text.FontWeight.Medium;
        } else if (strName.equalsIgnoreCase("600")) {
            return Text.FontWeight.Semibold;
        } else if (strName.equalsIgnoreCase("700")) {
            return Text.FontWeight.Bold;
        } else if (strName.equalsIgnoreCase("800")) {
            return Text.FontWeight.Heavy;
        } else if (strName.equalsIgnoreCase("900")) {
            return Text.FontWeight.ExtraBlack;
        } else {
            return Text.FontWeight.Regular;
        }
    }

    private Text.HorizontalAlignment getHorizontalAlignmentEnum(String strName) {
        // Depending on string, return the right enum
        if (strName.equalsIgnoreCase("Right")) {
            return Text.HorizontalAlignment.RIGHT;
        }
        else if (strName.equalsIgnoreCase("Center")) {
            return Text.HorizontalAlignment.CENTER;
        }
        else {
            // Default to left alignment
            return Text.HorizontalAlignment.LEFT;
        }
    }

    private Text.VerticalAlignment getVerticalAlignmentEnum(String strName) {
        // Depending on string, return the right enum
        if (strName.equalsIgnoreCase("Bottom")) {
            return Text.VerticalAlignment.BOTTOM;
        } else if (strName.equalsIgnoreCase("Center")) {
            return Text.VerticalAlignment.CENTER;
        } else {
            // Default to Top alignment
            return Text.VerticalAlignment.TOP;
        }
    }

    private Text.LineBreakMode getLineBreakModeEnum(String strName) {
        // Depending on string, return the right enum
        if (strName.equalsIgnoreCase("WordWrap")) {
            return Text.LineBreakMode.WORD_WRAP;
        } else if (strName.equalsIgnoreCase("CharWrap")) {
            return Text.LineBreakMode.CHAR_WRAP;
        } else if (strName.equalsIgnoreCase("Justify")) {
            return Text.LineBreakMode.JUSTIFY;
        } else {
            // Default to none
            return Text.LineBreakMode.NONE;
        }
    }

    private Text.ClipMode getTextClipModeEnum(String strName) {
        // Depending on string, return the right enum
        if (strName.equalsIgnoreCase("ClipToBounds")) {
            return Text.ClipMode.CLIP_TO_BOUNDS;
        } else {
            return Text.ClipMode.NONE;
        }
    }

}
