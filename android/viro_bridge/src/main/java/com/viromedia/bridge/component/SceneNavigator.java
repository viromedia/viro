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
    private int mSelectedSceneIndex = -1;

    /**
     * Array of scenes given by the bridge for the renderer to switch to.
     */
    private final ArrayList<Scene> mSceneArray = new ArrayList<Scene>();

    /**
     * Context passed around to views to get render specific information.
     */
    private RenderContextJni mRenderContext;

    private boolean mViewAdded = false;
    private boolean mGLInialized = false;
    public SceneNavigator(ReactApplicationContext reactContext) {
        this(reactContext.getBaseContext(), null, -1);

        mVrView = new ViroGvrLayout(reactContext.getCurrentActivity(),
                new ViroGvrLayout.GlListener() {
            @Override
            public void onGlInitialized() {
                mGLInialized = true;
                setRenderContext();
            }
        });
        ViroGvrLayout gvrLayout = (ViroGvrLayout)mVrView;
        addView(gvrLayout);
        mRenderContext = mVrView.getRenderContextRef();
        /*
         * Trigger VrView's onActivityStarted and onActivityResumed of the vrView as
         * React creates it's views within the activity's onResume().
         */
        gvrLayout.onActivityStarted(reactContext.getCurrentActivity());
        gvrLayout.onActivityResumed(reactContext.getCurrentActivity());
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
        mSceneArray.add(index, childScene);

        // Adding the scene view can occur after the prop type is set on the bridge.
        // Thus, refresh the selection of the current scene as needed.
        if (index == mSelectedSceneIndex){
            setCurrentSceneIndex(mSelectedSceneIndex);
        }

        mViewAdded = true;
        // In case gl was initialized before views were added.
        setRenderContext();
    }

    private void setRenderContext() {
        if (mViewAdded && mGLInialized) {
            Scene childScene = mSceneArray.get(mSelectedSceneIndex);
            childScene.setRenderContext(mRenderContext);
            childScene.setScene(childScene);
            childScene.setNativeRenderer(mVrView.getNativeRenderer());
        }
    }

    public void setCurrentSceneIndex(int index){
        mSelectedSceneIndex = index;

        if (index < 0 || index >= mSceneArray.size()){
            // Scene object may not yet have been initialized, so return here.
            return;
        }
        mVrView.setScene(mSceneArray.get(mSelectedSceneIndex).getNativeScene());
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
