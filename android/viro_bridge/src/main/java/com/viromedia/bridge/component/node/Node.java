/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

import com.viro.renderer.jni.BaseGeometry;
import com.viro.renderer.jni.MaterialJni;
import com.viro.renderer.jni.NodeJni;
import com.viromedia.bridge.component.AnimatedComponent;
import com.viromedia.bridge.component.Component;
import com.viromedia.bridge.component.Light;

import java.util.List;

/**
 * Node is inherited by any component which is represented by a VRONode in native
 */
public class Node extends Component {
    private NodeJni mNodeJni;
    protected float[] mPosition;
    protected float[] mRotation;
    protected float[] mScale;
    protected float mOpacity;
    protected boolean mVisible;
    protected List<MaterialJni> mMaterials;

    // TODO: Handle Event, Flex box properties, and TransformBehaviors

    public Node(Context context) {
        this(context, null, -1, -1);
    }

    public Node(Context context, AttributeSet attrs) {
        this(context, attrs, -1, -1);
    }

    public Node(Context context, AttributeSet attrs, int defStyleAttr) {
        this(context, attrs, defStyleAttr, -1);
    }

    public Node(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        mNodeJni = new NodeJni(context);
    }

    public NodeJni getNodeJni(){
        return mNodeJni;
    }

    // TODO: handle children separate from android viewgroup childViews
    @Override
    public void addView(View child, int index) {
        super.addView(child, index);

        if (child instanceof Light) {
            // TODO: Add a light
        } else if (child instanceof Node) {
            final Node childNode = (Node) child;
            mNodeJni.addChildNode(childNode.mNodeJni);
        } else if (child instanceof AnimatedComponent) {
            // TODO: handle AnimatedComponents
        } else {
            // TODO: Throw Error? Red Screen?
        }
    }

    @Override
    public void removeView(View child) {
        super.removeView(child);

        if (child instanceof Light) {
            // TODO: Add a light
        } else if (child instanceof Node) {
            final Node childNode = (Node) child;
            mNodeJni.removeChildNode(childNode.mNodeJni);
        } else if (child instanceof AnimatedComponent) {
            // TODO: handle AnimatedComponents
        } else {
            // TODO: Throw Error? Red Screen?
        }
    }

    protected void setPosition(float[] position){
        if (position.length != 3){
            throw new IllegalArgumentException("Missing a position value: All three " +
                    "[x,y,z] axis value are needed.");
        }

        mPosition = position;
        mNodeJni.setPosition(position);
    }

    protected void setRotation(float[] rotation){
        if (rotation.length != 3){
            throw new IllegalArgumentException("Missing a rotation value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mRotation = rotation;
        mNodeJni.setRotation(rotation);
    }

    protected void setScale(float[] scale){
        if (scale.length != 3){
            throw new IllegalArgumentException("Missing a scale value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mScale = scale;
        mNodeJni.setScale(scale);
    }

    protected void setOpacity(float opacity){
        mOpacity = opacity;
        mNodeJni.setOpacity(opacity);
    }

    protected void setVisible(boolean visible){
        mVisible = visible;
        mNodeJni.setVisible(visible);
    }

    protected void setGeometry(BaseGeometry geometry){
        mNodeJni.setGeometry(geometry);
        if (mMaterials != null) {
            mNodeJni.setMaterials(mMaterials);
        }
    }

    protected void setMaterials(List<MaterialJni> materials) {
        mMaterials = materials;
        mNodeJni.setMaterials(materials);
    }

    protected void setTransformBehaviors(String[] transformBehaviors) {
        mNodeJni.setTransformBehaviors(transformBehaviors);
    }
}
