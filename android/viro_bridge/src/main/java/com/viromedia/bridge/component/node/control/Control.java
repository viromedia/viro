/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viromedia.bridge.component.node.Node;

/**
 * Control is inherited by all Viro UI controls that
 * contains a native Node {@link com.viro.renderer.jni.NodeJni}.
 */
public class Control extends Node {

    public Control(ReactApplicationContext reactContext) {
        this(reactContext.getBaseContext(), null, -1, -1, reactContext);
    }

    public Control(Context context, AttributeSet attrs, int defStyleAttr,
                     int defStyleRes, ReactApplicationContext reactContext) {
        super(context, attrs, defStyleAttr, defStyleRes, reactContext);
    }

    @Override
    public void addView(View child, int index) {
        throw new IllegalStateException("Can't add child. Controls should not have children views.");
    }

    @Override
    public void removeView(View child) {
        throw new IllegalStateException("Can't remove child. Controls should not have children views.");
    }
}
