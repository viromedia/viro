/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.viro.renderer.jni.RenderContextJni;
import com.viromedia.bridge.component.node.Node;

/**
 * This class wraps a SpatialSound object because a SpatialSound object can't extend both
 * a Node and a BaseSound.
 */
public class SpatialSoundWrapper extends Node {

    private final SpatialSound mInnerSound;

    public SpatialSoundWrapper(ReactApplicationContext reactContext) {
        super(reactContext);
        mInnerSound = new SpatialSound(reactContext, getNodeJni());
    }

    @Override
    public void setId(int id) {
        super.setId(id);
        mInnerSound.setId(id);
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

    @Override
    protected void onPropsSet() {
        mInnerSound.onPropsSet();
    }

    @Override
    public void setRenderContext(RenderContextJni context) {
        mInnerSound.setRenderContext(context);
    }
}
