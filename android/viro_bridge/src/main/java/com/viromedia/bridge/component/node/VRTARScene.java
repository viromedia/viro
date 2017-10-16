/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.ARAnchor;
import com.viro.renderer.jni.ARPlane;
import com.viro.renderer.jni.ARSceneController;
import com.viro.renderer.jni.SceneController;
import com.viromedia.bridge.utility.ARUtils;
import com.viromedia.bridge.utility.ViroEvents;

public class VRTARScene extends VRTScene implements ARSceneController.ARSceneDelegate {

    private static final String AMBIENT_LIGHT_INFO_KEY = "ambientLightInfo";
    private static final String INTENSITY_KEY = "intensity";
    private static final String COLOR_TEMPERATURE_KEY = "colorTemperature";

    public VRTARScene(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    protected SceneController createSceneControllerJni() {
        ARSceneController sceneControllerJni = new ARSceneController();
        sceneControllerJni.registerARDelegate(this);
        return sceneControllerJni;
    }

    public void addARPlane(ARPlane planeJni) {
        ((ARSceneController) mNativeSceneController).addARPlane(planeJni);
    }

    public void updateARPlane(ARPlane planeJni) {
        ((ARSceneController) mNativeSceneController).updateARPlane(planeJni);
    }

    public void removeARPlane(ARPlane planeJni) {
        ((ARSceneController) mNativeSceneController).removeARPlane(planeJni);
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

    @Override
    public void onAnchorFound(ARAnchor arAnchor) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putMap("anchor", ARUtils.mapFromARAnchor(arAnchor));

        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
            getId(),
            ViroEvents.ON_ANCHOR_FOUND,
            returnMap);
    }

    @Override
    public void onAnchorUpdated(ARAnchor arAnchor) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putMap("anchor", ARUtils.mapFromARAnchor(arAnchor));

        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
            getId(),
            ViroEvents.ON_ANCHOR_UPDATED,
            returnMap);
    }

    @Override
    public void onAnchorRemoved(ARAnchor arAnchor) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putMap("anchor", ARUtils.mapFromARAnchor(arAnchor));

        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
            getId(),
            ViroEvents.ON_ANCHOR_REMOVED,
            returnMap);
    }
}
