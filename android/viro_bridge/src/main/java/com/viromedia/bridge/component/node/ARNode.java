/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.ARAnchor;
import com.viro.renderer.jni.ARNodeJni;
import com.viromedia.bridge.utility.ViroEvents;

public abstract class ARNode extends Node implements ARNodeJni.ARNodeDelegate {

    public ARNode(ReactApplicationContext context) {
        super(context);
    }

    abstract WritableMap mapFromARAnchor(ARAnchor arAnchor);

    @Override
    public void onAnchorFound(ARAnchor arAnchor) {
        WritableMap returnMap = mapFromARAnchor(arAnchor);
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_ANCHOR_FOUND,
                returnMap);
    }

    @Override
    public void onAnchorUpdated(ARAnchor arAnchor) {
        WritableMap returnMap = mapFromARAnchor(arAnchor);
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_ANCHOR_UPDATED,
                returnMap);
    }

    @Override
    public void onAnchorRemoved() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_ANCHOR_REMOVED,
                null);
    }
}
