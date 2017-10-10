/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.Camera;
import com.viro.renderer.jni.Node;

public class VRTCamera extends VRTControl {
    protected Camera mNativeCamera;
    private float[] mPosition;

    public VRTCamera(ReactApplicationContext context) {
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

    public void addToNode(Node node) {
        mNativeCamera.addToNode(node);
    }

    public void removeFromNode(Node node) {
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
            mNativeCamera = new Camera();
        }
        mNativeCamera.setPosition(mPosition);
        mNativeCamera.setRotationType(getRotationType());
        addToNode(getNodeJni());
    }
}
