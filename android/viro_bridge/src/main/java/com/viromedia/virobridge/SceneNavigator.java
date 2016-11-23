/**
 * Copyright @2016 ViroMedia. All rights reserved.
 */
package com.viromedia.virobridge;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.os.Bundle;
import android.util.AttributeSet;
import android.widget.FrameLayout;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.ViroGvrLayout;

/**
 * SceneNavigator manages the various scenes that a Viro App can navigate between.
 */
public class SceneNavigator extends FrameLayout implements Application.ActivityLifecycleCallbacks {
    private ViroGvrLayout mViroGvrLayout;

    public SceneNavigator(ReactApplicationContext reactContext){
        this(reactContext.getBaseContext(), null, -1);

        Application app = (Application )reactContext.getApplicationContext();
        app.registerActivityLifecycleCallbacks(this);

        mViroGvrLayout = new ViroGvrLayout(reactContext.getCurrentActivity());
        addView(mViroGvrLayout);

        /**
         * Call onCreate() here for the gvrLayout as the Activity has
         * already progressed beyond onCreate and is within onResume
         * by the time we construct a SceneNavigator.
         */
        mViroGvrLayout.onCreate(null);
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
    public void onActivityCreated(Activity activity, Bundle bundle) {
        mViroGvrLayout.onCreate(null);
    }

    @Override
    public void onActivityStarted(Activity activity) {
        //No-op
    }

    @Override
    public void onActivityResumed(Activity activity) {
        mViroGvrLayout.onResume();
    }

    @Override
    public void onActivityPaused(Activity activity){
        mViroGvrLayout.onPause();
    }

    @Override
    public void onActivityStopped(Activity activity) {
        //No-op
    }

    @Override
    public void onActivitySaveInstanceState(Activity activity, Bundle outState) {
        //No-op
    }

    @Override
    public void onActivityDestroyed(Activity activity) {
        mViroGvrLayout.onDestroy();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        mViroGvrLayout.onWindowFocusChanged(hasFocus);
    }

}
