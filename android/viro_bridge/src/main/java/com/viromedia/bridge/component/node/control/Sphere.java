/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.SphereJni;

public class Sphere extends Control {
    protected static final int DEFAULT_WIDTH_SEGMENT = 20;
    protected static final int DEFAULT_HEIGHT_SEGMENT = 20;
    protected static final float DEFAULT_RADIUS = 1.0f;
    protected static final boolean DEFAULT_FACES_OUTWARD = true;

    private SphereJni mNativeSphere;
    private boolean mFacesOutward = DEFAULT_FACES_OUTWARD;
    private int mWidthSegmentCount = DEFAULT_WIDTH_SEGMENT;
    private int mHeightSegmentCount = DEFAULT_HEIGHT_SEGMENT;
    private float mRadius = DEFAULT_RADIUS;

    public Sphere(ReactApplicationContext reactContext) {
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
            mNativeSphere.delete();
        }

        mNativeSphere = new SphereJni(mRadius, mWidthSegmentCount, mHeightSegmentCount, mFacesOutward);
        setGeometry(mNativeSphere);
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mNativeSphere != null) {
            mNativeSphere.delete();
            mNativeSphere = null;
        }
    }
}