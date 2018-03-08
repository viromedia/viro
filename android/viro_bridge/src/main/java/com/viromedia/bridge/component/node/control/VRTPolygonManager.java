/**
 * Copyright © 2018 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.util.Log;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viro.core.Vector;
import com.viromedia.bridge.component.node.VRTNodeManager;
import com.viromedia.bridge.utility.ViroEvents;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class VRTPolygonManager extends VRTNodeManager<VRTPolygon> {
    public VRTPolygonManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTPolygon";
    }

    @Override
    protected VRTPolygon createViewInstance(ThemedReactContext reactContext) {
        return new VRTPolygon(getContext());
    }

    @ReactProp(name = "vertices")
    public void setVertices(VRTPolygon view, ReadableArray vertices) {
        if (vertices == null || vertices.size() == 0) {
            throw new IllegalArgumentException("[ViroPolygon] Invalid Polygon vertex boundary list provided!");
        }

        List<Vector> vecVertices = new ArrayList<Vector>();
        for (int i = 0; i < vertices.size(); i ++) {
            ReadableArray vecArray = vertices.getArray(i);
            if (vecArray == null || vecArray.size() < 2) {
                throw new IllegalArgumentException("[ViroPolygon] Invalid Polygon vertex boundary list provided!");
            }

            if (vertices.size() > 2){
                Log.w("Viro","[ViroPolygon] Polygon only supports xy coordinates! " +
                        "But a set of 3 points had been provided!");
            }

            vecVertices.add(new Vector(vecArray.getDouble(0), vecArray.getDouble(1),0));
        }
        view.setVertices(vecVertices);
    }

    @ReactProp(name = "uvCoordinates")
    public void setUVCoordinates(VRTPolygon view, ReadableArray coordinates) {
        float u0 = 0; float v0 = 0; float u1 = 1; float v1 = 1;

        if (coordinates == null) {
            // do-nothing
        } else if (coordinates.size() != 4) {
            throw new IllegalArgumentException("[ViroPolygon] Expected 4 uv coordinates, got " + coordinates.size());
        } else { // not null && has 4 elements
            u0 = (float) coordinates.getDouble(0);
            v0 = (float) coordinates.getDouble(1);
            u1 = (float) coordinates.getDouble(2);
            v1 = (float) coordinates.getDouble(3);
        }

        view.setUVCoordinates(u0, v0, u1, v1);
    }

    @ReactProp(name = "lightReceivingBitMask", defaultInt = 1)
    public void setLightReceivingBitMask(VRTPolygon view, int bitMask) {view.setLightReceivingBitMask(bitMask); }

    @ReactProp(name = "shadowCastingBitMask", defaultInt = 1)
    public void setShadowCastingBitMask(VRTPolygon view, int bitMask) {view.setShadowCastingBitMask(bitMask); }

    @ReactProp(name = "arShadowReceiver", defaultBoolean = false)
    public void setARShadowReceiver(VRTPolygon view, boolean arShadowReceiver) { view.setARShadowReceiver(arShadowReceiver); }

    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START),
                ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH));
    }
}
