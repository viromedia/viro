/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import android.support.annotation.Nullable;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class SpatialSoundManager extends ViroViewGroupManager<SpatialSoundWrapper> {

    public SpatialSoundManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSpatialSound";
    }

    @Override
    public SpatialSoundWrapper createViewInstance(ThemedReactContext reactContext) {
        return new SpatialSoundWrapper(getContext());
    }

    @ReactProp(name = "source")
    public void setSource(SpatialSoundWrapper sound, ReadableMap source) {
        sound.setSource(source);
    }

    @ReactProp(name = "paused", defaultBoolean = false)
    public void setPaused(SpatialSoundWrapper sound, boolean paused) {
        sound.setPaused(paused);
    }

    @ReactProp(name = "volume", defaultFloat = 1.0f)
    public void setVolume(SpatialSoundWrapper sound, float volume) {
        sound.setVolume(volume);
    }

    @ReactProp(name = "muted", defaultBoolean = false)
    public void setMuted(SpatialSoundWrapper sound, boolean muted) {
        sound.setMuted(muted);
    }

    @ReactProp(name = "loop", defaultBoolean = false)
    public void setLoop(SpatialSoundWrapper sound, boolean loop) {
        sound.setLoop(loop);
    }

    @ReactProp(name = "position")
    public void setPosition(SpatialSoundWrapper sound, ReadableArray position) {
        sound.setPosition(Helper.toFloatArray(position));
    }

    @ReactProp(name = "rolloffModel")
    public void setRolloffModel(SpatialSoundWrapper sound, String rolloffModel) {
        sound.setRolloffModel(rolloffModel);
    }

    @ReactProp(name = "minDistance", defaultFloat = 0f)
    public void setMinDistance(SpatialSoundWrapper sound, float minDistance) {
        sound.setMinDistance(minDistance);
    }

    @ReactProp(name = "maxDistance", defaultFloat = 0f)
    public void setMaxDistance(SpatialSoundWrapper sound, float maxDistance) {
        sound.setMaxDistance(maxDistance);
    }

    // TODO: VIRO-758 - GVR doesn't support onFinish.
    @Override
    public @Nullable Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_FINISH)
        );
    }
}
