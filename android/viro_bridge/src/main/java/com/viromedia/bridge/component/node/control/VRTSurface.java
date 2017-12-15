/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.core.Material;
import com.viro.core.Surface;

import java.util.List;

public class VRTSurface extends VRTControl {
    private Surface mNativeSurface;
    private float mWidth = 1;
    private float mHeight = 1;
    private boolean mGeometryNeedsUpdate = false;
    private boolean mARShadowReceiver = false;

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

    public void setARShadowReceiver(boolean arShadowReceiver) {
        mARShadowReceiver = arShadowReceiver;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        updateSurface();
    }

    public void updateSurface() {
        if (mNativeSurface == null) {
            mNativeSurface = new Surface(mWidth, mHeight, 0, 0, 1, 1);
        }
        else if (mGeometryNeedsUpdate) {
            // make sure we release the old surface before we let it go.
            mNativeSurface.dispose();
            mNativeSurface = new Surface(mWidth, mHeight, 0, 0, 1, 1);
        }
        setGeometry(mNativeSurface);
        applyMaterials();
    }

    @Override
    public void setMaterials(List<Material> materials) {
        if (materials != null) {
            for (Material material : materials) {
                material.setShadowMode(mARShadowReceiver ? Material.ShadowMode.TRANSPARENT : Material.ShadowMode.NORMAL);
            }
            super.setMaterials(materials);
        }
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mNativeSurface != null) {
            mNativeSurface.dispose();
            mNativeSurface = null;
        }
    }
}
