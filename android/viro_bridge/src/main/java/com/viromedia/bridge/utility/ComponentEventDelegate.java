/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.utility;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.ARHitTestResult;
import com.viro.renderer.jni.CameraCallback;
import com.viro.renderer.jni.EventDelegate;
import com.viro.renderer.jni.Node;
import com.viro.renderer.jni.event.ClickState;
import com.viro.renderer.jni.event.ControllerStatus;
import com.viro.renderer.jni.event.PinchState;
import com.viro.renderer.jni.event.RotateState;
import com.viro.renderer.jni.event.SwipeState;
import com.viro.renderer.jni.event.TouchState;
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
    public void onCameraARHitTest(int source, ARHitTestResult results[]) {

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
}
