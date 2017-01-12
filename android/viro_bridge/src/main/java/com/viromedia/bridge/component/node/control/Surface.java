/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.SurfaceJni;

public class Surface extends Control {
    private SurfaceJni mNativeSurface;
    private float mWidth = 1;
    private float mHeight = 1;
    private boolean mGeometryNeedsUpdate = false;

    public Surface(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    public void setWidth(float width) {
        mWidth = width;
        mGeometryNeedsUpdate = true;
    }

    public void setHeight(float height) {
        mHeight = height;
        mGeometryNeedsUpdate = true;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if (mNativeSurface == null) {
            mNativeSurface = new SurfaceJni(mWidth, mHeight);
        } else if (mGeometryNeedsUpdate) {
            // make sure we release the old surface before we let it go.
            mNativeSurface.destroy();
            mNativeSurface = new SurfaceJni(mWidth, mHeight);
        }
        setGeometry(mNativeSurface);
    }

}
