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

package com.viromedia.bridge.component.node;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.VRTCamera;
import com.viromedia.bridge.component.node.VRTNodeManager;

public class VRTCameraManager extends VRTNodeManager<VRTCamera> {

    public VRTCameraManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTCamera";
    }

    @Override
    protected VRTCamera createViewInstance(ThemedReactContext reactContext) {
        return new VRTCamera(reactContext);
    }

    @ReactProp(name = "position")
    public void setPosition(VRTCamera camera, ReadableArray position) {
        float[] posArray = {(float) position.getDouble(0), (float) position.getDouble(1),
                (float) position.getDouble(2)};
        camera.setPosition(posArray);
    }

    @ReactProp(name = "fieldOfView", defaultFloat = VRTCamera.DEFAULT_FOV)
    public void setFieldOfView(VRTCamera camera, float fov) {
        camera.setFieldOfView(fov);
    }

}
