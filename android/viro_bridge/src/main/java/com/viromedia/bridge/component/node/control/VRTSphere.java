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
import com.viro.core.Sphere;

public class VRTSphere extends VRTControl {
    protected static final int DEFAULT_WIDTH_SEGMENT = 20;
    protected static final int DEFAULT_HEIGHT_SEGMENT = 20;
    protected static final float DEFAULT_RADIUS = 1.0f;
    protected static final boolean DEFAULT_FACES_OUTWARD = true;

    private Sphere mNativeSphere;
    private boolean mFacesOutward = DEFAULT_FACES_OUTWARD;
    private int mWidthSegmentCount = DEFAULT_WIDTH_SEGMENT;
    private int mHeightSegmentCount = DEFAULT_HEIGHT_SEGMENT;
    private float mRadius = DEFAULT_RADIUS;

    public VRTSphere(ReactContext reactContext) {
        super(reactContext);
    }

    public void setFacesOutward(boolean facesOutward) {
        mFacesOutward = facesOutward;
    }

    public void setWidthSegmentCount(int widthSegmentCount) {
        if (widthSegmentCount < 0) {
            throw new IllegalArgumentException("Width segment of a sphere cannot be smaller than 0");
        }
        mWidthSegmentCount = widthSegmentCount;
    }

    public void setHeightSegmentCount(int heightSegmentCount) {
        if (heightSegmentCount < 0) {
            throw new IllegalArgumentException("Height segment of a sphere cannot be smaller than 0");
        }
        mHeightSegmentCount = heightSegmentCount;
    }

    public void setRadius(float radius) {
        if (radius < 0) {
            throw new IllegalArgumentException("Radius of a sphere cannot be smaller than 0");
        }
        mRadius = radius;
    }

    @Override
    protected void onPropsSet(){
        if (mNativeSphere != null){
            mNativeSphere.dispose();
        }

        mNativeSphere = new Sphere(mRadius, mWidthSegmentCount, mHeightSegmentCount, mFacesOutward);
        setGeometry(mNativeSphere);
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mNativeSphere != null) {
            mNativeSphere.dispose();
            mNativeSphere = null;
        }
    }
}