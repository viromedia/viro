/**
 * Copyright @2016 ViroMedia. All rights reserved.
 */
package com.viromedia.virobridge;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.graphics.Point;
import android.os.Bundle;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.View;
import android.widget.FrameLayout;

import com.facebook.react.bridge.LifecycleEventListener;
import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.ViroGvrLayout;

/**
 * SceneNavigator manages the various scenes that a Viro App can navigate between.
 */
public class SceneNavigator extends FrameLayout implements LifecycleEventListener,Application.ActivityLifecycleCallbacks {
    private ViroGvrLayout mViroGvrLayout;
    private Point mScreenSize;
    private DisplayMetrics mMetrics;

    public SceneNavigator(ReactApplicationContext reactContext){
        this(reactContext.getBaseContext(), null, -1);
        reactContext.addLifecycleEventListener(this);
        Application app = (Application)reactContext.getApplicationContext();
        app.registerActivityLifecycleCallbacks(this);

        mViroGvrLayout = new ViroGvrLayout(reactContext.getCurrentActivity());

        LayoutParams rlp = new LayoutParams(
                LayoutParams.MATCH_PARENT,
                LayoutParams.MATCH_PARENT
        );

        Display screenDisplay = reactContext.getCurrentActivity().getWindowManager().getDefaultDisplay();
        mScreenSize = new Point();
        screenDisplay.getSize(mScreenSize);
        mMetrics = app.getResources().getDisplayMetrics();

       // this.setLayoutParams(rlp);
       this.addView(mViroGvrLayout, rlp);
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
    public void onHostResume() {
        mViroGvrLayout.onCreate(null);
        mViroGvrLayout.onResume();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec){
        int parentWidth = MeasureSpec.getSize(widthMeasureSpec);
        int parentHeight = MeasureSpec.getSize(heightMeasureSpec);
        super.onMeasure(parentWidth,parentHeight);

        LayoutParams parms = new LayoutParams(mMetrics.widthPixels, mMetrics.heightPixels);

        this.setLayoutParams(parms);
        this.setMeasuredDimension(mMetrics.widthPixels, mMetrics.heightPixels);
      //  super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }

    @Override
    public void onHostPause() {
        //No-op
    }

    @Override
    public void onHostDestroy() {
        //No-op
    }

    @Override
    public void onActivityCreated(Activity activity, Bundle bundle) {
        //No-op
    }

    @Override
    public void onActivityStarted(Activity activity) {
        //No-op
    }

    @Override
    public void onActivityResumed(Activity activity) {
        //No-op
    }

    @Override
    public void onActivityPaused(Activity activity) {
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
