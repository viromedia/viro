/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge;

import com.facebook.react.ReactPackage;
import com.facebook.react.bridge.JavaScriptModule;
import com.facebook.react.bridge.NativeModule;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ViewManager;
import com.viromedia.bridge.component.AnimatedComponentManager;
import com.viromedia.bridge.component.ControllerManager;
import com.viromedia.bridge.component.Image360Manager;
import com.viromedia.bridge.component.AmbientLightManager;
import com.viromedia.bridge.component.DirectionalLightManager;
import com.viromedia.bridge.component.OmniLightManager;
import com.viromedia.bridge.component.SkyBoxManager;
import com.viromedia.bridge.component.SoundFieldManager;
import com.viromedia.bridge.component.SoundManager;
import com.viromedia.bridge.component.SpatialSoundManager;
import com.viromedia.bridge.component.SpotLightManager;
import com.viromedia.bridge.component.ar.ARNavigatorManager;
import com.viromedia.bridge.component.node.control.Object3dManager;
import com.viromedia.bridge.component.node.control.PolylineManager;
import com.viromedia.bridge.component.node.control.TextManager;
import com.viromedia.bridge.component.Video360Manager;
import com.viromedia.bridge.component.node.control.CameraManager;
import com.viromedia.bridge.component.node.control.OrbitCameraManager;
import com.viromedia.bridge.component.node.FlexViewManager;
import com.viromedia.bridge.component.node.NodeContainerManager;
import com.viromedia.bridge.component.node.SceneManager;
import com.viromedia.bridge.component.node.control.BoxManager;
import com.viromedia.bridge.component.SceneNavigatorManager;
import com.viromedia.bridge.component.node.control.SphereManager;
import com.viromedia.bridge.component.node.control.ImageManager;
import com.viromedia.bridge.component.node.control.SurfaceManager;
import com.viromedia.bridge.component.node.control.VideoSurfaceManager;
import com.viromedia.bridge.module.AnimationManager;
import com.viromedia.bridge.module.CameraModule;
import com.viromedia.bridge.module.ControllerModule;
import com.viromedia.bridge.module.MaterialManager;
import com.viromedia.bridge.module.NodeModule;
import com.viromedia.bridge.module.SceneModule;
import com.viromedia.bridge.module.SceneNavigatorModule;
import com.viromedia.bridge.module.PerfMonitor;
import com.viromedia.bridge.module.SoundModule;

import java.util.Arrays;
import java.util.List;

import static java.util.Collections.emptyList;

/**
 * ReactViroPackage class containing an array of all ViroViewManagers to be created.
 */
public class ReactViroPackage implements ReactPackage {
    public static final String ON_EXIT_VIRO_BROADCAST ="com.viromedia.bridge.broadcast.OnExitViro";

    public enum ViroPlatform {
        GVR, OVR_MOBILE
    }

    private final ViroPlatform mViroPlatform;

    public ReactViroPackage(ViroPlatform platform) {
        mViroPlatform = platform;
    }

    @Override
    public List<NativeModule> createNativeModules(ReactApplicationContext reactContext) {
        return Arrays.<NativeModule>asList(
                new MaterialManager(reactContext),
                new AnimationManager(reactContext),
                new CameraModule(reactContext),
                new SoundModule(reactContext),
                new SceneNavigatorModule(reactContext),
                new PerfMonitor(reactContext),
                new ControllerModule(reactContext),
                new NodeModule(reactContext),
                new SceneModule(reactContext)
        );
    }

    @Override
    public List<Class<? extends JavaScriptModule>> createJSModules() {
        return emptyList();
    }

    @Override
    public List<ViewManager> createViewManagers(ReactApplicationContext reactContext) {
        return Arrays.<ViewManager>asList(
                new SceneNavigatorManager(reactContext, mViroPlatform),
                new SceneManager(reactContext),
                new BoxManager(reactContext),
                new VideoSurfaceManager(reactContext),
                new Video360Manager(reactContext),
                new NodeContainerManager(reactContext),
                new CameraManager(reactContext),
                new OrbitCameraManager(reactContext),
                new SphereManager(reactContext),
                new ImageManager(reactContext),
                new Image360Manager(reactContext),
                new SkyBoxManager(reactContext),
                new FlexViewManager(reactContext),
                new AnimatedComponentManager(reactContext),
                new SurfaceManager(reactContext),
                new FlexViewManager(reactContext),
                new DirectionalLightManager(reactContext),
                new AmbientLightManager(reactContext),
                new SpotLightManager(reactContext),
                new OmniLightManager(reactContext),
                new SoundManager(reactContext),
                new SoundFieldManager(reactContext),
                new SpatialSoundManager(reactContext),
                new OmniLightManager(reactContext),
                new TextManager(reactContext),
                new Object3dManager(reactContext),
                new ControllerManager(reactContext),
                new PolylineManager(reactContext),
                // AR Components
                new ARNavigatorManager(reactContext)
        );
    }
}
