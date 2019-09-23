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

package com.viromedia.bridge.component.node;

import com.viro.core.Geometry;
import com.viro.core.Material;
import com.viro.core.Quad;

import java.util.List;

import android.graphics.Color;

import com.facebook.react.bridge.ReactContext;

public class VRTFlexView extends VRTNode {
    private static final String DIFFUSE_COLOR_NAME  = "diffuseColor";

    // in accordance with how React Views behave. If you don't set a height/width on a FlexView then
    // it's width/height is 0.
    private float mWidth = 0;
    private float mHeight = 0;
    private int mBackgroundColor = Color.TRANSPARENT;

    private final Material mDefaultMaterial;
    private Quad mNativeQuad;
    private Material mNativeColorMaterial;

    public VRTFlexView(ReactContext context) {
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

        if (mNativeQuad != null) {
            mNativeQuad.dispose();
            mNativeQuad = null;
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

        if (mNativeQuad != null) {
            mNativeQuad.dispose();
        }

        mNativeQuad = new Quad(mWidth, mHeight, 0, 0, 1, 1);
        setBackgroundOnSurface();
        setGeometry(mNativeQuad);
    }

    /**
     * Helper function which will set a material on the mNativeQuad or remove material if neither
     * color nor materials are given.
     */
    private void setBackgroundOnSurface() {
        if (isTornDown() || mNativeQuad == null) {
            return;
        }

        if (mBackgroundColor != Color.TRANSPARENT) {
            // destroy the old color material
            if (mNativeColorMaterial != null) {
                mNativeColorMaterial.dispose();
            }
            mNativeColorMaterial = new Material();
            mNativeColorMaterial.setDiffuseColor(mBackgroundColor);
            mNativeQuad.setMaterial(mNativeColorMaterial);
        } else if (mMaterials != null && mMaterials.size() > 0) {
            // set the first material on the surface
            mNativeQuad.setMaterial(mMaterials.get(0));
        } else {
            if (mNativeColorMaterial != null) {
                mNativeColorMaterial.dispose();
                mNativeColorMaterial = null;
            }
            // set the default (transparent) material if no color or material is given
            mNativeQuad.setMaterial(mDefaultMaterial);
        }
    }

}