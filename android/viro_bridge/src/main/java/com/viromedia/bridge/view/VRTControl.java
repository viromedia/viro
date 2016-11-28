/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

import com.viro.renderer.jni.NodeJni;

/**
 * VRTControl is inherited from all Viro UI controls that
 * contains a native Node {@link com.viro.renderer.jni.NodeJni}.
 */
public class VRTControl extends View {
    protected final NodeJni mNode;

    public VRTControl(Context context) {
        this(context, null, -1, -1);
    }

    public VRTControl(Context context, AttributeSet attrs) {
        this(context, attrs, -1, -1);
    }

    public VRTControl(Context context, AttributeSet attrs, int defStyleAttr) {
        this(context, attrs, defStyleAttr, -1);
    }

    public VRTControl(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        mNode = new NodeJni(context);
    }

    public NodeJni getNode(){
        return mNode;
    }

}
