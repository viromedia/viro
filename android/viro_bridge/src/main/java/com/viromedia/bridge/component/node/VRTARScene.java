/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.view.ViewParent;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.ARAnchor;
import com.viro.renderer.jni.ARPlane;
import com.viro.renderer.jni.ARScene;
import com.viro.renderer.jni.Scene;
import com.viro.renderer.jni.ViroViewARCore;
import com.viromedia.bridge.component.VRTARSceneNavigator;
import com.viromedia.bridge.utility.ARUtils;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.ArrayList;

public class VRTARScene extends VRTScene implements ARScene.ARSceneDelegate {

    private static final String AMBIENT_LIGHT_INFO_KEY = "ambientLightInfo";
    private static final String INTENSITY_KEY = "intensity";
    private static final String COLOR_TEMPERATURE_KEY = "colorTemperature";

    public VRTARScene(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    protected Scene createSceneJni() {
        ARScene sceneControllerJni = new ARScene();
        sceneControllerJni.registerARDelegate(this);
        return sceneControllerJni;
    }

    public void addARPlane(ARPlane planeJni) {
        ((ARScene) mNativeScene).addARPlane(planeJni);
    }

    public void updateARPlane(ARPlane planeJni) {
        ((ARScene) mNativeScene).updateARPlane(planeJni);
    }

    public void removeARPlane(ARPlane planeJni) {
        ((ARScene) mNativeScene).removeARPlane(planeJni);
    }

    public void setAnchorDetectionTypes(ReadableArray types) {
        ViewParent parent = getParent();
        if (parent instanceof VRTARSceneNavigator) {
            VRTARSceneNavigator navigator = (VRTARSceneNavigator) parent;
            ViroViewARCore viroViewARCore = navigator.getARView();
            if (viroViewARCore != null) {
                ArrayList<String> typesList = new ArrayList<>();
                if (types != null) {
                    for (int i = 0; i < types.size(); i++) {
                        typesList.add(types.getString(i));
                    }
                }
                viroViewARCore.setAnchorDetectionTypes(typesList);
            }
        }
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
