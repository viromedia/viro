/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;


import com.facebook.react.bridge.ReactContext;
import com.viro.core.Camera;
import com.viro.core.Node;
import com.viro.core.Vector;

public class VRTCamera extends VRTControl {
    protected static final float DEFAULT_FOV = 0;
    private float mFieldOfView = DEFAULT_FOV;
    protected Camera mNativeCamera;

    public VRTCamera(ReactContext context) {
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

    public void setFieldOfView(float fov){
        mFieldOfView = fov;
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
        mNativeCamera.setFieldOfView(mFieldOfView);
        addToNode(getNodeJni());
    }
}
