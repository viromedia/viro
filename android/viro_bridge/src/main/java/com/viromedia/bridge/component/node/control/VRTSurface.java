/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.Surface;

public class VRTSurface extends VRTControl {
    private Surface mNativeSurface;
    private float mWidth = 1;
    private float mHeight = 1;
    private boolean mGeometryNeedsUpdate = false;

    public VRTSurface(ReactApplicationContext reactContext) {
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
        updateSurface();
    }

    public void updateSurface() {
        if (mNativeSurface == null) {
            mNativeSurface = new Surface(mWidth, mHeight, 0, 0, 1, 1);
        } else if (mGeometryNeedsUpdate) {
            // make sure we release the old surface before we let it go.
            mNativeSurface.destroy();
            mNativeSurface = new Surface(mWidth, mHeight, 0, 0, 1, 1);
        }
        setGeometry(mNativeSurface);
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mNativeSurface != null) {
            mNativeSurface.destroy();
            mNativeSurface = null;
        }
    }
}
