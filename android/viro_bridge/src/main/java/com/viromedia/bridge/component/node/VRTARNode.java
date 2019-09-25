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
