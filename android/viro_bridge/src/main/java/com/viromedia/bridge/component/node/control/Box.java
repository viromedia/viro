/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.util.Log;
import android.view.View;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.BoxJni;
import com.viromedia.bridge.component.Component;

public class Box extends Control {
    private BoxJni mNativeBox;
    private float mWidth = 1.0f;
    private float mHeight = 1.0f;
    private float mLength = 1.0f;

    public Box(ReactApplicationContext reactContext) {
        super(reactContext);
        createNativeBox();
    }

    public void setWidth(float width) {
        if (width < 0) {
            throw new IllegalArgumentException("Width of a box cannot be smaller than 0");
        }
        mWidth = width;
        createNativeBox();
    }

    public void setHeight(float height) {
        if (height < 0) {
            throw new IllegalArgumentException("Height of a box cannot be smaller than 0");
        }
        mHeight = height;
        createNativeBox();
    }

    public void setLength(float length) {
        if (length < 0) {
            throw new IllegalArgumentException("Length of a box cannot be smaller than 0");
        }
        mLength = length;
        createNativeBox();
    }

    /**
     * Because we can't modify the width/height/length of a box, we need to
     * create a new box, set it on the node, and then delete the old box for
     * now.
     */
    private void createNativeBox() {
        BoxJni oldBox = mNativeBox;
        mNativeBox = new BoxJni(mWidth, mHeight, mLength);
        setGeometry(mNativeBox);
        if (oldBox != null) {
            oldBox.destroy();
        }
    }
}