/**
 * Copyright @2016 ViroMedia. All rights reserved.
 */
package com.viromedia.bridge.view;

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
public class SceneNavigator extends FrameLayout {
    private ViroGvrLayout mViroGvrLayout;

    public SceneNavigator(ReactApplicationContext reactContext){
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
}
