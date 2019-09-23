//  Copyright © 2016 Viro Media. All rights reserved.
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
import com.viromedia.bridge.utility.ViroCommands;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

/**
 * Video360Manager for building a {@link VRT360Video}
 * corresponding to the Viro360Video.js control.
 */
public class VRT360VideoManager extends VRTViroViewGroupManager<VRT360Video> {
    public VRT360VideoManager(ReactApplicationContext context){
        super(context);
    }

    @Override
    public String getName() {
        return "VRT360Video";
    }

    @Override
    protected VRT360Video createViewInstance(ThemedReactContext reactContext) {
        VRT360Video video =  new VRT360Video(reactContext);
        return video;
    }

    @ReactProp(name = "paused", defaultBoolean = true)
    public void setPaused(VRT360Video view, boolean visibility) {
        view.setPaused(visibility);
    }

    @ReactProp(name = "loop", defaultBoolean = false)
    public void setLoop(VRT360Video view, boolean visibility) {
        view.setLoop(visibility);
    }

    @ReactProp(name = "muted", defaultBoolean = false)
    public void setMuted(VRT360Video view, boolean visibility) {
        view.setMuted(visibility);
    }

    @ReactProp(name = "volume", defaultFloat = 1f)
    public void setVolume(VRT360Video view, float opacity) {
        view.setVolume(opacity);
    }

    @ReactProp(name = "source")
    public void setSource(VRT360Video view, @Nullable ReadableMap map) {
        view.setSource(map.getString("uri"));
    }

    @ReactProp(name = "stereoMode")
    public void setStereoMode(VRT360Video view, String mode) { view.setStereoMode(mode); }

    @ReactProp(name = "rotation")
    public void setRotation(VRT360Video view, ReadableArray rotation) {
        view.setRotation(rotation);
    }

    @Override
    public @Nullable Map getExportedCustomDirectEventTypeConstants() {
        Map events = super.getExportedCustomDirectEventTypeConstants();

        events.put(ViroEvents.ON_BUFFER_START, MapBuilder.of("registrationName", ViroEvents.ON_BUFFER_START));
        events.put(ViroEvents.ON_BUFFER_END, MapBuilder.of("registrationName", ViroEvents.ON_BUFFER_END));
        events.put(ViroEvents.ON_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_FINISH));
        events.put(ViroEvents.ON_UPDATE_TIME, MapBuilder.of("registrationName", ViroEvents.ON_UPDATE_TIME));
        events.put(ViroEvents.ON_ERROR, MapBuilder.of("registrationName", ViroEvents.ON_ERROR));

        return events;
    }

    @Override
    public void receiveCommand(VRT360Video video, int commandType, @Nullable ReadableArray args) {
        switch (commandType) {
            case ViroCommands.SEEK_TO_TIME_INDEX:
                video.seekToTime((float) args.getDouble(0));
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
