/**
 * Copyright © 2018 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.module;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;

public class ARTrackingTargetsModule extends ReactContextBaseJavaModule {


    public ARTrackingTargetsModule(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    public String getName() {
        return "VRTARTrackingTargetsModule";
    }

    @ReactMethod
    public void createTargets(final ReadableMap targetsMap) {
        // no-op
    }

    @ReactMethod
    public void deleteTarget(final String targetName) {
        // no-op
    }
}
