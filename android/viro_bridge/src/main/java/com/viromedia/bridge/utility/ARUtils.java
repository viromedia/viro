/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.utility;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.WritableMap;
import com.viro.renderer.ARAnchor;

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
}
