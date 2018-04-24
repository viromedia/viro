/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.facebook.react.bridge.ReactContext;
import com.viro.core.ViroViewARCore;
import com.viro.core.ViroView;
import com.viromedia.bridge.ReactViroPackage;
import com.viromedia.bridge.component.node.VRTARScene;
import com.viromedia.bridge.utility.DisplayRotationListener;

import java.lang.ref.WeakReference;

/**
 * ARSceneNavigator manages the various AR scenes that a Viro App can navigate between.
 */
public class VRTARSceneNavigator extends VRT3DSceneNavigator {

    private DisplayRotationListener mRotationListener;

    private static class StartupListenerARCore implements ViroViewARCore.StartupListener {

        private WeakReference<VRTARSceneNavigator> mNavigator;

        public StartupListenerARCore(VRTARSceneNavigator navigator) {
            mNavigator = new WeakReference<VRTARSceneNavigator>(navigator);
        }

        @Override
        public void onSuccess() {
            final VRTARSceneNavigator navigator = mNavigator.get();
            final WeakReference<VRTARSceneNavigator> navigatorWeakReference =
                    new WeakReference<VRTARSceneNavigator>(navigator);

            if (navigator == null) {
                return;
            }

            navigator.mGLInitialized = true;
            (new Handler(Looper.getMainLooper())).post(new Runnable() {
                @Override
                public void run() {
                    VRTARSceneNavigator nav = navigatorWeakReference.get();
                    if (nav != null) {
                        nav.mGLInitialized = true;
                        nav.setViroContext();
                    }
                }
            });
        }

        @Override
        public void onFailure(ViroViewARCore.StartupError error, String errorMessage) {
            Log.e("Viro", "onRendererFailed [error: " + error + "], message [" + errorMessage + "]");
            // No-op
        }
    }

    public VRTARSceneNavigator(ReactContext context) {
        super(context, ReactViroPackage.ViroPlatform.AR);
        final  WeakReference<VRTARSceneNavigator> weakSceneARRef = new WeakReference<VRTARSceneNavigator>(this);
        mRotationListener = new DisplayRotationListener(context) {
            @Override
            public void onDisplayRotationChanged(int rotation) {
                VRTARSceneNavigator navigator = weakSceneARRef.get();
                if (navigator != null) {
                    ViroViewARCore view = navigator.getARView();
                    if (view != null) {
                        view.setCameraRotation(rotation);
                    }
                }
            }
        };
        mRotationListener.enable();
    }

    /*
     Override the parent method to use the ViroARView.
     */
    @Override
    protected ViroView createViroView(ReactContext reactContext) {
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

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        if (mRotationListener != null) {
            mRotationListener.disable();
        }
    }
}
