/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viromedia.bridge.component.node.control.Camera;

public class OrbitCamera extends Camera {
    private float[] mFocalPoint;

    public OrbitCamera(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getRotationType() {
        return "Orbit";
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
        mNativeCamera.setOrbitFocalPoint(mFocalPoint);
    }
}
