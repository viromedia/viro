//  Copyright © 2017 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

package com.viromedia.bridge.component;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;

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
    private boolean mAutoFocusEnabled = false;
    private boolean mNeedsAutoFocusToggle = false;

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

            if (navigator.mNeedsAutoFocusToggle) {
                navigator.setAutoFocusEnabled(navigator.mAutoFocusEnabled);
                navigator.mNeedsAutoFocusToggle = false;
            }
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

    public void setAutoFocusEnabled(boolean enabled) {
        mAutoFocusEnabled = enabled;
        if (mGLInitialized) {
            ((ViroViewARCore)mViroView).setCameraAutoFocusEnabled(mAutoFocusEnabled);
        } else {
            mNeedsAutoFocusToggle = true;
        }
    }
}
