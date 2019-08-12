//  Copyright © 2016 Viro Media. All rights reserved.
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


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.ReactViroPackage;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

/**
 * SceneNavigatorManager for building a {@link VRTVRSceneNavigator}
 * corresponding to the ViroSceneNavigator.js control.
 */
public class VRTVRSceneNavigatorManager extends VRTViroViewGroupManager<VRTVRSceneNavigator> {

    private final ReactViroPackage.ViroPlatform mPlatform;

    public VRTVRSceneNavigatorManager(ReactApplicationContext context,
                                      ReactViroPackage.ViroPlatform platform) {
        super(context);
        mPlatform = platform;
    }

    @Override
    public String getName() {
        return "VRTVRSceneNavigator";
    }

    @Override
    protected VRTVRSceneNavigator createViewInstance(ThemedReactContext reactContext) {
        return new VRTVRSceneNavigator(reactContext, mPlatform);
    }

    @ReactProp(name = "vrModeEnabled", defaultBoolean = true)
    public void setVrModeEnabled(VRTVRSceneNavigator navigator, boolean vrModeEnabled) {
        navigator.setVrModeEnabled(vrModeEnabled);
    }

    @ReactProp(name = "hdrEnabled", defaultBoolean = true)
    public void setHDREnabled(VRTVRSceneNavigator navigator, boolean enabled) {
        navigator.setHDREnabled(enabled);
    }

    @ReactProp(name = "pbrEnabled", defaultBoolean = true)
    public void setPBREnabled(VRTVRSceneNavigator navigator, boolean enabled) {
        navigator.setPBREnabled(enabled);
    }

    @ReactProp(name = "bloomEnabled", defaultBoolean = true)
    public void setBloomEnabled(VRTVRSceneNavigator navigator, boolean enabled) {
        navigator.setBloomEnabled(enabled);
    }

    @ReactProp(name = "shadowsEnabled", defaultBoolean = true)
    public void setShadowsEnabled(VRTVRSceneNavigator navigator, boolean enabled) {
        navigator.setShadowsEnabled(enabled);
    }

    @ReactProp(name = "multisamplingEnabled", defaultBoolean = true)
    public void setMultisamplingEnabled(VRTVRSceneNavigator navigator, boolean enabled) {
        navigator.setMultisamplingEnabled(enabled);
    }

    @ReactProp(name = "debug", defaultBoolean = false)
    public void setDebug(VRTVRSceneNavigator navigator, boolean debug) {
        navigator.setDebug(debug);
    }

    @ReactProp(name = "currentSceneIndex")
    public void setCurrentSceneIndex(VRTVRSceneNavigator view, int selectedIndex) {
        view.setCurrentSceneIndex(selectedIndex);
    }

    @Override
    protected void onAfterUpdateTransaction(VRTVRSceneNavigator view) {
        super.onAfterUpdateTransaction(view);
        if (view instanceof VRTVRSceneNavigator) {
            ((VRTVRSceneNavigator) view).onPropsSet();
        }
    }

    @ReactProp(name = "hasOnExitViroCallback", defaultBoolean = false)
    public void setHasOnExitViroCallback(VRTVRSceneNavigator navigator, boolean hasOnExitViroCallback) {
        navigator.setHasOnExitViroCallback(hasOnExitViroCallback);
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_EXIT_VIRO, MapBuilder.of("registrationName", ViroEvents.ON_EXIT_VIRO));
    }
}
