/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.Camera;
import com.viro.renderer.jni.Node;
import com.viro.renderer.jni.Vector;

public class VRTCamera extends VRTControl {
    protected Camera mNativeCamera;
    private float[] mPosition;

    public VRTCamera(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public void onTearDown(){
        if (mNativeCamera != null){
            mNativeCamera.dispose();
            mNativeCamera = null;
        }
        super.onTearDown();
    }

    public void addToNode(Node node) {
        node.setCamera(mNativeCamera);
    }

    public void removeFromNode(Node node) {
        if (node.getCamera() == mNativeCamera) {
            node.setCamera(null);
        }
    }

    public void setPosition(float[] position) {
        mPosition = position;
    }

    public float[] getPosition() {
        return mPosition;
    }

    public Camera.RotationType getRotationType() {
        return Camera.RotationType.STANDARD;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();

        if (mNativeCamera == null) {
            mNativeCamera = new Camera();
        }
        mNativeCamera.setPosition(new Vector(mPosition));
        mNativeCamera.setRotationType(getRotationType());
        addToNode(getNodeJni());
    }
}
