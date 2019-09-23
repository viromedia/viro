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

package com.viromedia.bridge.component;


import androidx.annotation.Nullable;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroCommands;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class VRTSpatialSoundManager extends VRTViroViewGroupManager<VRTSpatialSoundWrapper> {

    public VRTSpatialSoundManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSpatialSound";
    }

    @Override
    public VRTSpatialSoundWrapper createViewInstance(ThemedReactContext reactContext) {
        return new VRTSpatialSoundWrapper(reactContext);
    }

    @ReactProp(name = "source")
    public void setSource(VRTSpatialSoundWrapper sound, ReadableMap source) {
        sound.setSource(source);
    }

    @ReactProp(name = "paused", defaultBoolean = false)
    public void setPaused(VRTSpatialSoundWrapper sound, boolean paused) {
        sound.setPaused(paused);
    }

    @ReactProp(name = "volume", defaultFloat = 1.0f)
    public void setVolume(VRTSpatialSoundWrapper sound, float volume) {
        sound.setVolume(volume);
    }

    @ReactProp(name = "muted", defaultBoolean = false)
    public void setMuted(VRTSpatialSoundWrapper sound, boolean muted) {
        sound.setMuted(muted);
    }

    @ReactProp(name = "loop", defaultBoolean = false)
    public void setLoop(VRTSpatialSoundWrapper sound, boolean loop) {
        sound.setLoop(loop);
    }

    @ReactProp(name = "position")
    public void setPosition(VRTSpatialSoundWrapper sound, ReadableArray position) {
        sound.setPosition(Helper.toFloatArray(position));
    }

    @ReactProp(name = "rolloffModel")
    public void setRolloffModel(VRTSpatialSoundWrapper sound, String rolloffModel) {
        sound.setRolloffModel(rolloffModel);
    }

    @ReactProp(name = "minDistance", defaultFloat = 0f)
    public void setMinDistance(VRTSpatialSoundWrapper sound, float minDistance) {
        sound.setMinDistance(minDistance);
    }

    @ReactProp(name = "maxDistance", defaultFloat = 0f)
    public void setMaxDistance(VRTSpatialSoundWrapper sound, float maxDistance) {
        sound.setMaxDistance(maxDistance);
    }

    // TODO: VIRO-758 - GVR doesn't support onFinish.
    @Override
    public @Nullable Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_FINISH),
                ViroEvents.ON_ERROR, MapBuilder.of("registrationName", ViroEvents.ON_ERROR)
        );
    }

    @Override
    public void receiveCommand(VRTSpatialSoundWrapper sound, int commandType, @Nullable ReadableArray args) {
        switch (commandType) {
            case ViroCommands.SEEK_TO_TIME_INDEX:
                sound.seekToTime((int) args.getDouble(0));
                break;
            default:
                throw new IllegalArgumentException("Unsupported command " + commandType
                        + " received by" + getClass().getSimpleName());
        }
    }

    @Override
    public Map<String,Integer> getCommandsMap() {
        return MapBuilder.of(ViroCommands.SEEK_TO_TIME_NAME, ViroCommands.SEEK_TO_TIME_INDEX);
    }
}
