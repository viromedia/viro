/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.support.annotation.Nullable;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.NodeManager;
import com.viromedia.bridge.utility.ViroCommands;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

/**
 * VideoSurfaceManager for building a {@link VideoSurface}
 * corresponding to the ViroVideo.js control.
 */
public class VideoSurfaceManager extends NodeManager<VideoSurface> {

    public VideoSurfaceManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTVideoSurface";
    }

    @Override
    protected VideoSurface createViewInstance(ThemedReactContext reactContext) {
        VideoSurface video =  new VideoSurface(getContext());
        return video;
    }

    @ReactProp(name = "width", defaultFloat = 1f)
    public void setWidth(VideoSurface view, float width) {
        view.setWidth(width);
    }

    @ReactProp(name = "height", defaultFloat = 1f)
    public void setHeight(VideoSurface view, float height) {
        view.setHeight(height);
    }

    @ReactProp(name = "paused", defaultBoolean = true)
    public void setPaused(VideoSurface view, boolean paused) {
        view.setPaused(paused);
    }

    @ReactProp(name = "loop", defaultBoolean = false)
    public void setLoop(VideoSurface view, boolean loop) {
        view.setLoop(loop);
    }

    @ReactProp(name = "muted", defaultBoolean = false)
    public void setMuted(VideoSurface view, boolean muted) {
        view.setMuted(muted);
    }

    @ReactProp(name = "volume", defaultFloat = 1f)
    public void setVolume(VideoSurface view, float volume) {
        view.setVolume(volume);
    }

    @ReactProp(name = "stereoMode")
    public void setStereoMode(VideoSurface view, String mode) { view.setStereoMode(mode); }

    @ReactProp(name = "source")
    public void setSource(VideoSurface view, @Nullable ReadableMap map) {
        // TODO: verify/make sure this work with local files (ie. release builds)
        view.setSource(map.getString("uri"));
    }

    @Override
    public @Nullable Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_BUFFER_START, MapBuilder.of("registrationName", ViroEvents.ON_BUFFER_START),
                ViroEvents.ON_BUFFER_END, MapBuilder.of("registrationName", ViroEvents.ON_BUFFER_END),
                ViroEvents.ON_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_FINISH),
                ViroEvents.ON_UPDATE_TIME, MapBuilder.of("registrationName", ViroEvents.ON_UPDATE_TIME),
                ViroEvents.ON_ERROR, MapBuilder.of("registrationName", ViroEvents.ON_ERROR),
                ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START),
                ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH)
        );
    }

    @Override
    public void receiveCommand(VideoSurface video, int commandType, @Nullable ReadableArray args) {
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
