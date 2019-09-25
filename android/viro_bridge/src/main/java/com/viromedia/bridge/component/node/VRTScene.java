//  Copyright © 2016 Viro Media. All rights reserved.
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

import android.view.View;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.JSApplicationCausedNativeException;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.EventDelegate;
import com.viro.core.internal.CameraCallback;
import com.viro.core.Node;
import com.viro.core.PhysicsShape;
import com.viro.core.PhysicsWorld;
import com.viro.core.Scene;
import com.viro.core.Texture;
import com.viro.core.Vector;
import com.viro.core.VideoTexture;
import com.viro.core.Renderer;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;

public class VRTScene extends VRTNode implements Scene.VisibilityListener {
    private static final String TAG = VRTScene.class.getSimpleName();
    private static final String SIZE_KEY = "size";
    private static final String WALL_MATERIAL_KEY = "wallMaterial";
    private static final String CEILING_MATERIAL_KEY = "ceilingMaterial";
    private static final String FLOOR_MATERIAL_KEY = "floorMaterial";
    private static final String DEFAULT_MATERIAL = "transparent";
    private static final float[] DEFAULT_SIZE = {0,0,0};

    protected Scene mNativeScene;
    private Renderer mNativeRenderer;
    private VRTCamera mCamera;
    private float[] mSoundRoomSize = DEFAULT_SIZE;
    private String mWallMaterial;
    private String mCeilingMaterial;
    private String mFloorMaterial;

    // Platform Information (set by SceneNavigator.java)
    private String mPlatform;
    private String mHeadset;
    private String mController;

    public VRTScene(ReactContext reactContext) {
        super(reactContext);
        mVisible = true; // Scenes are always visible!
        mParentHasAppeared = true;
    }

    @Override
    protected Node createNodeJni() {
        mNativeScene = createSceneJni();
        return mNativeScene.getRootNode();
    }

    /*
     This function creates and returns a Scene. Child classes
     should override to return a different Scene if desired.
     */
    protected Scene createSceneJni() {
        Scene sceneJni = new Scene();
        sceneJni.setVisibilityListener(this);
        return sceneJni;
    }

    @Override
    public void onTearDown() {
        if (!isTornDown()) {
            mNativeScene.dispose();
        }
        super.onTearDown();
    }

    public Scene getNativeScene() {
        return mNativeScene;
    }

    public void setBackgroundVideoTexture(VideoTexture videoTexture) {
        mNativeScene.setBackgroundTexture(videoTexture);
    }

    public void setNativeRenderer(Renderer nativeRenderer) {
        mNativeRenderer = nativeRenderer;
        setCameraIfPossible();
    }

    public void setBackgroundImageTexture(Texture texture) {
        mNativeScene.setBackgroundTexture(texture);
    }

    public void setBackgroundRotation(float[] rotation) {
        mNativeScene.setBackgroundRotation(Helper.toRadiansVector(rotation));
    }

    public void setBackgroundCubeImageTexture(Texture texture) {
        mNativeScene.setBackgroundCubeTexture(texture);
    }

    public void setBackgroundCubeWithColor(long color) {
        mNativeScene.setBackgroundCubeWithColor(color);
    }

    public void setSoundRoom(ReadableMap soundRoom) {
        mSoundRoomSize = soundRoom.hasKey(SIZE_KEY) ? Helper.toFloatArray(soundRoom.getArray(SIZE_KEY)) : DEFAULT_SIZE;
        mWallMaterial = soundRoom.hasKey(WALL_MATERIAL_KEY) ? soundRoom.getString(WALL_MATERIAL_KEY) : DEFAULT_MATERIAL;
        mCeilingMaterial = soundRoom.hasKey(CEILING_MATERIAL_KEY) ? soundRoom.getString(CEILING_MATERIAL_KEY) : DEFAULT_MATERIAL;
        mFloorMaterial = soundRoom.hasKey(FLOOR_MATERIAL_KEY) ? soundRoom.getString(FLOOR_MATERIAL_KEY) : DEFAULT_MATERIAL;
        if (mViroContext != null) {
            mNativeScene.setSoundRoom(mViroContext, new Vector(mSoundRoomSize),
                    Scene.AudioMaterial.valueFromString(mWallMaterial),
                    Scene.AudioMaterial.valueFromString(mCeilingMaterial),
                    Scene.AudioMaterial.valueFromString(mFloorMaterial));
        }
    }

    public void setPostProcessEffects(ReadableArray effects){
        if (effects == null || effects.size() == 0){
            mNativeScene.setEffects(null);
            return;
        }

        String nativeEffects[] = new String[effects.size()];
        for (int i = 0; i < effects.size(); i ++){
            String effect = effects.getString(i);
            nativeEffects[i] = effect;
        }
        if (!mNativeScene.setEffects(nativeEffects)){
            onError("Viro: Attempted to set an invalid effect!");
        }
    }

    public void setCanCameraTransformUpdate(boolean canCameraTransformUpdate) {
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_CAMERA_TRANSFORM_UPDATE, canCameraTransformUpdate);
    }

    @Override
    public void addView(View child, int index) {
        super.addView(child, index);
    }

    private void setCameraIfPossible() {
        if (mCamera != null && mNativeRenderer != null && !isTornDown()) {
            mNativeRenderer.setPointOfView(mCamera.getNodeRootTransformCamera().getNodeJni());
        }
    }

    public void setCamera(VRTCamera camera) {
        mCamera = camera;
        setCameraIfPossible();
    }

    public void removeCamera(VRTCamera camera) {
        if (mCamera != camera) {
            return;
        }

        if (mNativeRenderer != null && !isTornDown()) {
            mNativeRenderer.setPointOfView(null);
        }
        mCamera = null;
    }

    public void getCameraPositionAsync(CameraCallback callback) {
        if (mViroContext == null || isTornDown()) {
            callback.onGetCameraOrientation(0,0,0,0,0,0,0,0,0,0,0,0);
        }
        else {
            mViroContext.getCameraOrientation(callback);
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
                mNativeScene.getPhysicsWorld().setGravity(new Vector(params));
            }
        }

        if (map.hasKey("drawBounds")) {
            mNativeScene.getPhysicsWorld().setDebugDraw(map.getBoolean("drawBounds"));
        } else {
            mNativeScene.getPhysicsWorld().setDebugDraw(false);
        }
    }

    public void findCollisionsWithRayAsync(float[] fromPos, float toPos[], boolean closest,
                                           String tag,
                                           PhysicsWorld.HitTestListener callback) {
        mNativeScene.getPhysicsWorld().findCollisionsWithRayAsync(new Vector(fromPos), new Vector(toPos),
                closest, tag, callback);
    }

    public void findCollisionsWithShapeAsync(float[] from, float[] to, PhysicsShape shape, String tag,
                                             PhysicsWorld.HitTestListener callback) {
        mNativeScene.getPhysicsWorld().findCollisionsWithShapeAsync(new Vector(from), new Vector(to),
                shape, tag, callback);
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
