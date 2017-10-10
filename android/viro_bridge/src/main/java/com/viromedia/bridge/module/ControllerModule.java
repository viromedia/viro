/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.module;

import android.view.View;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.uimanager.NativeViewHierarchyManager;
import com.facebook.react.uimanager.UIBlock;
import com.facebook.react.uimanager.UIManagerModule;
import com.viro.renderer.jni.Controller;
import com.viromedia.bridge.component.VRTController;

public class ControllerModule extends ReactContextBaseJavaModule {

    public ControllerModule(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTControllerModule";
    }

    @ReactMethod
    public void getForwardVectorAsync(final int viewTag, final Promise promise) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View controllerView = nativeViewHierarchyManager.resolveView(viewTag);
                if (controllerView instanceof VRTController) {
                    VRTController controller = (VRTController) controllerView;
                    controller.getForwardVectorAsync(new Controller.ControllerJniCallback() {
                        @Override
                        public void onGetForwardVector(float x, float y, float z) {
                            WritableArray array = Arguments.createArray();
                            array.pushDouble(x);
                            array.pushDouble(y);
                            array.pushDouble(z);
                            promise.resolve(array);
                        }
                    });
                }
            }
        });
    }
}
