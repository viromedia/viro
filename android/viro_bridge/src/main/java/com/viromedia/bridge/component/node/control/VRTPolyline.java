//  Copyright © 2017 Viro Media. All rights reserved.
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
import com.facebook.react.bridge.ReadableArray;
import com.viro.core.Polyline;

public class VRTPolyline extends VRTControl {
    private Polyline mNativeLine;
    private boolean mDidSetGeometry = false;
    private float[][] mPoints;
    private float mThickness = 0.1f; // default thickness of line is 0.1f
    private boolean mClosed = false;
    private ReadableArray mRawPoints;

    public VRTPolyline(ReactContext reactContext)  {
        super(reactContext);
    }

    @Override
    public void onTearDown() {
        if (mNativeLine != null) {
            mNativeLine.dispose();
        }
        super.onTearDown();
    }

    public void setPoints(ReadableArray points) {
        mRawPoints = points;
        mDidSetGeometry = false;
    }

    private float[][] processPoints() {
        if (mRawPoints.size() == 0) {
            throw new IllegalArgumentException("Polyline should consist of at least 1 point");
        }

        int pointSize = mRawPoints.size();
        if (mClosed) {
            pointSize+=1;
        }

        float[][] pointsArray = new float[pointSize][3];
        for (int i = 0; i < mRawPoints.size(); i++) {
            ReadableArray point = mRawPoints.getArray(i);
            if (point.size() < 3) {
                throw new IllegalArgumentException("Polyline points should have at least 3 coordinates");
            }
            for (int j = 0; j < 3; j++) {
                pointsArray[i][j] = (float) point.getDouble(j);
            }
        }

        // Add a closing point to the end if closed prop set.
        if (mClosed) {
            ReadableArray point = mRawPoints.getArray(0);
            pointsArray[mRawPoints.size()][0] = (float)point.getDouble(0);
            pointsArray[mRawPoints.size()][1] = (float)point.getDouble(1);
            pointsArray[mRawPoints.size()][2] = (float)point.getDouble(2);
        }

        return pointsArray;
    }

    public void setThickness(float thickness) {
        if (thickness < 0) {
            throw new IllegalArgumentException("Polyline thickness must be >= 0");
        }
        mThickness = thickness;
        if (mDidSetGeometry) {
            mNativeLine.setThickness(thickness);
        }
    }

    public void setClosed(boolean closed) {
        mClosed = closed;
        mDidSetGeometry = false;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if (!mDidSetGeometry && mRawPoints != null) {
            mPoints = processPoints();
            if (mNativeLine != null) {
                mNativeLine.dispose();
            }

            mNativeLine = new Polyline(mPoints, mThickness);
            setGeometry(mNativeLine);
            mDidSetGeometry = true;
        }
    }

}
