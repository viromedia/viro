/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.view.View;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.ARPlaneJni;
import com.viro.renderer.jni.ARSceneControllerJni;
import com.viro.renderer.jni.SceneControllerJni;
import com.viromedia.bridge.utility.ViroEvents;

public class ARScene extends Scene implements ARSceneControllerJni.ARSceneDelegate {

    private static final String AMBIENT_LIGHT_INFO_KEY = "ambientLightInfo";
    private static final String INTENSITY_KEY = "intensity";
    private static final String COLOR_TEMPERATURE_KEY = "colorTemperature";

    public ARScene(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    protected SceneControllerJni createSceneControllerJni() {
        ARSceneControllerJni sceneControllerJni = new ARSceneControllerJni(getNodeJni());
        sceneControllerJni.registerARDelegate(this);
        return sceneControllerJni;
    }

    public void addARPlane(ARPlaneJni planeJni) {
        ((ARSceneControllerJni) mNativeSceneController).addARPlane(planeJni);
    }

    public void updateARPlane(ARPlaneJni planeJni) {
        ((ARSceneControllerJni) mNativeSceneController).updateARPlane(planeJni);
    }

    public void removeARPlane(ARPlaneJni planeJni) {
        ((ARSceneControllerJni) mNativeSceneController).removeARPlane(planeJni);
    }

    // -- ARSceneDelegate Implementation --

    @Override
    public void onTrackingInitialized() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_TRACKING_INITIALIZED,
                null);
    }

    @Override
    public void onAmbientLightUpdate(float lightIntensity, float colorTemperature) {
        WritableMap lightInfoMap = Arguments.createMap();
        lightInfoMap.putDouble(INTENSITY_KEY, (double) lightIntensity);
        lightInfoMap.putDouble(COLOR_TEMPERATURE_KEY, (double) colorTemperature);

        WritableMap event = Arguments.createMap();
        event.putMap(AMBIENT_LIGHT_INFO_KEY, lightInfoMap);

        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_AMBIENT_LIGHT_UPDATE,
                event);
    }
}
