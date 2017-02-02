/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.BoxJni;

public class Box extends Control {
    private BoxJni mNativeBox;
    private float mWidth = 1.0f;
    private float mHeight = 1.0f;
    private float mLength = 1.0f;

    public Box(ReactApplicationContext reactContext) {
        super(reactContext);
        createNativeBox();
    }

    @Override
    protected void onTearDown(){
        if (mNativeBox != null){
            mNativeBox.destroy();
        }
        super.onTearDown();
    }

    public void setWidth(float width) {
        if (width < 0) {
            throw new IllegalArgumentException("Width of a box cannot be smaller than 0");
        }
        mWidth = width;
    }

    public void setHeight(float height) {
        if (height < 0) {
            throw new IllegalArgumentException("Height of a box cannot be smaller than 0");
        }
        mHeight = height;
    }

    public void setLength(float length) {
        if (length < 0) {
            throw new IllegalArgumentException("Length of a box cannot be smaller than 0");
        }
        mLength = length;
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

    @Override
    protected void onPropsSet(){
        createNativeBox();
    }
}