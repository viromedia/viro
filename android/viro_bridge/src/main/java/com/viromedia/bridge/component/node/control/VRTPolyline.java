/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.viro.renderer.jni.Polyline;

public class VRTPolyline extends VRTControl {
    private Polyline mNativeLine;
    private boolean mDidSetGeometry = false;
    private float[][] mPoints;
    private float mThickness = 0.1f; // default thickness of line is 0.1f

    public VRTPolyline(ReactApplicationContext reactContext)  {
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
        if (points.size() == 0) {
            throw new IllegalArgumentException("Polyline should consist of at least 1 point");
        }
        float[][] pointsArray = new float[points.size()][2];
        for (int i = 0; i < points.size(); i++) {
            ReadableArray point = points.getArray(i);
            if (point.size() < 2) {
                throw new IllegalArgumentException("Polyline points should have at least 2 coordinates");
            }
            for (int j = 0; j < 2; j++) {
                pointsArray[i][j] = (float) point.getDouble(j);
            }
        }
        mPoints = pointsArray;
        mDidSetGeometry = false;
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

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if (!mDidSetGeometry && mPoints != null) {
            if (mNativeLine != null) {
                mNativeLine.dispose();
            }

            mNativeLine = new Polyline(mPoints, mThickness);
            setGeometry(mNativeLine);
            mDidSetGeometry = true;
        }
    }

}
