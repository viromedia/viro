/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.utility;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.viro.core.ARAnchor;
import com.viro.core.ARHitTestResult;
import com.viro.core.ARPlaneAnchor;
import com.viro.core.ARPointCloud;
import com.viro.core.Vector;

public class ARUtils {

    public static WritableMap mapFromARAnchor(ARAnchor anchor) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putString("anchorId", anchor.getAnchorId());
        returnMap.putArray("position", Arguments.makeNativeArray(anchor.getPosition().toArray()));
        returnMap.putArray("scale", Arguments.makeNativeArray(anchor.getScale().toArray()));
        // rotation values come as radians, we need to convert to degrees
        returnMap.putArray("rotation", arrayFromRotationArray(anchor.getRotation().toArray()));
        returnMap.putString("type", anchor.getType().getStringValue());

        if (anchor.getType() == ARAnchor.Type.PLANE) {
            ARPlaneAnchor plane = (ARPlaneAnchor) anchor;
            returnMap.putArray("center", Arguments.makeNativeArray(plane.getCenter().toArray()));
            returnMap.putDouble("width", plane.getExtent().x);
            returnMap.putDouble("height", plane.getExtent().z);
            returnMap.putString("alignment", plane.getAlignment().getStringValue());

            WritableArray polygonPointsArray = Arguments.createArray();
            for (Vector point : plane.getVertices()){
                polygonPointsArray.pushArray(arrayFromVector(point));
            }
            returnMap.putArray("vertices", polygonPointsArray);
        }
        return returnMap;
    }

    // TODO: VIRO-2170 ARHitTestResults should also use Vectors
    public static WritableMap mapFromARHitTestResult(ARHitTestResult result) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putString("type", result.getType().getStringValue());
        WritableMap transformMap = Arguments.createMap();
        transformMap.putArray("position", ARUtils.arrayFromFloatArray(result.getPosition().toArray()));
        transformMap.putArray("scale", ARUtils.arrayFromFloatArray(result.getScale().toArray()));
        // rotation values come as radians, we need to convert to degrees
        transformMap.putArray("rotation", ARUtils.arrayFromRotationArray(result.getRotation().toArray()));
        returnMap.putMap("transform", transformMap);
        return returnMap;
    }

    public static WritableMap mapFromARPointCloud(ARPointCloud pointCloud) {
        WritableArray pointsArray = Arguments.createArray();
        float[] points = pointCloud.getPoints();
        for (int i = 0; i < points.length; i+=4) {
            WritableArray point = Arguments.createArray();
            point.pushDouble(points[i]);
            point.pushDouble(points[i+1]);
            point.pushDouble(points[i+2]);
            point.pushDouble(points[i+3]);
            pointsArray.pushArray(point);
        }

        WritableArray idsArray = Arguments.createArray();
        long[] ids = pointCloud.getIds();
        for (int i = 0; i < ids.length; i++) {
            idsArray.pushInt((int)ids[i]);
        }


        WritableMap returnMap = Arguments.createMap();
        returnMap.putArray("points", pointsArray);
        returnMap.putArray("identifiers", idsArray);
        return returnMap;
    }

    /*
     Assumes there are only 3 elements in it.
     */
    private static WritableArray arrayFromFloatArray(float[] array) {
        WritableArray returnArray = Arguments.createArray();
        returnArray.pushDouble(array[0]);
        returnArray.pushDouble(array[1]);
        returnArray.pushDouble(array[2]);
        return returnArray;
    }

    /*
     Rotation from the renderer/jni (ViroCore) comes as radians, so we need to convert
     to degrees for ViroReact
     */
    private static WritableArray arrayFromRotationArray(float[] array) {
        WritableArray returnArray = Arguments.createArray();
        returnArray.pushDouble(Math.toDegrees(array[0]));
        returnArray.pushDouble(Math.toDegrees(array[1]));
        returnArray.pushDouble(Math.toDegrees(array[2]));
        return returnArray;
    }

    private static WritableArray arrayFromVector(Vector vector) {
        WritableArray returnArray = Arguments.createArray();
        returnArray.pushDouble(vector.x);
        returnArray.pushDouble(vector.y);
        returnArray.pushDouble(vector.z);
        return returnArray;
    }
}
