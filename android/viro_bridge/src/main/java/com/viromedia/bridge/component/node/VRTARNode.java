/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.ARAnchor;
import com.viro.renderer.jni.ARNodeJni;
import com.viromedia.bridge.utility.ViroEvents;

public abstract class VRTARNode extends VRTNode implements ARNodeJni.ARNodeDelegate {

    protected boolean mIsAnchored = false;

    public VRTARNode(ReactApplicationContext context) {
        super(context);
    }

    abstract WritableMap mapFromARAnchor(ARAnchor arAnchor);

    @Override
    public boolean shouldAppear() {
        return super.shouldAppear() && mIsAnchored;
    }

    // -- ARNodeDelegate implementation --

    @Override
    public void onAnchorFound(ARAnchor arAnchor) {
        mIsAnchored = true;
        handleAppearanceChange();
        WritableMap returnMap = Arguments.createMap();
        returnMap.putMap("anchorFoundMap", mapFromARAnchor(arAnchor));
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_ANCHOR_FOUND,
                returnMap);
    }

    @Override
    public void onAnchorUpdated(ARAnchor arAnchor) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putMap("anchorUpdatedMap", mapFromARAnchor(arAnchor));
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_ANCHOR_UPDATED,
                returnMap);
    }

    @Override
    public void onAnchorRemoved() {
        mIsAnchored = false;
        handleAppearanceChange();
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_ANCHOR_REMOVED,
                null);
    }
}
