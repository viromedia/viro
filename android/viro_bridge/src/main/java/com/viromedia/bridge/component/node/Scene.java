/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.view.View;
import android.view.ViewParent;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.SceneJni;
import com.viro.renderer.jni.TextureJni;
import com.viro.renderer.jni.VideoTextureJni;
import com.viro.renderer.jni.RendererJni;
import com.viromedia.bridge.component.Camera;
import com.viromedia.bridge.component.OrbitCamera;

public class Scene extends Node implements SceneJni.SceneDelegate {
    private static final String TAG = Scene.class.getSimpleName();
    private final SceneJni mNativeScene;
    private RendererJni mNativeRenderer;
    private Camera mCamera;
    private boolean mReticleEnabled = true;
    public Scene(ReactApplicationContext reactContext) {
        super(reactContext);
        mNativeScene = new SceneJni(getNodeJni());
        mNativeScene.registerDelegate(this);
    }

    @Override
    protected void onTearDown(){
        mNativeScene.destroy();
        super.onTearDown();
    }

    public SceneJni getNativeScene(){
        return mNativeScene;
    }

    public void setBackgroundVideoTexture(VideoTextureJni videoTexture) {
        mNativeScene.setBackgroundVideoTexture(videoTexture);
    }

    public void setNativeRenderer(RendererJni nativeRenderer) {
        mNativeRenderer = nativeRenderer;
        mNativeRenderer.enableReticle(mReticleEnabled);
        setCameraIfPossible();
    }

    public void setReticleEnabled(boolean enable) {
        mReticleEnabled = enable;
        if (mNativeRenderer != null) {
            mNativeRenderer.enableReticle(enable);
        }
    }

    public void setBackgroundImageTexture(TextureJni texture) {
        mNativeScene.setBackgroundImageTexture(texture);
    }

    public void setBackgroundRotation(float[] rotation) {
        mNativeScene.setBackgroundRotation(rotation);
    }

    public void setBackgroundCubeImageTexture(TextureJni texture) {
        mNativeScene.setBackgroundCubeImageTexture(texture);
    }

    public void setBackgroundCubeWithColor(long color) {
        mNativeScene.setBackgroundCubeWithColor(color);
    }

    @Override
    public void addView(View child, int index) {
        super.addView(child, index);
    }

    private void setCameraIfPossible() {
        if (mCamera != null && mNativeRenderer != null) {
            Node parent = (Node) mCamera.getParent();
            mNativeRenderer.setPointOfView(parent.getNodeJni());
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

        if (mNativeRenderer != null) {
            mNativeRenderer.setPointOfView(null);
        }
        mCamera = null;
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
