/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.view;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.widget.FrameLayout;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.ViroGvrLayout;

/**
 * SceneNavigator manages the various scenes that a Viro App can navigate between.
 */
public class SceneNavigator extends FrameLayout {
    private static final String TAG = SceneNavigator.class.getSimpleName();
    private ViroGvrLayout mViroGvrLayout;

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
        Log.d(TAG, "Adding a view! " + child.getClass().getSimpleName() + " at: " + index);
    }
}
