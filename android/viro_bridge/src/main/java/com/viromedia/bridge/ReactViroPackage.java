/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge;

import com.facebook.react.ReactPackage;
import com.facebook.react.bridge.NativeModule;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ViewManager;

import com.viromedia.bridge.component.VRTAnimatedComponentManager;
import com.viromedia.bridge.component.VRTControllerManager;
import com.viromedia.bridge.component.VRT360ImageManager;
import com.viromedia.bridge.component.VRTAmbientLightManager;
import com.viromedia.bridge.component.VRTDirectionalLightManager;
import com.viromedia.bridge.component.VRTLightingEnvironmentManager;
import com.viromedia.bridge.component.VRTOmniLightManager;
import com.viromedia.bridge.component.VRTSkyBoxManager;
import com.viromedia.bridge.component.VRTSoundFieldManager;
import com.viromedia.bridge.component.VRTSoundManager;
import com.viromedia.bridge.component.VRTSpatialSoundManager;
import com.viromedia.bridge.component.VRTSpotLightManager;
import com.viromedia.bridge.component.node.VRTARImageMarkerManager;
import com.viromedia.bridge.component.node.VRTARPlaneManager;
import com.viromedia.bridge.component.node.VRTARSceneManager;
import com.viromedia.bridge.component.node.VRTSceneManagerImpl;
import com.viromedia.bridge.component.node.control.VRT3DObjectManager;
import com.viromedia.bridge.component.node.control.VRTParticleEmitterManager;
import com.viromedia.bridge.component.node.control.VRTPolylineManager;
import com.viromedia.bridge.component.node.control.VRTTextManager;
import com.viromedia.bridge.component.VRT360VideoManager;
import com.viromedia.bridge.component.node.control.VRTCameraManager;
import com.viromedia.bridge.component.node.control.VRTOrbitCameraManager;
import com.viromedia.bridge.component.node.VRTFlexViewManager;
import com.viromedia.bridge.component.node.VRTNodeManagerImpl;
import com.viromedia.bridge.component.node.control.VRTBoxManager;
import com.viromedia.bridge.component.VRTSceneNavigatorManager;
import com.viromedia.bridge.component.VRTARSceneNavigatorManager;
import com.viromedia.bridge.component.node.control.VRTSphereManager;
import com.viromedia.bridge.component.node.control.VRTImageManager;
import com.viromedia.bridge.component.node.control.VRTSurfaceManager;
import com.viromedia.bridge.component.node.control.VRTVideoSurfaceManager;
import com.viromedia.bridge.component.node.VRTPortalSceneManager;
import com.viromedia.bridge.component.node.VRTPortalManager;


import com.viromedia.bridge.module.ARSceneModule;
import com.viromedia.bridge.module.ARSceneNavigatorModule;
import com.viromedia.bridge.module.ARTrackingTargetsModule;
import com.viromedia.bridge.module.AnimationManager;
import com.viromedia.bridge.module.CameraModule;
import com.viromedia.bridge.module.ControllerModule;
import com.viromedia.bridge.module.MaterialManager;
import com.viromedia.bridge.module.NodeModule;
import com.viromedia.bridge.module.SceneModule;
import com.viromedia.bridge.module.SceneNavigatorModule;
import com.viromedia.bridge.module.PerfMonitor;
import com.viromedia.bridge.module.SoundModule;
import com.viromedia.bridge.module.VRTImageModule;

import java.util.Arrays;
import java.util.List;

/**
 * ReactViroPackage class containing an array of all ViroViewManagers to be created.
 */
public class ReactViroPackage implements ReactPackage {
    public static final String ON_EXIT_VIRO_BROADCAST ="com.viromedia.bridge.broadcast.OnExitViro";

    public enum ViroPlatform {
        GVR, OVR_MOBILE, AR
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
                new SceneModule(reactContext),
                new VRTImageModule(reactContext),
                new ARSceneModule(reactContext),
                new ARSceneNavigatorModule(reactContext),
                new ARTrackingTargetsModule(reactContext)
        );
    }

    @Override
    public List<ViewManager> createViewManagers(ReactApplicationContext reactContext) {
        return Arrays.<ViewManager>asList(
                new VRTSceneNavigatorManager(reactContext, mViroPlatform),
                new VRTSceneManagerImpl(reactContext),
                new VRTBoxManager(reactContext),
                new VRTVideoSurfaceManager(reactContext),
                new VRT360VideoManager(reactContext),
                new VRTNodeManagerImpl(reactContext),
                new VRTCameraManager(reactContext),
                new VRTOrbitCameraManager(reactContext),
                new VRTSphereManager(reactContext),
                new VRTImageManager(reactContext),
                new VRT360ImageManager(reactContext),
                new VRTSkyBoxManager(reactContext),
                new VRTFlexViewManager(reactContext),
                new VRTAnimatedComponentManager(reactContext),
                new VRTSurfaceManager(reactContext),
                new VRTFlexViewManager(reactContext),
                new VRTDirectionalLightManager(reactContext),
                new VRTAmbientLightManager(reactContext),
                new VRTSpotLightManager(reactContext),
                new VRTOmniLightManager(reactContext),
                new VRTSoundManager(reactContext),
                new VRTSoundFieldManager(reactContext),
                new VRTSpatialSoundManager(reactContext),
                new VRTOmniLightManager(reactContext),
                new VRTTextManager(reactContext),
                new VRT3DObjectManager(reactContext),
                new VRTControllerManager(reactContext),
                new VRTPolylineManager(reactContext),
                new VRTParticleEmitterManager(reactContext),
                new VRTPortalSceneManager(reactContext),
                new VRTPortalManager(reactContext),
                new VRTLightingEnvironmentManager(reactContext),
                // AR Components
                new VRTARSceneNavigatorManager(reactContext),
                new VRTARSceneManager(reactContext),
                new VRTARPlaneManager(reactContext),
                new VRTARImageMarkerManager(reactContext)
        );
    }
}
