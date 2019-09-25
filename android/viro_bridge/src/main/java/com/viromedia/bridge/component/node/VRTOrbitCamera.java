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


import com.facebook.react.bridge.ReactContext;
import com.viro.core.Camera;
import com.viro.core.Vector;
import com.viromedia.bridge.component.node.VRTCamera;

public class VRTOrbitCamera extends VRTCamera {
    private float[] mFocalPoint;

    public VRTOrbitCamera(ReactContext context) {
        super(context);
    }

    @Override
    public Camera.RotationType getRotationType() {
        return Camera.RotationType.ORBIT;
    }

    public void setFocalPoint(float[] focalPoint) {
        mFocalPoint = focalPoint;
    }

    public float[] getFocalPoint() {
        return mFocalPoint;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        mNativeCamera.setOrbitFocalPoint(new Vector(mFocalPoint));
    }
}
