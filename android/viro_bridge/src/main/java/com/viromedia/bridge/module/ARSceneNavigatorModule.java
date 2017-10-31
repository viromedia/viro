/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.module;

import android.Manifest;
import android.app.Activity;
import android.util.Log;
import android.view.View;

import com.facebook.react.ReactActivity;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Callback;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.modules.core.PermissionListener;
import com.facebook.react.uimanager.IllegalViewOperationException;
import com.facebook.react.uimanager.NativeViewHierarchyManager;
import com.facebook.react.uimanager.UIBlock;
import com.facebook.react.uimanager.UIManagerModule;
import com.viro.renderer.jni.ViroMediaRecorder;
import com.viromedia.bridge.component.VRTARSceneNavigator;
import com.viro.renderer.jni.ViroMediaRecorderDelegate;
import static com.viro.renderer.jni.ViroMediaRecorderDelegate.ERROR.*;

public class ARSceneNavigatorModule extends ReactContextBaseJavaModule {
    private static final String RECORDING_SUCESS_KEY = "success";
    private static final String RECORDING_URL_KEY = "url";
    private static final String RECORDING_ERROR_KEY = "errorCode";
    private static final int PERMISSION_REQ_CODE_AUDIO = 1;
    private static final int PERMISSION_REQ_CODE_STORAGE = 2;

    private ReactApplicationContext mContext;

    public ARSceneNavigatorModule(ReactApplicationContext context) {
        super(context);
        mContext = context;
    }

    @Override
    public String getName() {
        return "VRTARSceneNavigatorModule";
    }

    @ReactMethod
    public void startVideoRecording(final int sceneNavTag, final String fileName,
                                    final boolean saveToCameraRool, final Callback reactErrorDelegate) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View sceneView = nativeViewHierarchyManager.resolveView(sceneNavTag);
                if (!(sceneView instanceof VRTARSceneNavigator)) {
                    throw new IllegalViewOperationException("Viro: Attempted to call startVideoRecording on a non-ARSceneNav view!");
                }
                VRTARSceneNavigator scene = (VRTARSceneNavigator) sceneView;

                // Grab the recorder from the ar scene view
                final ViroMediaRecorder recorder = scene.getARView().getRecorder();
                if (recorder == null){
                    reactErrorDelegate.invoke(UNSUPPORTED_PLATFORM.toInt());
                    return;
                }

                // Construct an error listener callback that may be triggered during recording.
                final ViroMediaRecorderDelegate viroErrorDelegate = new ViroMediaRecorderDelegate() {
                    @Override
                    public void onTaskCompleted(boolean success, ERROR error, String url) {
                        reactErrorDelegate.invoke(error.toInt());
                    }
                };

                // Start recording if we have the right permissions
                checkPermissionsAndRun(new PermissionListener() {
                    @Override
                    public boolean onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
                        recorder.startRecordingAsync(fileName, saveToCameraRool, viroErrorDelegate);
                        return true;
                    }
                }, true);
            }
        });
    }

    @ReactMethod
    public void stopVideoRecording(final int sceneNavTag, final Promise promise) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View sceneView = nativeViewHierarchyManager.resolveView(sceneNavTag);
                if (!(sceneView instanceof VRTARSceneNavigator)) {
                    throw new IllegalViewOperationException("Viro: Attempted to call startVideoRecording on a non-ARSceneNav view!");
                }
                VRTARSceneNavigator scene = (VRTARSceneNavigator) sceneView;

                // Grab the recorder from the ar scene view
                final ViroMediaRecorder recorder = scene.getARView().getRecorder();
                if (recorder == null){
                    WritableMap returnMap = Arguments.createMap();
                    returnMap.putBoolean(RECORDING_SUCESS_KEY, false);
                    returnMap.putInt(RECORDING_ERROR_KEY, UNSUPPORTED_PLATFORM.toInt());
                    returnMap.putString(RECORDING_URL_KEY, null);
                    promise.resolve(returnMap);
                    return;
                }

                // Construct a completion delegate callback to be notified of the result of the recording.
                final ViroMediaRecorderDelegate completionCallback = new ViroMediaRecorderDelegate() {
                    @Override
                    public void onTaskCompleted(boolean success, ERROR error, String url) {
                        WritableMap returnMap = Arguments.createMap();
                        returnMap.putBoolean(RECORDING_SUCESS_KEY, success);
                        returnMap.putInt(RECORDING_ERROR_KEY, error.toInt());
                        returnMap.putString(RECORDING_URL_KEY, url);
                        promise.resolve(returnMap);
                    }
                };

                // Stop recording if we have the right permissions
                checkPermissionsAndRun(new PermissionListener() {
                    @Override
                    public boolean onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
                        recorder.stopRecordingAsync(completionCallback);
                        return true;
                    }
                }, true);
            }
        });
    }

    @ReactMethod
    public void takeScreenshot(final int sceneNavTag, final String fileName,
                               final boolean saveToCameraRool, final Promise promise) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View sceneView = nativeViewHierarchyManager.resolveView(sceneNavTag);
                if (!(sceneView instanceof VRTARSceneNavigator)) {
                    throw new IllegalViewOperationException("Viro: Attempted to call startVideoRecording on a non-ARSceneNav view!");
                }
                VRTARSceneNavigator scene = (VRTARSceneNavigator) sceneView;

                // Grab the recorder from the ar scene view
                final ViroMediaRecorder recorder = scene.getARView().getRecorder();
                if (recorder == null){
                    WritableMap returnMap = Arguments.createMap();
                    returnMap.putBoolean(RECORDING_SUCESS_KEY, false);
                    returnMap.putInt(RECORDING_ERROR_KEY, UNSUPPORTED_PLATFORM.toInt());
                    returnMap.putString(RECORDING_URL_KEY, null);
                    promise.resolve(returnMap);
                    return;
                }

                // Construct a completion delegate callback to be notified of sceenshot results.
                final ViroMediaRecorderDelegate callback = new ViroMediaRecorderDelegate() {
                    @Override
                    public void onTaskCompleted(boolean success, ERROR error, String url) {
                        WritableMap returnMap = Arguments.createMap();
                        returnMap.putBoolean(RECORDING_SUCESS_KEY, success);
                        returnMap.putInt(RECORDING_ERROR_KEY, error.toInt());
                        returnMap.putString(RECORDING_URL_KEY, url);
                        promise.resolve(returnMap);
                    }
                };
                
                // Schedule taking a screen shot if we have the right permission
                checkPermissionsAndRun(new PermissionListener() {
                    @Override
                    public boolean onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
                        recorder.takeScreenShotAsync(fileName, saveToCameraRool, callback);
                        return true;
                    }
                }, false);
            }
        });
    }

    private void checkPermissionsAndRun(PermissionListener listener, boolean audioAndRecordingPerm){
        Activity activity = mContext.getCurrentActivity();

        // return if we already have permissions
        if (audioAndRecordingPerm && ViroMediaRecorder.hasAudioAndRecordingPermissions(mContext)) {
            listener.onRequestPermissionsResult(0, null, null);
            return;
        } else if (!audioAndRecordingPerm && ViroMediaRecorder.hasRecordingPermissions(mContext)) {
            listener.onRequestPermissionsResult(0, null, null);
            return;
        }

        if (!(activity instanceof ReactActivity)){
            Log.e("Viro","Error: Missing ReactActivity required for checking recording permissions!");

            // Trigger a permission failure callback.
            listener.onRequestPermissionsResult(0, null, null);
            return;
        }

        ReactActivity reactActivity = (ReactActivity) activity;
        if (audioAndRecordingPerm){
            reactActivity.requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE,
                    Manifest.permission.RECORD_AUDIO}, PERMISSION_REQ_CODE_AUDIO, listener);
        } else {
            reactActivity.requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    PERMISSION_REQ_CODE_STORAGE, listener);
        }
    }
}
