/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.module;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;


public class AnimationManager extends ReactContextBaseJavaModule {

    public AnimationManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTAnimationManager";
    }

    @ReactMethod
    public void setJSAnimations(ReadableMap animationMap) {
        
    }

}
