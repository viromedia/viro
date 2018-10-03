/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.module;

import android.view.View;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.NativeViewHierarchyManager;
import com.facebook.react.uimanager.UIBlock;
import com.facebook.react.uimanager.UIManagerModule;
import com.viro.core.Vector;
import com.viromedia.bridge.component.VRTVRSceneNavigator;

public class SceneNavigatorModule extends ReactContextBaseJavaModule {

    public SceneNavigatorModule(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSceneNavigatorModule";
    }

    @ReactMethod
    public void requestExitVr(final int sceneNavTag) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View sceneView = nativeViewHierarchyManager.resolveView(sceneNavTag);
                if (sceneView instanceof VRTVRSceneNavigator) {
                    VRTVRSceneNavigator scene = (VRTVRSceneNavigator) sceneView;
                    scene.userDidRequestExitVR();
                }
            }
        });
    }

    @ReactMethod
    public void recenterTracking(final int sceneNavTag) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View view = nativeViewHierarchyManager.resolveView(sceneNavTag);
                if (view instanceof VRTVRSceneNavigator) {
                    VRTVRSceneNavigator sceneNavigator = (VRTVRSceneNavigator) view;
                    sceneNavigator.recenterTracking();
                }
            }
        });
    }

    @ReactMethod
    public void project(final int sceneNavTag, final ReadableArray point, final Promise promise) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View view = nativeViewHierarchyManager.resolveView(sceneNavTag);
                if (view instanceof VRTVRSceneNavigator) {
                    VRTVRSceneNavigator sceneNavigator = (VRTVRSceneNavigator) view;
                    float[] projectPoint = {0,0,0};
                    projectPoint[0] = (float)point.getDouble(0);
                    projectPoint[1] = (float)point.getDouble(1);
                    projectPoint[2] = (float)point.getDouble(2);
                    Vector projectedPoint = sceneNavigator.projectPoint(new Vector(projectPoint[0], projectPoint[1], projectPoint[2]));

                    WritableMap returnMap = Arguments.createMap();
                    WritableArray writablePosArray = Arguments.createArray();
                    writablePosArray.pushDouble(projectedPoint.x);
                    writablePosArray.pushDouble(projectedPoint.y);
                    returnMap.putArray("screenPosition", writablePosArray);
                    promise.resolve(returnMap);
                }
            }
        });
    }

    @ReactMethod
    public void unproject(final int sceneNavTag, final ReadableArray point, final Promise promise) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View view = nativeViewHierarchyManager.resolveView(sceneNavTag);
                if (view instanceof VRTVRSceneNavigator) {
                    VRTVRSceneNavigator sceneNavigator = (VRTVRSceneNavigator) view;
                    float[] unprojectPoint = {0,0,0};
                    unprojectPoint[0] = (float)point.getDouble(0);
                    unprojectPoint[1] = (float)point.getDouble(1);
                    unprojectPoint[2] = (float)point.getDouble(2);
                    Vector unProjectedPoint = sceneNavigator.unprojectPoint(new Vector(unprojectPoint[0], unprojectPoint[1], unprojectPoint[2]));

                    WritableMap returnMap = Arguments.createMap();
                    WritableArray writablePosArray = Arguments.createArray();
                    writablePosArray.pushDouble(unProjectedPoint.x);
                    writablePosArray.pushDouble(unProjectedPoint.y);
                    writablePosArray.pushDouble(unProjectedPoint.z);
                    returnMap.putArray("position", writablePosArray);
                    promise.resolve(returnMap);
                }
            }
        });
    }
}
