/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.ARAnchor;
import com.viro.core.internal.ARDeclarativeNode;
import com.viro.core.ARNode;
import com.viromedia.bridge.utility.ARUtils;
import com.viromedia.bridge.utility.ViroEvents;

public class VRTARNode extends VRTNode implements ARDeclarativeNode.Delegate {

    protected boolean mIsAnchored = false;

    public VRTARNode(ReactContext context) {
        super(context);
    }

    @Override
    public boolean shouldAppear() {
        return super.shouldAppear() && mIsAnchored;
    }

    public void setPauseUpdates(boolean pauseUpdates) {
        ((ARDeclarativeNode) getNodeJni()).setPauseUpdates(pauseUpdates);
    }

    // -- ARNodeDelegate implementation --

    @Override
    public void onAnchorFound(ARAnchor arAnchor) {
        mIsAnchored = true;
        handleAppearanceChange();
        WritableMap returnMap = Arguments.createMap();
        returnMap.putMap("anchorFoundMap", ARUtils.mapFromARAnchor(arAnchor));
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_ANCHOR_FOUND,
                returnMap);
    }

    @Override
    public void onAnchorUpdated(ARAnchor arAnchor) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putMap("anchorUpdatedMap", ARUtils.mapFromARAnchor(arAnchor));
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
