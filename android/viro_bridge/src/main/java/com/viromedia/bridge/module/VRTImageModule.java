/*
 * Copyright © 2018 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.module;


import android.content.Context;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.module.annotations.ReactModule;
import com.viromedia.bridge.utility.ImageDownloader;

@ReactModule(name = "VRTImageModule")
public class VRTImageModule extends ReactContextBaseJavaModule {

    private final Context mContext;

    @Override
    public String getName() {
        return "VRTImageModule";
    }

    public VRTImageModule(ReactApplicationContext context) {
        super(context);
        mContext = context;
    }

    @ReactMethod
    public void evictFromCache(ReadableMap map) {
        ImageDownloader.evictFromCache(map, mContext);
    }
}
