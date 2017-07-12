/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.utility;

import android.util.Log;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.EventDelegateJni;
import com.viromedia.bridge.component.Component;

import java.lang.ref.WeakReference;

/**
 * Represents all java-to-javascript calls that can be triggered from an EventDelegate.
 */
public class ComponentEventDelegate implements EventDelegateJni.EventDelegateCallback {
    private WeakReference<Component> weakComponent;
    public ComponentEventDelegate(Component component){
        weakComponent = new WeakReference<Component>(component);
    }

    @Override
    public void onHover(int source, boolean isHovering, float position[]) {
        Component component = weakComponent.get();
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
    public void onClick(int source, EventDelegateJni.ClickState clickState, float position[]) {
        Component component = weakComponent.get();
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
    public void onTouch(int source, EventDelegateJni.TouchState touchState, float touchPadPos[]){
        Component component = weakComponent.get();
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
    public void onSwipe(int source, EventDelegateJni.SwipeState swipeState) {
        Component node = weakComponent.get();
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
        Component component = weakComponent.get();
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
        Component node = weakComponent.get();
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
        Component node = weakComponent.get();
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
    public void onControllerStatus(int source, EventDelegateJni.ControllerStatus controllerStatus) {
        Component node = weakComponent.get();
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
