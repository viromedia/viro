/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.CameraJni;
import com.viro.renderer.jni.NodeJni;

public class Camera extends Control {
    protected CameraJni mNativeCamera;
    private float[] mPosition;

    public Camera(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public void onTearDown(){
        if (mNativeCamera != null){
            mNativeCamera.destroy();
            mNativeCamera = null;
        }
        super.onTearDown();
    }

    public void addToNode(NodeJni node) {
        mNativeCamera.addToNode(node);
    }

    public void removeFromNode(NodeJni node) {
        mNativeCamera.removeFromNode(node);
    }

    public void setPosition(float[] position) {
        mPosition = position;
    }

    public float[] getPosition() {
        return mPosition;
    }

    public String getRotationType() {
        return "Standard";
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();

        if (mNativeCamera == null) {
            mNativeCamera = new CameraJni();
        }
        mNativeCamera.setPosition(mPosition);
        mNativeCamera.setRotationType(getRotationType());
        addToNode(getNodeJni());
    }
}
