/* Copyright © 2017 Viro Media. All rights reserved. */
package com.viromedia.bridge.utility;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.viro.renderer.ARHitTestResult;



public class ARHitTestResultUtil {

    public static WritableMap mapFromARHitTestResult(ARHitTestResult result) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putString("type", result.getType());
        WritableMap transformMap = Arguments.createMap();
        transformMap.putArray("position", ARHitTestResultUtil.arrayFromFloatArray(result.getPosition()));
        transformMap.putArray("scale", ARHitTestResultUtil.arrayFromFloatArray(result.getScale()));
        transformMap.putArray("rotation", ARHitTestResultUtil.arrayFromFloatArray(result.getRotation()));
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
