//  Copyright © 2017 Viro Media. All rights reserved.
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

package com.viromedia.bridge.utility;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.viro.core.ARAnchor;
import com.viro.core.ARHitTestResult;
import com.viro.core.ARImageAnchor;
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
        } else if (anchor.getType() == ARAnchor.Type.IMAGE) {
            ARImageAnchor imageAnchor = (ARImageAnchor)anchor;
            returnMap.putString("trackingMethod", imageAnchor.getTrackingMethod().getStringValue());
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
