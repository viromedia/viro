/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.Camera;
import com.viro.renderer.jni.Vector;

public class VRTOrbitCamera extends VRTCamera {
    private float[] mFocalPoint;

    public VRTOrbitCamera(ReactApplicationContext context) {
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
