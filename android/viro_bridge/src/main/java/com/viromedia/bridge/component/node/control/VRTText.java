/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.graphics.Color;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.core.ViroContext;
import com.viro.core.Text;

public class VRTText extends VRTControl {
    static final String DEFAULT_FONT = "Roboto";
    static final float DEFAULT_WIDTH = 1f;
    static final float DEFAULT_HEIGHT = 1f;
    static final long DEFAULT_COLOR = Color.WHITE;
    static final int DEFAULT_MAX_LINES = 0;
    static final int DEFAULT_FONT_SIZE = 14;

    private Text mNativeText = null;
    private long mColor = DEFAULT_COLOR;
    private int mMaxLines = DEFAULT_MAX_LINES;
    private float mWidth = DEFAULT_WIDTH;
    private float mHeight = DEFAULT_HEIGHT;
    private String mText;
    private String mFontFamilyName = DEFAULT_FONT;
    private int mSize = DEFAULT_FONT_SIZE;
    private String mHorizontalAlignment = "Left";
    private String mVerticalAlignment = "Top";
    private String mTextLineBreakMode = "WordWrap";
    private String mTextClipMode = "ClipToBounds";
    public VRTText(ReactApplicationContext context) {
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
    }

    public void setHeight(float height) {
        mHeight = height;
    }

    public void setColor(long color) {
        mColor = color;
    }

    public void setMaxLines(int maxLines) {
        mMaxLines = maxLines;
    }

    public void setText(String text) {
        mText = text;
    }

    public void setFontFamilyName(String fontFamilyName) {
        if (fontFamilyName == null || fontFamilyName.trim().isEmpty()) {
            return;
        }
        mFontFamilyName = fontFamilyName;
    }

    public void setSize(int size) {
        mSize = size;
    }

    public void setHorizontalAlignment(String horizontalAlignment) {
        mHorizontalAlignment = horizontalAlignment;
    }

    public void setVerticalAlignment(String verticalAlignment) {
        mVerticalAlignment = verticalAlignment;
    }

    public void setTextClipMode(String textClipMode) {
        mTextClipMode = textClipMode;
    }

    public void setTextLineBreakMode(String textLineBreakMode) {
        mTextLineBreakMode = textLineBreakMode;
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
        mNativeText = new Text(mViroContext, mText, mFontFamilyName, mSize, mColor, mWidth, mHeight,
                getHorizontalAlignmentEnum(mHorizontalAlignment),
                getVerticalAlignmentEnum(mVerticalAlignment),
                getLineBreakModeEnum(mTextLineBreakMode),
                getTextClipModeEnum(mTextClipMode), mMaxLines);

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
        updateLabel();
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
