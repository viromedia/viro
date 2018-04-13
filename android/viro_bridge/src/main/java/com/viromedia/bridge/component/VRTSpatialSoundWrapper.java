/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
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
