/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

import com.viro.renderer.jni.NodeJni;

/**
 * Control is inherited by all Viro UI controls that
 * contains a native Node {@link com.viro.renderer.jni.NodeJni}.
 */
public class Control extends Node {

    public Control(Context context) {
        this(context, null, -1, -1);
    }

    public Control(Context context, AttributeSet attrs) {
        this(context, attrs, -1, -1);
    }

    public Control(Context context, AttributeSet attrs, int defStyleAttr) {
        this(context, attrs, defStyleAttr, -1);
    }

    public Control(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    @Override
    public void addView(View child, int index) {
        // TODO: show red screen
    }

    @Override
    public void removeView(View child) {
        // TODO: show red screen
    }
}
