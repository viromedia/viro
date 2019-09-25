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


import com.facebook.react.bridge.ReactContext;
import com.viro.core.Camera;
import com.viro.core.Node;
import com.viro.core.Vector;
import com.viromedia.bridge.component.node.VRTNode;
import com.viromedia.bridge.utility.Helper;

public class VRTCamera extends VRTNode {
    protected static final float DEFAULT_FOV = 0;
    private float mFieldOfView = DEFAULT_FOV;
    protected Camera mNativeCamera;
    protected VRTNode mNodeRootTransformCamera;

    public VRTCamera(ReactContext context) {
        super(context);
        mNodeRootTransformCamera = new VRTNode(getReactContext());
        mNativeCamera = new Camera();

        // set the node to copy camera head rotation to.
        mNativeCamera.setRefNodeToCopyRotation(getNodeJni());

        //add the camera to the root node.
        mNodeRootTransformCamera.getNodeJni().setCamera(mNativeCamera);
        //now add this base node of VRTNode as the a child of the root camera node.
        mNodeRootTransformCamera.getNodeJni().addChildNode(getNodeJni());
        //change the animations to affect the root camera node.
        mNodeAnimation.setNode(mNodeRootTransformCamera);
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

        if(mNodeRootTransformCamera == null) {
            mNodeRootTransformCamera = new VRTNode(getReactContext());
        }

        mNativeCamera.setPosition(new Vector(mPosition));
        mNativeCamera.setRotationType(getRotationType());
        mNativeCamera.setFieldOfView(mFieldOfView);
        mNativeCamera.setRotation(Helper.toRadiansVector(mRotation));

    }

    public VRTNode getNodeRootTransformCamera() {
        return mNodeRootTransformCamera;
    }
}
