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
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.uimanager.NativeViewHierarchyManager;
import com.facebook.react.uimanager.UIBlock;
import com.facebook.react.uimanager.UIManagerModule;
import com.viro.renderer.jni.CameraCallback;
import com.viromedia.bridge.component.node.control.Camera;
import com.viromedia.bridge.component.node.Scene;

public class CameraModule extends ReactContextBaseJavaModule {

    public CameraModule(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTCameraModule";
    }

    @ReactMethod
    public void getCameraOrientation(final int sceneTag, final Promise promise) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View sceneView = nativeViewHierarchyManager.resolveView(sceneTag);
                if (sceneView instanceof Scene) {
                    Scene scene = (Scene) sceneView;

                    scene.getCameraPositionAsync(new CameraCallback() {
                        @Override
                        public void onGetCameraOrientation(float posX, float poxY, float posZ,
                                                           float rotEulerX, float rotEulerY, float rotEulerZ) {
                            WritableArray array = Arguments.createArray();
                            array.pushDouble(posX);
                            array.pushDouble(poxY);
                            array.pushDouble(posZ);
                            array.pushDouble(rotEulerX);
                            array.pushDouble(rotEulerY);
                            array.pushDouble(rotEulerZ);
                            promise.resolve(array);
                        }
                    });
                }


            }
        });
    }

    @ReactMethod
    public void setSceneCamera(final int sceneTag, final int cameraTag) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View sceneView = nativeViewHierarchyManager.resolveView(sceneTag);
                View cameraView = nativeViewHierarchyManager.resolveView(cameraTag);

                if (!(cameraView instanceof Camera)) {
                    //RCTLogError(@"Invalid view returned when setting camera: expected VRTCamera, got [%@]", cameraView);
                }
                else if (!(sceneView instanceof Scene)) {
                    //RCTLogError(@"Invalid view returned when setting camera: expected VRTScene, got [%@]", sceneView);
                }
                else {
                    Camera camera = (Camera) cameraView;
                    Scene scene = (Scene) sceneView;

                    scene.setCamera(camera);
                }
            }
        });
    }

    @ReactMethod
    public void removeSceneCamera(final int sceneTag, final int cameraTag) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View sceneView = nativeViewHierarchyManager.resolveView(sceneTag);
                View cameraView = nativeViewHierarchyManager.resolveView(cameraTag);

                if (!(cameraView instanceof Camera)) {
                    //RCTLogError(@"Invalid view returned when removing camera: expected VRTCamera, got [%@]", cameraView);
                }
                else if (!(sceneView instanceof Scene)) {
                    //RCTLogError(@"Invalid view returned when removing camera: expected VRTScene, got [%@]", sceneView);
                }
                else {
                    Camera camera = (Camera) cameraView;
                    Scene scene = (Scene) sceneView;

                    scene.removeCamera(camera);
                }
            }
        });
    }
}
