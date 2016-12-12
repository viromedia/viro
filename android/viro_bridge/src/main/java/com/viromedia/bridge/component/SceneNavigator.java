/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.FrameLayout;
import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.RenderContextJni;
import com.viro.renderer.jni.ViroGvrLayout;
import com.viro.renderer.jni.VrView;
import com.viromedia.bridge.component.node.Scene;
import com.viromedia.bridge.utility.ViroLog;

import java.util.ArrayList;

/**
 * SceneNavigator manages the various scenes that a Viro App can navigate between.
 *
 * TODO:
 * Add Scene Navigation support! This only supports adding one view and does
 * not take into consideration the logic needed for multiple scene handling.
 */
public class SceneNavigator extends FrameLayout {
    private static final String TAG = ViroLog.getTag(SceneNavigator.class);

    /**
     * View containing our renderer
     */
    private VrView mVrView;

    /**
     * Currently rendered scene
     */
    private int mSelectedSceneIndex = 0;

    /**
     * Array of scenes given by the bridge for the renderer to switch to.
     */
    private final ArrayList<Scene> mSceneArray = new ArrayList<Scene>();

    /**
     * Context passed around to views to get render specific information.
     */
    private RenderContextJni mRenderContext;

    public SceneNavigator(ReactApplicationContext reactContext) {
        this(reactContext.getBaseContext(), null, -1);

        mVrView = new ViroGvrLayout(reactContext.getCurrentActivity());
        addView((ViroGvrLayout)mVrView);
        mRenderContext = mVrView.getRenderContextRef();
    }

    public SceneNavigator(Context context) {
        this(context, null, -1);
    }

    public SceneNavigator(Context context, AttributeSet attrs) {
        this(context, attrs, -1);
    }

    public SceneNavigator(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    public void addView(View child, int index) {
        if (child instanceof ViroGvrLayout) {
            // only add a view to the childViews if it's a ViroGvrLayout. This function is called
            // by the single argument addView(child) method.
            super.addView(child, index);
            return;
        } else if (!(child instanceof Scene)){
            ViroLog.error(TAG, "Attempted to add a non-scene element [" + child.getClass().getSimpleName()
                    + "] to SceneNavigator!");
            return;
        }

        if (!(child instanceof Scene)){
            throw new IllegalArgumentException("Attempted to add a non-scene element to SceneNavigator");
        }

        Scene childScene = (Scene)child;
        childScene.setRenderContext(mRenderContext);
        childScene.setScene(childScene);
        childScene.setNativeRenderer(mVrView.getNativeRenderer());
        mSceneArray.add(index, childScene);
        if (mSelectedSceneIndex == index){
            mVrView.setScene(mSceneArray.get(mSelectedSceneIndex).getNativeScene());
        }
    }

    @Override
    public void removeView(View view){
        if (view instanceof ViroGvrLayout){
            super.removeView(view);
        } else if (view instanceof Scene) {
            // TODO: also remove Scene from mViroGvrLayout if it's the current scene too.
            mSceneArray.remove(view);
        }
    }

    public void setVrModeEnabled(boolean vrModeEnabled) {
        mVrView.setVrModeEnabled(vrModeEnabled);
    }
}
