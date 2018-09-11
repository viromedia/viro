/**
 * Copyright © 2018 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.util.Log;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viro.core.Vector;

import java.util.ArrayList;
import java.util.List;

public class VRTGeometryManager extends VRTControlManager<VRTGeometry> {

    public VRTGeometryManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTGeometry";
    }

    @Override
    protected VRTGeometry createViewInstance(ThemedReactContext reactContext) {
        return new VRTGeometry(reactContext);
    }

    private List<Vector> convertVectorArray(ReadableArray array, int componentsPerVertex, String type) {
        List<Vector> result = new ArrayList<Vector>();
        for (int i = 0; i < array.size(); i ++) {
            ReadableArray vecArray = array.getArray(i);
            if (vecArray == null) {
                throw new IllegalArgumentException("[ViroGeometry] geometry requires " + componentsPerVertex + " coordinates per vertex for type " + type + " but null vertex was provided!");
            }
            if (vecArray.size() < componentsPerVertex) {
                throw new IllegalArgumentException("[ViroGeometry] geometry requires " + componentsPerVertex + " coordinates per vertex for type " + type + " but " +
                        " vertex with " + vecArray.size() + " points was provided!");
            }
            if (vecArray.size() > componentsPerVertex) {
                Log.w("Viro","[ViroGeometry] geometry only supports " + componentsPerVertex + " coordinates per vertex for type " + type + " but " +
                        " vertex with " + vecArray.size() + " points was provided!");

            }

            Vector v = new Vector();
            if (componentsPerVertex > 0) {
                v.x = (float) vecArray.getDouble(0);
            }
            if (componentsPerVertex > 1) {
                v.y = (float) vecArray.getDouble(1);
            }
            if (componentsPerVertex > 2) {
                v.z = (float) vecArray.getDouble(2);
            }
            result.add(v);
        }
        return result;
    }

    @ReactProp(name = "vertices")
    public void setVertices(VRTGeometry view, ReadableArray vertices) {
        if (vertices == null || vertices.size() == 0) {
            throw new IllegalArgumentException("[ViroGeometry] Invalid Geometry vertex boundary list provided!");
        }
        view.setVertices(convertVectorArray(vertices, 3, "vertices"));
    }

    @ReactProp(name = "texcoords")
    public void setTexcoords(VRTGeometry view, ReadableArray texcoords) {
        view.setTexcoords(convertVectorArray(texcoords, 2, "texcoords"));
    }

    @ReactProp(name = "normals")
    public void setNormals(VRTGeometry view, ReadableArray normals) {
        view.setNormals(convertVectorArray(normals, 3, "normals"));
    }

    @ReactProp(name = "triangleIndices")
    public void setTriangleIndices(VRTGeometry view, ReadableArray triangleIndicesArray) {
        List<List<Integer>> triangleIndices = new ArrayList<>();

        for (int i = 0; i < triangleIndicesArray.size(); i++) {
            ReadableArray submeshArray = triangleIndicesArray.getArray(i);
            if (submeshArray == null) {
                continue;
            }

            List<Integer> submesh = new ArrayList<>();
            for (int j = 0; j < submeshArray.size(); j++) {
                submesh.add(submeshArray.getInt(j));
            }
            triangleIndices.add(submesh);
        }
        view.setTriangleIndices(triangleIndices);
    }
}
