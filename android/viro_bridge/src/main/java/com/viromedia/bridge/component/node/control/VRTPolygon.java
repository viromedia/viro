/**
 * Copyright © 2018 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.core.Material;
import com.viro.core.Polygon;
import com.viro.core.Vector;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class VRTPolygon extends VRTControl {
    private Polygon mNativePolygon;
    private float mU0 = 0;
    private float mV0 = 0;
    private float mU1 = 1;
    private float mV1 = 1;
    private boolean mGeometryNeedsUpdate = false;
    private boolean mARShadowReceiver = false;
    private List<Vector> mVertices = new ArrayList<>();

    public VRTPolygon(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    public void setUVCoordinates(float u0, float v0, float u1, float v1) {
        mU0 = u0;
        mV0 = v0;
        mU1 = u1;
        mV1 = v1;
        mGeometryNeedsUpdate = true;
    }

    public void setVertices(List<Vector> coords){
        mVertices = coords;
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
        if (mGeometryNeedsUpdate && mNativePolygon != null){
            mNativePolygon.dispose();
            mNativePolygon = null;
        }

        if (mNativePolygon == null) {
            mNativePolygon = new Polygon(mVertices, mU0, mV0, mU1, mV1);
            setGeometry(mNativePolygon);
        }

        applyMaterials();
    }

    @Override
    public void setMaterials(List<Material> materials) {
        /*
         Override to set the shadow mode in the materials.
         */
        if (materials != null) {
            for (Material material : materials) {
                material.setShadowMode(mARShadowReceiver ? Material.ShadowMode.TRANSPARENT : Material.ShadowMode.NORMAL);
            }
            super.setMaterials(materials);
        }
        /*
         If no material was assigned to this surface, then set a default material with the correct
         shadow mode.
         */
        else if (mNativePolygon.getMaterials() == null) {
            Material material = new Material();
            material.setShadowMode(mARShadowReceiver ? Material.ShadowMode.TRANSPARENT : Material.ShadowMode.NORMAL);
            mNativePolygon.setMaterials(Arrays.asList(material));
        }
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mNativePolygon != null) {
            mNativePolygon.dispose();
            mNativePolygon = null;
        }
    }
}
