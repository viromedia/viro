/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.os.Handler;
import android.os.Looper;

import com.facebook.react.bridge.ReactContext;
import com.viro.core.ViroViewGVR;
import com.viro.core.ViroViewOVR;
import com.viro.core.ViroView;
import com.viromedia.bridge.ReactViroPackage;
import com.viromedia.bridge.utility.ViroLog;

import java.lang.ref.WeakReference;

public class VRTVRSceneNavigator extends VRT3DSceneNavigator {
    private static final String TAG = ViroLog.getTag(VRTVRSceneNavigator.class);

    // keeps track if we should call setVrModeEnabled on the ViroView in onPropsSet(). We always
    // need to set it the first time because we want to start with "false" and then switch to "true"
    // in order to prevent the disabling of low persistence mode when entering 360 mode.
    private boolean mNeedsSetVrMode = true;
    private boolean mVrMode = true;

    private static class StartupListenerGVR implements ViroViewGVR.StartupListener {

        private WeakReference<VRTVRSceneNavigator> mNavigator;

        public StartupListenerGVR(VRTVRSceneNavigator navigator) {
            mNavigator = new WeakReference<VRTVRSceneNavigator>(navigator);
        }

        @Override
        public void onSuccess() {
            final VRTVRSceneNavigator navigator = mNavigator.get();
            final WeakReference<VRTVRSceneNavigator> navigatorWeakReference =
                    new WeakReference<VRTVRSceneNavigator>(navigator);

            if (navigator == null) {
                return;
            }

            navigator.mGLInitialized = true;
            (new Handler(Looper.getMainLooper())).post(new Runnable() {
                @Override
                public void run() {
                    final VRTVRSceneNavigator sceneNav = navigatorWeakReference.get();
                    if(sceneNav != null) {
                        sceneNav.mGLInitialized = true;
                        sceneNav.setViroContext();
                    }
                }
            });
        }

        @Override
        public void onFailure(ViroViewGVR.StartupError startupError, String s) {

        }
    }

    private static class StartupListenerOVR implements ViroViewOVR.StartupListener {

        private WeakReference<VRTVRSceneNavigator> mNavigator;

        public StartupListenerOVR(VRTVRSceneNavigator navigator) {
            mNavigator = new WeakReference<VRTVRSceneNavigator>(navigator);
        }

        @Override
        public void onSuccess() {
            final VRTVRSceneNavigator navigator = mNavigator.get();
            if (navigator == null) {
                return;
            }

            navigator.mGLInitialized = true;
            (new Handler(Looper.getMainLooper())).post(new Runnable() {
                @Override
                public void run() {
                    navigator.mGLInitialized = true;
                    navigator.setViroContext();
                }
            });
        }

        @Override
        public void onFailure(ViroViewOVR.StartupError startupError, String s) {

        }
    }

    public VRTVRSceneNavigator(ReactContext reactContext,
                               ReactViroPackage.ViroPlatform platform) {
        super(reactContext, platform);
    }

    protected ViroView createViroView(ReactContext reactContext) {
        switch (mPlatform) {
            case OVR_MOBILE:
                return new ViroViewOVR(reactContext.getCurrentActivity(),
                        new StartupListenerOVR(this));
            case GVR:
                // default case is to use GVR
            default:
                return new ViroViewGVR(reactContext.getCurrentActivity(),
                        new StartupListenerGVR(this), new OnGVRExitListener(this), null, false);
        }
    }

    public void setVrModeEnabled(boolean vrModeEnabled) {
        mVrMode = vrModeEnabled;
        mNeedsSetVrMode = true;
    }

    @Override
    public void onPropsSet() {
        if (mNeedsSetVrMode) {
            mViroView.setVRModeEnabled(mVrMode);
            mNeedsSetVrMode = false;
        }
    }

    private static class OnGVRExitListener implements Runnable {
        private WeakReference<VRTVRSceneNavigator> mSceneNavWeak;
        public OnGVRExitListener(VRTVRSceneNavigator sceneNav) {
            mSceneNavWeak = new WeakReference<VRTVRSceneNavigator>(sceneNav);
        }

        @Override
        public void run() {
            VRTVRSceneNavigator view = mSceneNavWeak.get();
            if (view == null) {
                return;
            }

            view.userDidRequestExitVR();
        }
    }
}
