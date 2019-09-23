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

public class VRTSoundFieldManager extends VRTViroViewGroupManager<VRTSoundField> {

    public VRTSoundFieldManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSoundField";
    }

    @Override
    public VRTSoundField createViewInstance(ThemedReactContext reactContext) {
        return new VRTSoundField(reactContext);
    }

    @ReactProp(name = "source")
    public void setSource(VRTSoundField sound, ReadableMap source) {
        sound.setSource(source);
    }

    @ReactProp(name = "paused", defaultBoolean = false)
    public void setPaused(VRTSoundField sound, boolean paused) {
        sound.setPaused(paused);
    }

    @ReactProp(name = "volume", defaultFloat = 1.0f)
    public void setVolume(VRTSoundField sound, float volume) {
        sound.setVolume(volume);
    }

    @ReactProp(name = "muted", defaultBoolean = false)
    public void setMuted(VRTSoundField sound, boolean muted) {
        sound.setMuted(muted);
    }

    @ReactProp(name = "loop", defaultBoolean = false)
    public void setLoop(VRTSoundField sound, boolean loop) {
        sound.setLoop(loop);
    }

    @ReactProp(name = "rotation")
    public void setRotation(VRTSoundField sound, ReadableArray rotation) {
        sound.setRotation(Helper.toFloatArray(rotation));
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
    public void receiveCommand(VRTSoundField sound, int commandType, @Nullable ReadableArray args) {
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
