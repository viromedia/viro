/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.utility;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.EventDelegate;
import com.viromedia.bridge.component.VRTComponent;

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
    public void onHover(int source, boolean isHovering, float position[]) {
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
    public void onClick(int source, EventDelegate.ClickState clickState, float position[]) {
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
        event.putInt("clickState", clickState.mTypeId);
        event.putArray("position", positionArray);

        component.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                component.getId(),
                ViroEvents.ON_CLICK,
                event);
    }

    @Override
    public void onTouch(int source, EventDelegate.TouchState touchState, float touchPadPos[]){
        VRTComponent component = weakComponent.get();
        if (component == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        event.putInt("touchState", touchState.mTypeId);

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
    public void onSwipe(int source, EventDelegate.SwipeState swipeState) {
        VRTComponent node = weakComponent.get();
        if (node == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        event.putInt("swipeState", swipeState.mTypeId);
        node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                node.getId(),
                ViroEvents.ON_SWIPE,
                event);
    }

    @Override
    public void onScroll(int source, float x, float y) {
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
    public void onDrag(int source, float x, float y, float z) {
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
    public void onFuse(int source) {
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
    public void onPinch(int source, float scaleFactor, EventDelegate.PinchState pinchState) {
        VRTComponent node = weakComponent.get();
        if (node == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        event.putDouble("scaleFactor", scaleFactor);
        event.putInt("pinchState", pinchState.mTypeId);

        node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                node.getId(),
                ViroEvents.ON_PINCH,
                event);
    }

    @Override
    public void onRotate(int source, float rotationFactor, EventDelegate.RotateState rotateState) {
        VRTComponent node = weakComponent.get();
        if (node == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        event.putDouble("rotationFactor", rotationFactor);
        event.putInt("rotateState", rotateState.mTypeId);

        node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                node.getId(),
                ViroEvents.ON_ROTATE,
                event);
    }

    @Override
    public void onControllerStatus(int source, EventDelegate.ControllerStatus controllerStatus) {
        VRTComponent node = weakComponent.get();
        if (node == null){
            return;
        }

        WritableMap event = Arguments.createMap();
        event.putInt("source", source);
        event.putInt("controllerStatus", controllerStatus.mTypeId);
        node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                node.getId(),
                ViroEvents.ON_CONTROLLER_STATUS,
                event);
    }
}
