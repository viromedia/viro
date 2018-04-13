package com.viromedia.bridge.component;

import android.support.annotation.Nullable;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.control.VRTVideoSurface;
import com.viromedia.bridge.utility.ViroCommands;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

/**
 * Created by vadvani on 3/12/18.
 */

public class VRTMaterialVideoManager extends VRTViroViewGroupManager<VRTMaterialVideo> {

    public VRTMaterialVideoManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    protected VRTMaterialVideo createViewInstance(ThemedReactContext reactContext) {
        return new VRTMaterialVideo(reactContext);
    }
    @Override
    public String getName() {
        return "VRTMaterialVideo";
    }

    @ReactProp(name = "material")
    public void setMaterial(VRTMaterialVideo view, String material) { view.setMaterial(material);}

    @ReactProp(name = "paused", defaultBoolean = true)
    public void setPaused(VRTMaterialVideo view, boolean paused) {
        view.setPaused(paused);
    }

    @ReactProp(name = "loop", defaultBoolean = false)
    public void setLoop(VRTMaterialVideo view, boolean loop) {
        view.setLoop(loop);
    }

    @ReactProp(name = "muted", defaultBoolean = false)
    public void setMuted(VRTMaterialVideo view, boolean muted) {
        view.setMuted(muted);
    }

    @ReactProp(name = "volume", defaultFloat = 1f)
    public void setVolume(VRTMaterialVideo view, float volume) {
        view.setVolume(volume);
    }


    @Override
    public @Nullable
    Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_BUFFER_START, MapBuilder.of("registrationName", ViroEvents.ON_BUFFER_START),
                ViroEvents.ON_BUFFER_END, MapBuilder.of("registrationName", ViroEvents.ON_BUFFER_END),
                ViroEvents.ON_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_FINISH),
                ViroEvents.ON_UPDATE_TIME, MapBuilder.of("registrationName", ViroEvents.ON_UPDATE_TIME),
                ViroEvents.ON_ERROR, MapBuilder.of("registrationName", ViroEvents.ON_ERROR)
        );
    }

    @Override
    public void receiveCommand(VRTMaterialVideo video, int commandType, @Nullable ReadableArray args) {
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

