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


import com.facebook.react.bridge.ReactContext;
import com.viro.core.Box;

public class VRTBox extends VRTControl {
    private Box mNativeBox;

    public VRTBox(ReactContext reactContext) {
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