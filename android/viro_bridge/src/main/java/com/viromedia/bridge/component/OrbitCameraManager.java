/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;

public class OrbitCameraManager extends ViroViewGroupManager<OrbitCamera> {
    public OrbitCameraManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTOrbitCamera";
    }

    @Override
    protected OrbitCamera createViewInstance(ThemedReactContext reactContext) {
        return new OrbitCamera(reactContext);
    }

    @ReactProp(name = "position")
    public void setPosition(Camera camera, ReadableArray position) {
        float[] posArray = {(float) position.getDouble(0), (float) position.getDouble(1),
                (float) position.getDouble(2)};
        camera.setPosition(posArray);
    }

    @ReactProp(name = "focalPoint")
    public void setFocalPoint(OrbitCamera camera, ReadableArray focalPoint) {
        float[] focalPointArray = {(float) focalPoint.getDouble(0), (float) focalPoint.getDouble(1),
                (float) focalPoint.getDouble(2)};
        camera.setFocalPoint(focalPointArray);
    }
}
