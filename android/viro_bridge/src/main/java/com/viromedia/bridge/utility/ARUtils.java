/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.utility;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.viro.renderer.ARAnchor;
import com.viro.renderer.ARHitTestResult;

public class ARUtils {

    public static WritableMap mapFromARAnchor(ARAnchor anchor) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putString("anchorId", anchor.getAnchorId());
        returnMap.putArray("position", Arguments.makeNativeArray(anchor.getPosition()));
        returnMap.putArray("scale", Arguments.makeNativeArray(anchor.getScale()));
        returnMap.putArray("rotation", Arguments.makeNativeArray(anchor.getRotation()));
        returnMap.putString("type", anchor.getType());

        if (anchor.getType().equals("plane")) {
            returnMap.putArray("center", Arguments.makeNativeArray(anchor.getCenter()));
            returnMap.putDouble("width", anchor.getExtent()[0]);
            returnMap.putDouble("height", anchor.getExtent()[2]);
            returnMap.putString("alignment", anchor.getAlignment());
        }
        return returnMap;
    }

    public static WritableMap mapFromARHitTestResult(ARHitTestResult result) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putString("type", result.getType());
        WritableMap transformMap = Arguments.createMap();
        transformMap.putArray("position", ARUtils.arrayFromFloatArray(result.getPosition()));
        transformMap.putArray("scale", ARUtils.arrayFromFloatArray(result.getScale()));
        transformMap.putArray("rotation", ARUtils.arrayFromFloatArray(result.getRotation()));
        returnMap.putMap("transform", transformMap);
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
}
