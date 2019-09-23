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

package com.viromedia.bridge.utility;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.ARHitTestResult;
import com.viro.core.ARPointCloud;
import com.viro.core.internal.CameraCallback;
import com.viro.core.EventDelegate;
import com.viro.core.Node;
import com.viro.core.ClickState;
import com.viro.core.ControllerStatus;
import com.viro.core.PinchState;
import com.viro.core.RotateState;
import com.viro.core.SwipeState;
import com.viro.core.TouchState;
import com.viromedia.bridge.component.VRTComponent;
import com.viromedia.bridge.component.node.VRTARScene;
import com.viromedia.bridge.component.node.VRTScene;

import java.lang.ref.WeakReference;

/**
 * Represents all java-to-javascript calls that can be triggered from an EventDelegate.
 */
public class ComponentEventDelegate implements EventDelegate.EventDelegateCallback {
    private WeakReference<VRTComponent> weakComponent;
    public ComponentEventDelegate(VRTComponent component){
        weakComponent = new WeakReference<VRTComponent>(component);
    }

    @Override
    public void onHover(int source, Node node, boolean isHovering, float position[]) {
        VRTComponent component = weakComponent.get();
        if (component == null){
            return;
        }

        WritableArray positionArray = Arguments.createArray();
        if (position != null && position.length == 3) {
            positionArray.pushDouble(position[0]);
            positionArray.pushDouble(position[1]);
            positionArray.pushDouble(position[2]);
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        event.putBoolean("isHovering", isHovering);
        event.putArray("position", positionArray);

        component.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                component.getId(),
                ViroEvents.ON_HOVER,
                event);
    }

    @Override
    public void onClick(int source, Node node, ClickState clickState, float position[]) {
        VRTComponent component = weakComponent.get();
        if (component == null){
            return;
        }

        WritableArray positionArray = Arguments.createArray();
        if (position != null && position.length == 3) {
            positionArray.pushDouble(position[0]);
            positionArray.pushDouble(position[1]);
            positionArray.pushDouble(position[2]);
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        event.putInt("clickState", clickState.getTypeId());
        event.putArray("position", positionArray);

        component.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                component.getId(),
                ViroEvents.ON_CLICK,
                event);
    }

    @Override
    public void onTouch(int source, Node node, TouchState touchState, float touchPadPos[]){
        VRTComponent component = weakComponent.get();
        if (component == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        event.putInt("touchState", touchState.getTypeId());

        WritableArray touchPos = Arguments.createArray();
        touchPos.pushDouble(touchPadPos[0]);
        touchPos.pushDouble(touchPadPos[1]);
        event.putArray("touchPos", touchPos);

        component.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                component.getId(),
                ViroEvents.ON_TOUCH,
                event);
    }

    @Override
    public void onSwipe(int source, Node target, SwipeState swipeState) {
        VRTComponent node = weakComponent.get();
        if (node == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        event.putInt("swipeState", swipeState.getTypeId());
        node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                node.getId(),
                ViroEvents.ON_SWIPE,
                event);
    }

    @Override
    public void onScroll(int source, Node node, float x, float y) {
        VRTComponent component = weakComponent.get();
        if (component == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        WritableArray scrollPos = Arguments.createArray();
        scrollPos.pushDouble(x);
        scrollPos.pushDouble(y);

        event.putArray("scrollPos", scrollPos);
        component.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                component.getId(),
                ViroEvents.ON_SCROLL,
                event);
    }

    @Override
    public void onDrag(int source, Node target, float x, float y, float z) {
        VRTComponent node = weakComponent.get();
        if (node == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        WritableArray dragToPos = Arguments.createArray();
        dragToPos.pushDouble(x);
        dragToPos.pushDouble(y);
        dragToPos.pushDouble(z);
        event.putArray("dragToPos", dragToPos);

        node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                node.getId(),
                ViroEvents.ON_DRAG,
                event);
    }

    @Override
    public void onFuse(int source, Node target) {
        VRTComponent node = weakComponent.get();
        if (node == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                node.getId(),
                ViroEvents.ON_FUSE,
                event);
    }

    @Override
    public void onPinch(int source, Node target, float scaleFactor, PinchState pinchState) {
        VRTComponent node = weakComponent.get();
        if (node == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        event.putDouble("scaleFactor", scaleFactor);
        event.putInt("pinchState", pinchState.getTypeId());

        node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                node.getId(),
                ViroEvents.ON_PINCH,
                event);
    }

    @Override
    public void onCameraARHitTest(ARHitTestResult results[]) {

        VRTComponent node = weakComponent.get();

        if (node == null){
            return;
        }
        if( node instanceof VRTScene) {
            final VRTScene scene = (VRTScene) node;
            final ARHitTestResult arResults[] = results;
            scene.getCameraPositionAsync(new CameraCallback() {
                @Override
                public void onGetCameraOrientation(float posX, float poxY, float posZ,
                                                   float rotEulerX, float rotEulerY, float rotEulerZ,
                                                   float forwardX, float forwardY, float forwardZ,
                                                   float upX, float upY, float upZ) {
                    WritableArray cameraOrientationArray = Arguments.createArray();
                    cameraOrientationArray.pushDouble(posX);
                    cameraOrientationArray.pushDouble(poxY);
                    cameraOrientationArray.pushDouble(posZ);
                    cameraOrientationArray.pushDouble(Math.toDegrees(rotEulerX));
                    cameraOrientationArray.pushDouble(Math.toDegrees(rotEulerY));
                    cameraOrientationArray.pushDouble(Math.toDegrees(rotEulerZ));
                    cameraOrientationArray.pushDouble(forwardX);
                    cameraOrientationArray.pushDouble(forwardY);
                    cameraOrientationArray.pushDouble(forwardZ);
                    cameraOrientationArray.pushDouble(upX);
                    cameraOrientationArray.pushDouble(upY);
                    cameraOrientationArray.pushDouble(upZ);

                    WritableArray hitTestResultsArray = Arguments.createArray();
                    for (ARHitTestResult result : arResults) {
                        hitTestResultsArray.pushMap(ARUtils.mapFromARHitTestResult(result));
                    }
                    WritableMap event = Arguments.createMap();
                    event.putArray("hitTestResults", hitTestResultsArray);

                    event.putArray("cameraOrientation", cameraOrientationArray);

                    scene.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                            scene.getId(),
                            ViroEvents.ON_CAMERA_AR_HIT_TEST_VIRO,
                            event);
                }
            });
        }
    }

    @Override
    public void onARPointCloudUpdate(ARPointCloud arPointCloud) {
        VRTComponent node = weakComponent.get();
        if (node == null) {
            return;
        }

        if (node instanceof VRTARScene) {
            final VRTARScene arScene = (VRTARScene) node;

            WritableMap event = Arguments.createMap();
            event.putMap("pointCloud", ARUtils.mapFromARPointCloud(arPointCloud));

            arScene.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                    arScene.getId(),
                    ViroEvents.ON_AR_POINT_CLOUD_UPDATE,
                    event);
        }
    }

    @Override
    public void onRotate(int source, Node target, float rotationRadians, RotateState rotateState) {
        VRTComponent node = weakComponent.get();
        if (node == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        event.putDouble("rotationFactor", Math.toDegrees(rotationRadians));
        event.putInt("rotateState", rotateState.getTypeId());

        node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                node.getId(),
                ViroEvents.ON_ROTATE,
                event);
    }

    @Override
    public void onControllerStatus(int source, ControllerStatus controllerStatus) {
        VRTComponent node = weakComponent.get();
        if (node == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        event.putInt("controllerStatus", controllerStatus.getTypeId());
        node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                node.getId(),
                ViroEvents.ON_CONTROLLER_STATUS,
                event);
    }

    @Override
    public void onCameraTransformUpdate(float posX, float poxY, float posZ,
                                        float rotEulerX, float rotEulerY, float rotEulerZ,
                                        float forwardX, float forwardY, float forwardZ,
                                        float upX, float upY, float upZ) {
        VRTComponent node = weakComponent.get();
        if (node == null) {
            return;
        }

        // This applies to all scenes (AR and non-AR)
        if (node instanceof VRTScene) {
            final VRTScene scene = (VRTScene) node;

            WritableMap event = Arguments.createMap();

            WritableArray cameraTransformArray = Arguments.createArray();
            cameraTransformArray.pushDouble(posX);
            cameraTransformArray.pushDouble(poxY);
            cameraTransformArray.pushDouble(posZ);
            cameraTransformArray.pushDouble(Math.toDegrees(rotEulerX));
            cameraTransformArray.pushDouble(Math.toDegrees(rotEulerY));
            cameraTransformArray.pushDouble(Math.toDegrees(rotEulerZ));
            cameraTransformArray.pushDouble(forwardX);
            cameraTransformArray.pushDouble(forwardY);
            cameraTransformArray.pushDouble(forwardZ);
            cameraTransformArray.pushDouble(upX);
            cameraTransformArray.pushDouble(upY);
            cameraTransformArray.pushDouble(upZ);

            event.putArray("cameraTransform", cameraTransformArray);

            scene.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                    scene.getId(),
                    ViroEvents.ON_CAMERA_TRANSFORM_UPDATE,
                    event);
        }
    }
}
