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

package com.viromedia.bridge.component.node;

import android.graphics.Color;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;

public class VRTFlexViewManager extends VRTNodeManager<VRTFlexView> {
    public VRTFlexViewManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTFlexView";
    }

    @Override
    protected VRTFlexView createViewInstance(ThemedReactContext reactContext) {
        return new VRTFlexView(reactContext);
    }

    @ReactProp(name = "width", defaultFloat = 1)
    public void setWidth(VRTFlexView view, float width) {
        view.setWidth(width);
    }

    @ReactProp(name = "height", defaultFloat = 1)
    public void setHeight(VRTFlexView view, float height) {
        view.setHeight(height);
    }

    @ReactProp(name = "backgroundColor", customType = "Color")
    public void setBackgroundColor(VRTFlexView view, Integer color) {
        if(color == null){
            view.setBackgroundColor(Color.TRANSPARENT);
        } else {
            view.setBackgroundColor((long) color);
        }
    }
}
