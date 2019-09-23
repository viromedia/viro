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
import com.facebook.react.module.annotations.ReactModule;
import com.viro.core.Controller;
import com.viromedia.bridge.component.VRTController;

@ReactModule(name = "VRTControllerModule")
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
