/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.viro.core.Geometry;
import com.viro.core.Material;
import com.viro.core.Surface;

import java.util.List;

import android.graphics.Color;

import com.facebook.react.bridge.ReactApplicationContext;

public class VRTFlexView extends VRTNode {
    private static final String DIFFUSE_COLOR_NAME  = "diffuseColor";

    // in accordance with how React Views behave. If you don't set a height/width on a FlexView then
    // it's width/height is 0.
    private float mWidth = 0;
    private float mHeight = 0;
    private int mBackgroundColor = Color.TRANSPARENT;

    private final Material mDefaultMaterial;
    private Surface mNativeSurface;
    private Material mNativeColorMaterial;

    public VRTFlexView(ReactApplicationContext context) {
        super(context);
        mDefaultMaterial = new Material();
        mDefaultMaterial.setDiffuseColor(Color.TRANSPARENT);
        getNodeJni().setHierarchicalRendering(true);
    }

    public void setWidth(float width) {
        mWidth = width;
    }

    public void setHeight(float height) {
        mHeight = height;
    }

    public void setBackgroundColor(long color) {
        mBackgroundColor = (int) color;
        setBackgroundOnSurface();
    }

    /**
     * Override setMaterials() of Node because we don't necessarily always want to use the given
     * Materials (ie. in the case a backgroundColor is also provided).
     */
    @Override
    protected void setMaterials(List<Material> materials) {
        mMaterials = materials;
        setBackgroundOnSurface();
    }

    /**
     * Override setGeometry because we don't want to always use the Materials set on a FlexView.
     */
    @Override
    protected void setGeometry(Geometry geometry) {
        if (getNodeJni() != null) {
            getNodeJni().setGeometry(geometry);
        }
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        // Do nothing for "onPropsSet" on FlexViews because they are only ready AFTER they've
        // been laid out (see attemptReacalcLayout()).
    }

    @Override
    public void onTearDown() {
        if (isTornDown()) {
            return;
        }
        super.onTearDown();

        if (mNativeSurface != null) {
            mNativeSurface.dispose();
            mNativeSurface = null;
        }

        if (mNativeColorMaterial != null) {
            mNativeColorMaterial.dispose();
            mNativeColorMaterial = null;
        }

        mDefaultMaterial.dispose();
    }

    @Override
    protected void attemptRecalcLayout() {
        super.attemptRecalcLayout();

        // create the surface once we're sure that we've been laid out
        createSurface();
    }

    @Override
    protected float[] get2DPosition() {
        if (isRootFlexView()) {
            float[] arr = {getPivotX(), getPivotY()};
            return arr;
        } else {
            return super.get2DPosition();
        }
    }

    private boolean isRootFlexView() {
        // Since Node is the "parent" class of most components, we want to check if the parent
        // is exactly a Node (not a child of Node).
        return getParent() instanceof VRTScene || VRTNode.class.equals(getParent().getClass());
    }

    private void createSurface() {
        if (isTornDown()) {
            return;
        }

        if (mNativeSurface != null) {
            mNativeSurface.dispose();
        }

        mNativeSurface = new Surface(mWidth, mHeight, 0, 0, 1, 1);
        setBackgroundOnSurface();
        setGeometry(mNativeSurface);
    }

    /**
     * Helper function which will set a material on the mNativeSurface or remove material if neither
     * color nor materials are given.
     */
    private void setBackgroundOnSurface() {
        if (isTornDown() || mNativeSurface == null) {
            return;
        }

        if (mBackgroundColor != Color.TRANSPARENT) {
            // destroy the old color material
            if (mNativeColorMaterial != null) {
                mNativeColorMaterial.dispose();
            }
            mNativeColorMaterial = new Material();
            mNativeColorMaterial.setDiffuseColor(mBackgroundColor);
            mNativeSurface.setMaterial(mNativeColorMaterial);
        } else if (mMaterials != null && mMaterials.size() > 0) {
            // set the first material on the surface
            mNativeSurface.setMaterial(mMaterials.get(0));
        } else {
            if (mNativeColorMaterial != null) {
                mNativeColorMaterial.dispose();
                mNativeColorMaterial = null;
            }
            // set the default (transparent) material if no color or material is given
            mNativeSurface.setMaterial(mDefaultMaterial);
        }
    }

}