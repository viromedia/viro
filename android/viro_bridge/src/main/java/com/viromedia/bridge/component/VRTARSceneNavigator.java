/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.view.View;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.ViroViewARCore;
import com.viro.renderer.jni.VrView;
import com.viromedia.bridge.ReactViroPackage;
import com.viromedia.bridge.component.node.VRTARScene;

/**
 * ARSceneNavigator manages the various AR scenes that a Viro App can navigate between.
 */
public class VRTARSceneNavigator extends VRTSceneNavigator {

    public VRTARSceneNavigator(ReactApplicationContext context) {
        super(context, ReactViroPackage.ViroPlatform.AR);
    }

    /*
     Override the parent method to use the ViroARView.
     */
    @Override
    protected VrView createViroView(ReactApplicationContext reactContext) {
        return new ViroViewARCore(reactContext.getCurrentActivity(),
                new InnerGLListener(this));
    }

    @Override
    public void addView(View child, int index) {
        // This view only accepts ARScene and VrView children!
        if (!(child instanceof VRTARScene) && !(child instanceof VrView)) {
            throw new IllegalArgumentException("Attempted to add a non-ARScene element ["
                    + child.getClass().getSimpleName() + "] to ARSceneNavigator!");
        }
        super.addView(child, index);
    }

    public ViroViewARCore getARView() {
        return (ViroViewARCore) mViroView;
    }
}
