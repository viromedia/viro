/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;

public class VRTOrbitCameraManager extends VRTNodeManager<VRTOrbitCamera> {
    public VRTOrbitCameraManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTOrbitCamera";
    }

    @Override
    protected VRTOrbitCamera createViewInstance(ThemedReactContext reactContext) {
        return new VRTOrbitCamera(reactContext);
    }

    @ReactProp(name = "position")
    public void setPosition(VRTCamera camera, ReadableArray position) {
        float[] posArray = {(float) position.getDouble(0), (float) position.getDouble(1),
                (float) position.getDouble(2)};
        camera.setPosition(posArray);
    }

    @ReactProp(name = "focalPoint")
    public void setFocalPoint(VRTOrbitCamera camera, ReadableArray focalPoint) {
        float[] focalPointArray = {(float) focalPoint.getDouble(0), (float) focalPoint.getDouble(1),
                (float) focalPoint.getDouble(2)};
        camera.setFocalPoint(focalPointArray);
    }

    @ReactProp(name = "fieldOfView", defaultFloat = VRTCamera.DEFAULT_FOV)
    public void setFieldOfView(VRTCamera camera, float fov) {
        camera.setFieldOfView(fov);
    }

}
