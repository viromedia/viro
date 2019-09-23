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

package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReadableMap;
import com.viro.core.ViroContext;
import com.viromedia.bridge.component.node.VRTNode;

/**
 * This class wraps a SpatialSound object because a SpatialSound object can't extend both
 * a Node and a BaseSound.
 */
public class VRTSpatialSoundWrapper extends VRTNode {

    private final VRTSpatialSound mInnerSound;

    public VRTSpatialSoundWrapper(ReactContext reactContext) {
        super(reactContext);
        mInnerSound = new VRTSpatialSound(reactContext, getNodeJni());
    }

    @Override
    public void setId(int id) {
        super.setId(id);
        mInnerSound.setId(id);
    }

    @Override
    protected void handleAppearanceChange() {
        super.handleAppearanceChange();
        mInnerSound.handleAppearanceChange();
    }

    @Override
    protected void parentDidAppear() {
        super.parentDidAppear();
        mInnerSound.parentDidAppear();
    }

    public void setSource(ReadableMap source) {
        mInnerSound.setSource(source);
    }

    public void setPaused(boolean paused) {
        mInnerSound.setPaused(paused);
    }

    public void setVolume(float volume) {
        mInnerSound.setVolume(volume);
    }

    public void setMuted(boolean muted) {
        mInnerSound.setMuted(muted);
    }

    public void setLoop(boolean loop) {
        mInnerSound.setLoop(loop);
    }

    public void setPosition(float[] position) {
        mInnerSound.setPosition(position);
    }

    public void setRolloffModel(String rolloffModel) {
        mInnerSound.setRolloffModel(rolloffModel);
    }

    public void setMinDistance(float minDistance) {
        mInnerSound.setMinDistance(minDistance);
    }

    public void setMaxDistance(float maxDistance) {
        mInnerSound.setMaxDistance(maxDistance);
    }

    public void seekToTime(int seconds) {
        mInnerSound.seekToTime(seconds);
    }

    @Override
    protected void onPropsSet() {
        mInnerSound.onPropsSet();
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mInnerSound != null) {
            mInnerSound.onTearDown();
        }
    }

    @Override
    public void setViroContext(ViroContext context) {
        mInnerSound.setViroContext(context);
    }
}
