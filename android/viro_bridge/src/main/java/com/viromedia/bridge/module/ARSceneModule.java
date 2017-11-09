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
import com.facebook.react.uimanager.IllegalViewOperationException;
import com.facebook.react.uimanager.NativeViewHierarchyManager;
import com.facebook.react.uimanager.UIBlock;
import com.facebook.react.uimanager.UIManagerModule;
import com.viro.core.ARHitTestListener;
import com.viro.core.ARHitTestResult;
import com.viro.core.Renderer;
import com.viro.core.Vector;
import com.viro.core.ViroViewARCore;
import com.viromedia.bridge.component.VRTARSceneNavigator;
import com.viromedia.bridge.utility.ARUtils;


public class ARSceneModule extends ReactContextBaseJavaModule {

    public ARSceneModule(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTARSceneModule";
    }

    @ReactMethod
    public void performARHitTestWithRay(final int viewTag, final ReadableArray ray,
                                        final Promise promise) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View sceneView = nativeViewHierarchyManager.resolveView(viewTag);
                if (sceneView.getParent() == null || !(sceneView.getParent() instanceof VRTARSceneNavigator)) {
                    throw new IllegalViewOperationException("Invalid view returned when calling " +
                            "calling performARHitTestWithRay: expected ViroARSceneNavigator as parent");
                }

                VRTARSceneNavigator arSceneNavigator = (VRTARSceneNavigator) sceneView.getParent();
                ViroViewARCore arView = arSceneNavigator.getARView();

                if (ray.size() != 3) {
                    promise.resolve(Arguments.createArray());
                    return;
                }

                float[] rayArray = new float[ray.size()];
                rayArray[0] = (float) ray.getDouble(0);
                rayArray[1] = (float) ray.getDouble(1);
                rayArray[2] = (float) ray.getDouble(2);

                arView.performARHitTestWithRay(new Vector(rayArray), new ARHitTestListener() {
                    @Override
                    public void onHitTestFinished(ARHitTestResult[] arHitTestResults) {
                        WritableArray returnArray = Arguments.createArray();
                        for (ARHitTestResult result : arHitTestResults) {
                            returnArray.pushMap(ARUtils.mapFromARHitTestResult(result));
                        }
                        promise.resolve(returnArray);
                    }
                });
            }
        });
    }

    @ReactMethod
    public void performARHitTestWithPosition(final int viewTag, final ReadableArray position,
                                        final Promise promise) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View sceneView = nativeViewHierarchyManager.resolveView(viewTag);
                if (sceneView.getParent() == null || !(sceneView.getParent() instanceof VRTARSceneNavigator)) {
                    throw new IllegalViewOperationException("Invalid view returned when calling " +
                            "calling performARHitTestWithPosition: expected ViroARSceneNavigator as parent");
                }

                VRTARSceneNavigator arSceneNavigator = (VRTARSceneNavigator) sceneView.getParent();
                ViroViewARCore arView = arSceneNavigator.getARView();

                if (position.size() != 3) {
                    promise.resolve(Arguments.createArray());
                    return;
                }

                float[] positionArray = new float[position.size()];
                positionArray[0] = (float) position.getDouble(0);
                positionArray[1] = (float) position.getDouble(1);
                positionArray[2] = (float) position.getDouble(2);

                arView.performARHitTestWithPosition(new Vector(positionArray), new ARHitTestListener() {
                    @Override
                    public void onHitTestFinished(ARHitTestResult[] arHitTestResults) {
                        WritableArray returnArray = Arguments.createArray();
                        for (ARHitTestResult result : arHitTestResults) {
                            returnArray.pushMap(ARUtils.mapFromARHitTestResult(result));
                        }
                        promise.resolve(returnArray);
                    }
                });
            }
        });
    }

}
