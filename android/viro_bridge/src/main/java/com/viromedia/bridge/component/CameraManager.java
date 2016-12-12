/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;

public class CameraManager extends ViroViewGroupManager<Camera> {

    public CameraManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTCamera";
    }

    @Override
    protected Camera createViewInstance(ThemedReactContext reactContext) {
        return new Camera(reactContext);
    }

    @ReactProp(name = "position")
    public void setPosition(Camera camera, ReadableArray position) {
        float[] posArray = {(float) position.getDouble(0), (float) position.getDouble(1),
                (float) position.getDouble(2)};
        camera.setPosition(posArray);
    }

}
