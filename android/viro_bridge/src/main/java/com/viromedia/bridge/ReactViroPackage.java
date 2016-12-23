/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge;

import com.facebook.react.ReactPackage;
import com.facebook.react.bridge.JavaScriptModule;
import com.facebook.react.bridge.NativeModule;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ViewManager;
import com.viromedia.bridge.component.Photo360Manager;
import com.viromedia.bridge.component.SkyBoxManager;
import com.viromedia.bridge.component.Video360Manager;
import com.viromedia.bridge.component.CameraManager;
import com.viromedia.bridge.component.OrbitCameraManager;
import com.viromedia.bridge.component.node.FlexViewManager;
import com.viromedia.bridge.component.node.NodeContainerManager;
import com.viromedia.bridge.component.node.SceneManager;
import com.viromedia.bridge.component.node.control.BoxManager;
import com.viromedia.bridge.component.SceneNavigatorManager;
import com.viromedia.bridge.component.node.control.SphereManager;
import com.viromedia.bridge.component.node.control.ImageManager;
import com.viromedia.bridge.component.node.control.VideoSurfaceManager;
import com.viromedia.bridge.module.MaterialManager;

import java.util.Arrays;
import java.util.List;

import static java.util.Collections.emptyList;

/**
 * ReactViroPackage class containing an array of all ViroViewManagers to be created.
 */
public class ReactViroPackage implements ReactPackage {
    @Override
    public List<NativeModule> createNativeModules(ReactApplicationContext reactContext) {
        return Arrays.<NativeModule>asList(
                new MaterialManager(reactContext)
        );
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
                new BoxManager(reactContext),
                new VideoSurfaceManager(reactContext),
                new Video360Manager(reactContext),
                new NodeContainerManager(reactContext),
                new CameraManager(reactContext),
                new OrbitCameraManager(reactContext),
                new SphereManager(reactContext),
                new ImageManager(reactContext),
                new Photo360Manager(reactContext),
                new SkyBoxManager(reactContext),
                new FlexViewManager(reactContext)
        );
    }
}
