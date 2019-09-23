//  Copyright © 2017 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
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
        return new VRTARScene(reactContext);
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

    @ReactProp(name = "canARPointCloudUpdate", defaultBoolean = VRTNode.DEFAULT_CAN_AR_POINT_CLOUD_UPDATE)
    public void setCanARPointCloudUpdate(VRTARScene scene, boolean canARPointCloudUpdate) {
        scene.setCanARPointCloudUpdate(canARPointCloudUpdate);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map map = super.getExportedCustomDirectEventTypeConstants();
        map.put(ViroEvents.ON_AMBIENT_LIGHT_UPDATE, MapBuilder.of("registrationName", ViroEvents.ON_AMBIENT_LIGHT_UPDATE));
        map.put(ViroEvents.ON_TRACKING_UPDATED, MapBuilder.of("registrationName", ViroEvents.ON_TRACKING_UPDATED));
        map.put(ViroEvents.ON_ANCHOR_FOUND, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_FOUND));
        map.put(ViroEvents.ON_ANCHOR_UPDATED, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_UPDATED));
        map.put(ViroEvents.ON_ANCHOR_REMOVED, MapBuilder.of("registrationName", ViroEvents.ON_ANCHOR_REMOVED));
        map.put(ViroEvents.ON_CAMERA_AR_HIT_TEST_VIRO, MapBuilder.of("registrationName", ViroEvents.ON_CAMERA_AR_HIT_TEST_VIRO));
        map.put(ViroEvents.ON_AR_POINT_CLOUD_UPDATE, MapBuilder.of("registrationName", ViroEvents.ON_AR_POINT_CLOUD_UPDATE));
        return map;
    }

}