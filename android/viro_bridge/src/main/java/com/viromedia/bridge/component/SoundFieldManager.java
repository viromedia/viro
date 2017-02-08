/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.Helper;

public class SoundFieldManager extends ViroViewGroupManager<SoundField> {

    public SoundFieldManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSoundField";
    }

    @Override
    public SoundField createViewInstance(ThemedReactContext reactContext) {
        return new SoundField(reactContext);
    }

    @ReactProp(name = "source")
    public void setSource(SoundField sound, ReadableMap source) {
        sound.setSource(source);
    }

    @ReactProp(name = "paused", defaultBoolean = false)
    public void setPaused(SoundField sound, boolean paused) {
        sound.setPaused(paused);
    }

    @ReactProp(name = "volume", defaultFloat = 1.0f)
    public void setVolume(SoundField sound, float volume) {
        sound.setVolume(volume);
    }

    @ReactProp(name = "muted", defaultBoolean = false)
    public void setMuted(SoundField sound, boolean muted) {
        sound.setMuted(muted);
    }

    @ReactProp(name = "loop", defaultBoolean = false)
    public void setLoop(SoundField sound, boolean loop) {
        sound.setLoop(loop);
    }

    @ReactProp(name = "rotation")
    public void setRotation(SoundField sound, ReadableArray rotation) {
        sound.setRotation(Helper.toFloatArray(rotation));
    }
}
