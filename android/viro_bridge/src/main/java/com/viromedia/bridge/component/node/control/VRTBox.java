/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.core.Box;

public class VRTBox extends VRTControl {
    private Box mNativeBox;

    public VRTBox(ReactApplicationContext reactContext) {
        super(reactContext);
        mNativeBox = new Box(1, 1, 1);
        setGeometry(mNativeBox);
    }

    @Override
    public void onTearDown(){
        if (mNativeBox != null){
            mNativeBox.dispose();
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