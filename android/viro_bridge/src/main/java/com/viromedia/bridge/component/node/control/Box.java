/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.BoxJni;

public class Box extends Control {
    private BoxJni mNativeBox;

    public Box(ReactApplicationContext reactContext) {
        super(reactContext);
        mNativeBox = new BoxJni(1, 1, 1);
        setGeometry(mNativeBox);
    }

    @Override
    public void onTearDown(){
        if (mNativeBox != null){
            mNativeBox.destroy();
        }
        super.onTearDown();
    }

    public void setWidth(float width) {
        if (width < 0) {
            throw new IllegalArgumentException("Width of a box cannot be smaller than 0");
        }
        mNativeBox.setWidth(width);
    }

    public void setHeight(float height) {
        if (height < 0) {
            throw new IllegalArgumentException("Height of a box cannot be smaller than 0");
        }
        mNativeBox.setHeight(height);
    }

    public void setLength(float length) {
        if (length < 0) {
            throw new IllegalArgumentException("Length of a box cannot be smaller than 0");
        }
        mNativeBox.setLength(length);
    }
}