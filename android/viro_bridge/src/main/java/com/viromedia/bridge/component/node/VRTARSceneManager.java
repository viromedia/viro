/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viro.core.Vector;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

import javax.annotation.Nullable;

public class VRTARSceneManager extends VRTSceneManager<VRTARScene> {

    public VRTARSceneManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTARScene";
    }

    @Override
    protected VRTARScene createViewInstance(ThemedReactContext reactContext) {
        return new VRTARScene(getContext());
    }

    @ReactProp(name="displayPointCloud", defaultBoolean = false)
    public void setDisplayPointCloud(VRTARScene scene, boolean displayPointCloud) {
        scene.setDisplayPointCloud(displayPointCloud);
    }

    @ReactProp(name="pointCloudImage")
    public void setPointCloudImage(VRTARScene scene, ReadableMap pointCloudImage) {
        scene.setPointCloudImage(pointCloudImage);
    }

    @ReactProp(name="pointCloudScale")
    public void setPointCloudScale(VRTARScene scene, ReadableArray pointCloudScale) {
        try {
            if (pointCloudScale == null) {
                scene.setPointCloudScale(new Vector(.01f, .01f, .01f));

            } else {
                scene.setPointCloudScale(Helper.toVector(pointCloudScale));
            }
        } catch (IllegalArgumentException e) {
            throw new IllegalArgumentException("imageScale requires 3 scale values for [x, y, z].");
        }
    }

    @ReactProp(name="pointCloudMaxPoints", defaultInt = 500)
    public void setPointCloudMaxPoints(VRTARScene scene, int maxPoints) {
        scene.setPointCloudMaxPoints(maxPoints);
    }

    @ReactProp(name="anchorDetectionTypes")
    public void setAnchorDetectionTypes(VRTARScene scene, @Nullable ReadableArray array) {
        scene.setAnchorDetectionTypes(array);
    }

    @ReactProp(name = "canCameraARHitTest", defaultBoolean = VRTNode.DEFAULT_CAN_CAMERA_HIT_TEST)
    public void setCanCameraARHitTest(VRTARScene scene, boolean canCameraARHitTest) {
        scene.setCanCameraARHitTest(canCameraARHitTest);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map map = super.getExportedCustomDirectEventTypeConstants();
        map.put(ViroEvents.ON_AMBIENT_LIGHT_UPDATE, MapBuilder.of("registrationName", ViroEvents.ON_AMBIENT_LIGHT_UPDATE));
        map.put(ViroEvents.ON_TRACKING_INITIALIZED, MapBuilder.of("registrationName", ViroEvents.ON_TRACKING_INITIALIZED));
        map.put(ViroEvents.ON_ANCHOR_FOUND, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_FOUND));
        map.put(ViroEvents.ON_ANCHOR_UPDATED, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_UPDATED));
        map.put(ViroEvents.ON_ANCHOR_REMOVED, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_REMOVED));
        map.put(ViroEvents.ON_CAMERA_AR_HIT_TEST_VIRO, MapBuilder.of("registrationName", ViroEvents.ON_CAMERA_AR_HIT_TEST_VIRO));
        return map;
    }

}