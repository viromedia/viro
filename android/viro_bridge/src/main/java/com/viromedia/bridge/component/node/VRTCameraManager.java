/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.VRTCamera;
import com.viromedia.bridge.component.node.VRTNodeManager;

public class VRTCameraManager extends VRTNodeManager<VRTCamera> {

    public VRTCameraManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTCamera";
    }

    @Override
    protected VRTCamera createViewInstance(ThemedReactContext reactContext) {
        return new VRTCamera(reactContext);
    }

    @ReactProp(name = "position")
    public void setPosition(VRTCamera camera, ReadableArray position) {
        float[] posArray = {(float) position.getDouble(0), (float) position.getDouble(1),
                (float) position.getDouble(2)};
        camera.setPosition(posArray);
    }

    @ReactProp(name = "fieldOfView", defaultFloat = VRTCamera.DEFAULT_FOV)
    public void setFieldOfView(VRTCamera camera, float fov) {
        camera.setFieldOfView(fov);
    }

}
