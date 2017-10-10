/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.VRTNodeManager;

/**
 * ParticleEmitterManager for building a {@link VRTParticleEmitter}
 * corresponding to the ViroParticleEmitter.js control.
 */
public class VRTParticleEmitterManager extends VRTNodeManager<VRTParticleEmitter> {

    public VRTParticleEmitterManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTParticleEmitter";
    }

    @Override
    protected VRTParticleEmitter createViewInstance(ThemedReactContext reactContext) {
        return new VRTParticleEmitter(getContext());
    }

    @ReactProp(name = "duration", defaultFloat = VRTParticleEmitter.DEFAULT_DURATION)
    public void setDuration(VRTParticleEmitter emitter, float width) {
        emitter.setDuration(width);
    }

    @ReactProp(name = "delay", defaultFloat = VRTParticleEmitter.DEFAULT_DELAY)
    public void setDelay(VRTParticleEmitter emitter, float height) {
        emitter.setDelay(height);
    }

    @ReactProp(name = "loop", defaultBoolean = VRTParticleEmitter.DEFAULT_LOOP)
    public void setLoop(VRTParticleEmitter emitter, boolean length) {
        emitter.setLoop(length);
    }

    @ReactProp(name = "run", defaultBoolean = VRTParticleEmitter.DEFAULT_RUN)
    public void setRun(VRTParticleEmitter emitter, boolean length) {
        emitter.setRun(length);
    }

    @ReactProp(name = "fixedToEmitter", defaultBoolean = VRTParticleEmitter.DEFAULT_FIXED_TO_EMITTER)
    public void setFixedToEmitter(VRTParticleEmitter emitter, boolean length) {
        emitter.setFixedToEmitter(length);
    }

    @ReactProp(name = "image")
    public void setImage(VRTParticleEmitter emitter, ReadableMap map) {
        emitter.setImage(map);
    }

    @ReactProp(name = "spawnBehavior")
    public void setSpawnBehavior(VRTParticleEmitter emitter, ReadableMap map) {
        emitter.setSpawnBehavior(map);
    }

    @ReactProp(name = "particleAppearance")
    public void setParticleAppearance(VRTParticleEmitter emitter, ReadableMap map) {
        emitter.setParticleAppearance(map);
    }

    @ReactProp(name = "particlePhysics")
    public void setParticlePhysics(VRTParticleEmitter emitter, ReadableMap map) {
        emitter.setParticlePhysics(map);
    }
}
