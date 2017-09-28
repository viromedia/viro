/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.view.View;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.JSApplicationCausedNativeException;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.CameraCallback;
import com.viro.renderer.jni.SceneControllerJni;
import com.viro.renderer.jni.TextureJni;
import com.viro.renderer.jni.VideoTextureJni;
import com.viro.renderer.jni.RendererJni;
import com.viromedia.bridge.component.node.control.Camera;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;

public class Scene extends Node implements SceneControllerJni.SceneDelegate {
    private static final String TAG = Scene.class.getSimpleName();
    private static final String SIZE_KEY = "size";
    private static final String WALL_MATERIAL_KEY = "wallMaterial";
    private static final String CEILING_MATERIAL_KEY = "ceilingMaterial";
    private static final String FLOOR_MATERIAL_KEY = "floorMaterial";
    private static final String DEFAULT_MATERIAL = "transparent";
    private static final float[] DEFAULT_SIZE = {0,0,0};

    protected SceneControllerJni mNativeSceneController;
    private RendererJni mNativeRenderer;
    private Camera mCamera;
    private float[] mSoundRoomSize = DEFAULT_SIZE;
    private String mWallMaterial;
    private String mCeilingMaterial;
    private String mFloorMaterial;

    // Platform Information (set by SceneNavigator.java)
    private String mPlatform;
    private String mHeadset;
    private String mController;

    public Scene(ReactApplicationContext reactContext) {
        super(reactContext);
        mNativeSceneController = createSceneControllerJni();
        mNativeSceneController.registerDelegate(this);
        mVisible = true; // Scenes are always visible!
        mParentHasAppeared = true;
    }

    /*
     This function creates and returns a SceneControllerJni. Child classes
     should override to return a different SceneController if desired.
     */
    protected SceneControllerJni createSceneControllerJni() {
        return new SceneControllerJni(getNodeJni());
    }

    @Override
    public void onTearDown() {
        if (!isTornDown()) {
            mNativeSceneController.destroy();
        }
        super.onTearDown();
    }

    public SceneControllerJni getNativeScene() {
        return mNativeSceneController;
    }

    public void setBackgroundVideoTexture(VideoTextureJni videoTexture) {
        mNativeSceneController.setBackgroundVideoTexture(videoTexture);
    }

    public void setNativeRenderer(RendererJni nativeRenderer) {
        mNativeRenderer = nativeRenderer;
        setCameraIfPossible();
    }

    public void setBackgroundImageTexture(TextureJni texture) {
        mNativeSceneController.setBackgroundImageTexture(texture);
    }

    public void setBackgroundRotation(float[] rotation) {
        mNativeSceneController.setBackgroundRotation(rotation);
    }

    public void setBackgroundCubeImageTexture(TextureJni texture) {
        mNativeSceneController.setBackgroundCubeImageTexture(texture);
    }

    public void setBackgroundCubeWithColor(long color) {
        mNativeSceneController.setBackgroundCubeWithColor(color);
    }

    public void setSoundRoom(ReadableMap soundRoom) {
        mSoundRoomSize = soundRoom.hasKey(SIZE_KEY) ? Helper.toFloatArray(soundRoom.getArray(SIZE_KEY)) : DEFAULT_SIZE;
        mWallMaterial = soundRoom.hasKey(WALL_MATERIAL_KEY) ? soundRoom.getString(WALL_MATERIAL_KEY) : DEFAULT_MATERIAL;
        mCeilingMaterial = soundRoom.hasKey(CEILING_MATERIAL_KEY) ? soundRoom.getString(CEILING_MATERIAL_KEY) : DEFAULT_MATERIAL;
        mFloorMaterial = soundRoom.hasKey(FLOOR_MATERIAL_KEY) ? soundRoom.getString(FLOOR_MATERIAL_KEY) : DEFAULT_MATERIAL;
        if (mRenderContext != null) {
            mNativeSceneController.setSoundRoom(mRenderContext, mSoundRoomSize, mWallMaterial, mCeilingMaterial, mFloorMaterial);
        }
    }

    @Override
    public void addView(View child, int index) {
        super.addView(child, index);
    }

    private void setCameraIfPossible() {
        if (mCamera != null && mNativeRenderer != null && !isTornDown()) {
            mNativeRenderer.setPointOfView(mCamera.getNodeJni());
        }
    }

    public void setCamera(Camera camera) {
        mCamera = camera;
        setCameraIfPossible();
    }

    public void removeCamera(Camera camera) {
        if (mCamera != camera) {
            return;
        }

        if (mNativeRenderer != null && !isTornDown()) {
            mNativeRenderer.setPointOfView(null);
        }
        mCamera = null;
    }

    public void getCameraPositionAsync(CameraCallback callback) {
        if (mRenderContext == null || isTornDown()) {
            callback.onGetCameraOrientation(0,0,0,0,0,0,0,0,0,0,0,0);
        }
        else {
            mRenderContext.getCameraOrientation(callback);
        }
    }

    /**
     * Override the setId function to notify the JS layer of the platform information. We do this
     * here because if you can't emit events to the JS layer before ID is set.
     */
    @Override
    public void setId(int id) {
        super.setId(id);
        // make sure platform info was set before we go notifying js of nothing.
        if (mPlatform != null) {
            notifyPlatformInformation();
        }
    }

    /**
     * This method is called to set the platform information on the Scene so that it can call
     * the listener in javascript to let developers listen for the platform.
     */
    public void setPlatformInformation(String platform, String headset, String controller) {
        mPlatform = platform;
        mHeadset = headset;
        mController = controller;

        if (getId() != View.NO_ID) {
            notifyPlatformInformation();
        }
    }

    private void notifyPlatformInformation() {
        WritableMap event = Arguments.createMap();
        event.putString("platform", mPlatform);
        event.putString("headset", mHeadset);
        event.putString("controller", mController);
        WritableMap eventContainer = Arguments.createMap();
        eventContainer.putMap("platformInfoViro", event);
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_PLATFORM_UPDATE,
                eventContainer);
    }

    public void setPhysicsWorld(ReadableMap map){
        if (map == null){
            return;
        }

        if (map.hasKey("gravity")){
            ReadableArray readableParams = map.getArray("gravity");
            if (readableParams.size() != 3){
                throw new JSApplicationCausedNativeException("Incorrect parameters provided " +
                        "for gravity, expected: [x, y, z]!");
            } else {
                float params[] = new float[readableParams.size()];
                for (int i = 0; i < readableParams.size(); i ++){
                    params[i] = (float) readableParams.getDouble(i);
                }
                mNativeSceneController.setPhysicsWorldGravity(params);
            }
        }

        if (map.hasKey("drawBounds")) {
            mNativeSceneController.setPhysicsDebugDraw(map.getBoolean("drawBounds"));
        } else {
            mNativeSceneController.setPhysicsDebugDraw(false);
        }
    }

    public void addPhysicsBodyToScene(Node node){
        if (!isTornDown()) {
            mNativeSceneController.attachBodyToPhysicsWorld(node.getNodeJni());
        }
    }

    public void removePhysicsBodyFromScene(Node node){
        if (!isTornDown()) {
            mNativeSceneController.detachBodyFromPhysicsWorld(node.getNodeJni());
        }
    }

    public void findCollisionsWithRayAsync(float[] fromPos, float toPos[], boolean closest,
                                           String tag,
                                           SceneControllerJni.PhysicsWorldHitTestCallback callback){
        mNativeSceneController.findCollisionsWithRayAsync(fromPos, toPos, closest, tag, callback);
    }

    public void findCollisionsWithShapeAsync(float[] from, float[] to, String shapeType,
                                             float[] params, String tag,
                                             SceneControllerJni.PhysicsWorldHitTestCallback callback) {
        mNativeSceneController.findCollisionsWithShapeAsync(from, to, shapeType, params, tag, callback);
    }

    /**
     * Attach Renderer Delegate callbacks to the Scene to be propagated
     * across all of it's child views.
     */
    @Override
    public void onSceneWillAppear() {
        sceneWillAppear();
    }

    @Override
    public void onSceneDidAppear() {
        sceneDidAppear();
    }

    @Override
    public void onSceneWillDisappear() {
        sceneWillDisappear();
    }

    @Override
    public void onSceneDidDisappear() {
        sceneDidDisappear();
    }
}
