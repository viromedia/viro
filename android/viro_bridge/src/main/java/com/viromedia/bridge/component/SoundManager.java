/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;

public class SoundManager extends ViroViewGroupManager<Sound> {

    public SoundManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSound";
    }

    @Override
    protected Sound createViewInstance(ThemedReactContext reactContext) {
        return new Sound(reactContext);
    }

    @ReactProp(name = "source")
    protected void setSource(Sound sound, ReadableMap source) {
        sound.setSource(source);
    }

    @ReactProp(name = "paused", defaultBoolean = false)
    protected void setPaused(Sound sound, boolean paused) {
        sound.setPaused(paused);
    }

    @ReactProp(name = "volume", defaultFloat = 1.0f)
    protected void setVolume(Sound sound, float volume) {
        sound.setVolume(volume);
    }

    @ReactProp(name = "muted", defaultBoolean = false)
    protected void setMuted(Sound sound, boolean muted) {
        sound.setMuted(muted);
    }

    @ReactProp(name = "loop", defaultBoolean = false)
    protected void setLoop(Sound sound, boolean loop) {
        sound.setLoop(loop);
    }
}
