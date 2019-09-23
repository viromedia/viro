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

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReactContext;
import com.viromedia.bridge.component.node.VRTNode;

/**
 * Control is inherited by all Viro UI controls that
 * contains a native Node {@link com.viro.core.Node}.
 */
public class VRTControl extends VRTNode {

    public VRTControl(ReactContext reactContext) {
        this(reactContext.getBaseContext(), null, -1, -1, reactContext);
    }

    public VRTControl(Context context, AttributeSet attrs, int defStyleAttr,
                      int defStyleRes, ReactContext reactContext) {
        super(context, attrs, defStyleAttr, defStyleRes, reactContext);
    }

    @Override
    public void addView(View child, int index) {
        throw new IllegalStateException("Can't add child. Controls should not have children views.");
    }

    @Override
    public void removeView(View child) {
        throw new IllegalStateException("Can't remove child. Controls should not have children views.");
    }
}
