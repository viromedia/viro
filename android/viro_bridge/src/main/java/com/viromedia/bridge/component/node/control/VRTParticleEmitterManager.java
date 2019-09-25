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
public class VRTParticleEmitterManager extends VRTControlManager<VRTParticleEmitter> {

    public VRTParticleEmitterManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTParticleEmitter";
    }

    @Override
    protected VRTParticleEmitter createViewInstance(ThemedReactContext reactContext) {
        return new VRTParticleEmitter(reactContext);
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
