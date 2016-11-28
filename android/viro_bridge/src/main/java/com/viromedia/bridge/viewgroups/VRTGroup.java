/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.viewgroups;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.FrameLayout;

import com.viro.renderer.jni.NodeJni;
import com.viromedia.bridge.view.VRTControl;

/**
 * VRTGroup is inherited from all container views (any view that has at least
 * one child view, whether it be of type VRTControl or VRTGroup). It also contains
 * a node to which all child views are parented to.
 */
public class VRTGroup extends FrameLayout {
    protected final NodeJni mNode;

    public VRTGroup(Context context) {
        this(context, null, -1, -1);
    }

    public VRTGroup(Context context, AttributeSet attrs) {
        this(context, attrs, -1, -1);
    }

    public VRTGroup(Context context, AttributeSet attrs, int defStyleAttr) {
        this(context, attrs, defStyleAttr, -1);
    }

    public VRTGroup(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        mNode = new NodeJni(context);
    }

    @Override
    public void addView(View child, int index) {
        super.addView(child, index);
        final VRTControl vrtControl = (VRTControl)child;
        mNode.addChildNode(vrtControl.getNode());
    }

    @Override
    public void removeView(View child) {
        super.addView(child);
        final VRTControl vrtControl = (VRTControl)child;
        mNode.removeChildNode(vrtControl.getNode());
    }
}
