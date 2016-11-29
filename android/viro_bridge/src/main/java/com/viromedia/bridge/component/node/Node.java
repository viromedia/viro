/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.FrameLayout;

import com.viro.renderer.jni.NodeJni;
import com.viromedia.bridge.component.AnimatedComponent;
import com.viromedia.bridge.component.Component;
import com.viromedia.bridge.component.Light;

import java.util.ArrayList;

/**
 * Node is inherited by any component which is represented by a VRONode in native
 */
public class Node extends Component {

    private final NodeJni mNodeJni;

    protected ArrayList<String> materials;
    protected double[] position;
    protected double[] rotation;
    protected double[] scale;
    protected double opacity;
    protected ArrayList<String> transformBehaviors;
    protected boolean visible;

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
        super.addView(child);

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
}
