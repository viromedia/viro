/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.FrameLayout;

import com.viro.renderer.jni.NodeJni;

/**
 * Node is inherited from all container views (any view that has at least
 * one child view, whether it be of type VRTControl or VRTGroup). It also contains
 * a node to which all child views are parented to.
 */
public class Node extends FrameLayout {
    private final NodeJni mNodeJni;

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

    @Override
    public void addView(View child, int index) {
        super.addView(child, index);
        final Node childNode = (Node)child;
        mNodeJni.addChildNode(childNode.mNodeJni);
    }

    @Override
    public void removeView(View child) {
        super.addView(child);
        final Node childNode = (Node)child;
        mNodeJni.removeChildNode(childNode.mNodeJni);
    }
}
