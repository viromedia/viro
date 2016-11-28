/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.viewgroups;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.widget.FrameLayout;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.ViroGvrLayout;

import java.util.ArrayList;

/**
 * SceneNavigator manages the various scenes that a Viro App can navigate between.
 *
 * TODO:
 * Add Scene Navigation support! This only supports adding one view and does
 * not take into consideration the logic needed for multiple scene handling.
 */
public class SceneNavigator extends FrameLayout {
    private static final String TAG = SceneNavigator.class.getSimpleName();

    /**
     * Layout containing our renderer
     */
    private ViroGvrLayout mViroGvrLayout;

    /**
     * Currently rendered scene
     */
    private int mSelectedSceneIndex = 0;

    /**
     * Array of scenes given by the bridge for the renderer to switch to.
     */
    private final ArrayList<Scene> mSceneArray = new ArrayList<Scene>();

    public SceneNavigator(ReactApplicationContext reactContext) {
        this(reactContext.getBaseContext(), null, -1);

        mViroGvrLayout = new ViroGvrLayout(reactContext.getCurrentActivity(), true);
        addView(mViroGvrLayout);
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
    public void addView(View child) {
        // This is called by us in the constructor
        super.addView(child);
        Log.d(TAG, "Adding a view! " + child.getClass().getSimpleName());
    }

    @Override
    public void addView(View child, int index) {
        // This is how react adds child views
        super.addView(child, index);

        if (!(child instanceof Scene)){
            Log.e(TAG, "Attempted to add a non-scene element to SceneNavigator!");
            return;
        }

        Scene childScene = (Scene)child;
        mSceneArray.add(index, childScene);
        if (mSelectedSceneIndex == index){
            mViroGvrLayout.setScene(mSceneArray.get(mSelectedSceneIndex).getNativeScene());
        }
    }

    @Override
    public void removeView(View view){
        mViroGvrLayout.removeView(view);
        super.removeView(view);
    }
}
