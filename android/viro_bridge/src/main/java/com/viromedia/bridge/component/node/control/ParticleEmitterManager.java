/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.Node;
import com.viromedia.bridge.component.node.NodeManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

/**
 * ParticleEmitterManager for building a {@link ParticleEmitter}
 * corresponding to the ViroParticleEmitter.js control.
 */
public class ParticleEmitterManager extends NodeManager<ParticleEmitter> {

    public ParticleEmitterManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTParticleEmitter";
    }

    @Override
    protected ParticleEmitter createViewInstance(ThemedReactContext reactContext) {
        return new ParticleEmitter(getContext());
    }

    @ReactProp(name = "duration", defaultFloat = ParticleEmitter.DEFAULT_DURATION)
    public void setDuration(ParticleEmitter emitter, float width) {
        emitter.setDuration(width);
    }

    @ReactProp(name = "delay", defaultFloat = ParticleEmitter.DEFAULT_DELAY)
    public void setDelay(ParticleEmitter emitter, float height) {
        emitter.setDelay(height);
    }

    @ReactProp(name = "loop", defaultBoolean = ParticleEmitter.DEFAULT_LOOP)
    public void setLoop(ParticleEmitter emitter, boolean length) {
        emitter.setLoop(length);
    }

    @ReactProp(name = "run", defaultBoolean = ParticleEmitter.DEFAULT_RUN)
    public void setRun(ParticleEmitter emitter, boolean length) {
        emitter.setRun(length);
    }

    @ReactProp(name = "fixedToEmitter", defaultBoolean = ParticleEmitter.DEFAULT_FIXED_TO_EMITTER)
    public void setFixedToEmitter(ParticleEmitter emitter, boolean length) {
        emitter.setFixedToEmitter(length);
    }

    @ReactProp(name = "image")
    public void setImage(ParticleEmitter emitter, ReadableMap map) {
        emitter.setImage(map);
    }

    @ReactProp(name = "spawnBehavior")
    public void setSpawnBehavior(ParticleEmitter emitter, ReadableMap map) {
        emitter.setSpawnBehavior(map);
    }

    @ReactProp(name = "particleAppearance")
    public void setParticleAppearance(ParticleEmitter emitter, ReadableMap map) {
        emitter.setParticleAppearance(map);
    }

    @ReactProp(name = "particlePhysics")
    public void setParticlePhysics(ParticleEmitter emitter, ReadableMap map) {
        emitter.setParticlePhysics(map);
    }
}
