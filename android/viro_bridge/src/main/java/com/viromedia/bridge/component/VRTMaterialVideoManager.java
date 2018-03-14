package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.control.VRTVideoSurface;

/**
 * Created by vadvani on 3/12/18.
 */

public class VRTMaterialVideoManager extends VRTViroViewGroupManager<VRTMaterialVideo> {

    public VRTMaterialVideoManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    protected VRTMaterialVideo createViewInstance(ThemedReactContext reactContext) {
        return new VRTMaterialVideo(getContext());
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

}
