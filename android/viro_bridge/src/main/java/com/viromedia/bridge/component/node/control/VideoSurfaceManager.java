/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.support.annotation.Nullable;
import android.util.Log;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.NodeManager;
import java.util.Map;

/**
 * VideoSurfaceManager for building a {@link VideoSurface}
 * corresponding to the ViroVideo.js control.
 */
public class VideoSurfaceManager extends NodeManager<VideoSurface> {
    private final ReactApplicationContext mContext;

    public VideoSurfaceManager(ReactApplicationContext context){
        mContext = context;
    }

    @Override
    public String getName() {
        return "VRTVideoSurface";
    }

    @Override
    protected VideoSurface createViewInstance(ThemedReactContext reactContext) {
        VideoSurface video =  new VideoSurface(mContext);
        return video;
    }

    @ReactProp(name = "width", defaultFloat = 1f)
    public void setWidth(VideoSurface view, float opacity) {
        view.setWidth(opacity);
    }

    @ReactProp(name = "height", defaultFloat = 1f)
    public void setHeight(VideoSurface view, float opacity) {
        view.setHeight(opacity);
    }

    @ReactProp(name = "paused", defaultBoolean = true)
    public void setPaused(VideoSurface view, boolean visibility) {
        view.setPaused(visibility);
    }

    @ReactProp(name = "loop", defaultBoolean = false)
    public void setLoop(VideoSurface view, boolean visibility) {
        view.setLoop(visibility);
    }

    @ReactProp(name = "muted", defaultBoolean = false)
    public void setMuted(VideoSurface view, boolean visibility) {
        view.setMuted(visibility);
    }

    @ReactProp(name = "volume", defaultFloat = 1f)
    public void setVolume(VideoSurface view, float opacity) {
        view.setVolume(opacity);
    }

    @ReactProp(name = "source")
    public void setSource(VideoSurface view, @Nullable ReadableMap map) {
        view.setSource(map.getString("uri"));
    }


    protected final static String VIDEO_FINISHED_CALLBACK = "onFinish";
    @Override
    public @Nullable Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                VIDEO_FINISHED_CALLBACK,
                MapBuilder.of("registrationName", VIDEO_FINISHED_CALLBACK)
        );
    }
}
