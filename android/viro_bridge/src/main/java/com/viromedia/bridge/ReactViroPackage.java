/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge;

import com.facebook.react.ReactPackage;
import com.facebook.react.bridge.JavaScriptModule;
import com.facebook.react.bridge.NativeModule;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ViewManager;
import com.viromedia.bridge.view.SceneManager;
import com.viromedia.bridge.view.BoxManager;
import com.viromedia.bridge.view.SceneNavigatorManager;

import java.util.Arrays;
import java.util.List;

import static java.util.Collections.emptyList;

/**
 * ReactViroPackage class containing an array of all ViroViewManagers to be created.
 */
public class ReactViroPackage implements ReactPackage {
    @Override
    public List<NativeModule> createNativeModules(ReactApplicationContext reactContext) {
        return emptyList();
    }

    @Override
    public List<Class<? extends JavaScriptModule>> createJSModules() {
        return emptyList();
    }

    @Override
    public List<ViewManager> createViewManagers(ReactApplicationContext reactContext) {
        return Arrays.<ViewManager>asList(
                new SceneNavigatorManager(reactContext),
                new SceneManager(reactContext),
                new BoxManager(reactContext));
    }
}
