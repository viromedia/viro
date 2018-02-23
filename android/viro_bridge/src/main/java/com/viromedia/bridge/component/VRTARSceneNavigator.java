/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.core.ViroViewARCore;
import com.viro.core.ViroView;
import com.viromedia.bridge.ReactViroPackage;
import com.viromedia.bridge.component.node.VRTARScene;
import com.viromedia.bridge.utility.DisplayRotationListener;

import java.lang.ref.WeakReference;

/**
 * ARSceneNavigator manages the various AR scenes that a Viro App can navigate between.
 */
public class VRTARSceneNavigator extends VRTSceneNavigator {

    private DisplayRotationListener mRotationListener;

    private static class StartupListenerARCore implements ViroViewARCore.StartupListener {

        private WeakReference<VRTSceneNavigator> mNavigator;

        public StartupListenerARCore(VRTSceneNavigator navigator) {
            mNavigator = new WeakReference<VRTSceneNavigator>(navigator);
        }

        @Override
        public void onSuccess() {
            final VRTSceneNavigator navigator = mNavigator.get();
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
        public void onFailure(ViroViewARCore.StartupError error, String errorMessage) {
            Log.e("Viro", "onRendererFailed [error: " + error + "], message [" + errorMessage + "]");
            // TODO VIRO-2906 Response to ARCore errors on bridge
        }
    }

    public VRTARSceneNavigator(ReactApplicationContext context) {
        super(context, ReactViroPackage.ViroPlatform.AR);
        mRotationListener = new DisplayRotationListener(context) {
            @Override
            public void onDisplayRotationChanged(int rotation) {
                ViroViewARCore view = getARView();
                if (view != null) {
                    view.setCameraRotation(rotation);
                }
            }
        };
        mRotationListener.enable();
    }

    /*
     Override the parent method to use the ViroARView.
     */
    @Override
    protected ViroView createViroView(ReactApplicationContext reactContext) {
        return new ViroViewARCore(reactContext.getCurrentActivity(),
                new StartupListenerARCore(this));
    }

    @Override
    public void addView(View child, int index) {
        // This view only accepts ARScene and VrView children!
        if (!(child instanceof VRTARScene) && !(child instanceof ViroView)) {
            throw new IllegalArgumentException("Attempted to add a non-ARScene element ["
                    + child.getClass().getSimpleName() + "] to ARSceneNavigator!");
        }
        super.addView(child, index);
    }

    public ViroViewARCore getARView() {
        return (ViroViewARCore) mViroView;
    }

    public void resetARSession() {
        ViroViewARCore arView = getARView();
        // No-op for now.
    }
}
