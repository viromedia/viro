/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.content.Context;
import android.util.AttributeSet;
import com.viro.renderer.jni.SceneJni;

public class Scene extends Node {
    private static final String TAG = Scene.class.getSimpleName();
    private final SceneJni mNativeScene;

    public Scene(Context context) {
        this(context, null, -1 -1);
    }

    public Scene(Context context, AttributeSet attrs) {
        this(context, attrs, -1 -1);
    }

    public Scene(Context context, AttributeSet attrs, int defStyleAttr) {
        this(context, attrs, defStyleAttr, -1);
    }

    public Scene(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        mNativeScene = new SceneJni(getNodeJni());
    }

    public SceneJni getNativeScene(){
        return mNativeScene;
    }
}
