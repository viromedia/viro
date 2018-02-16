/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.VRTNodeManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.Map;

public class VRTSurfaceManager extends VRTNodeManager<VRTSurface> {

    public VRTSurfaceManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSurface";
    }

    @Override
    protected VRTSurface createViewInstance(ThemedReactContext reactContext) {
        return new VRTSurface(getContext());
    }

    @ReactProp(name = "width", defaultFloat = 1)
    public void setWidth(VRTSurface view, float width) {
        view.setWidth(width);
    }

    @ReactProp(name = "height", defaultFloat = 1)
    public void setHeight(VRTSurface view, float height) {
        view.setHeight(height);
    }

    @ReactProp(name = "uvCoordinates")
    public void setUVCoordinates(VRTSurface view, ReadableArray coordinates) {
        float u0 = 0; float v0 = 0; float u1 = 1; float v1 = 1;

        if (coordinates == null) {
            // do-nothing
        } else if (coordinates.size() != 4) {
            throw new IllegalArgumentException("[ViroSurface] Expected 4 uv coordinates, got " + coordinates.size());
        } else { // not null && has 4 elements
            u0 = (float) coordinates.getDouble(0);
            v0 = (float) coordinates.getDouble(1);
            u1 = (float) coordinates.getDouble(2);
            v1 = (float) coordinates.getDouble(3);
        }

        view.setUVCoordinates(u0, v0, u1, v1);
    }

    @ReactProp(name = "lightReceivingBitMask", defaultInt = 1)
    public void setLightReceivingBitMask(VRTSurface view, int bitMask) {view.setLightReceivingBitMask(bitMask); }

    @ReactProp(name = "shadowCastingBitMask", defaultInt = 1)
    public void setShadowCastingBitMask(VRTSurface view, int bitMask) {view.setShadowCastingBitMask(bitMask); }

    @ReactProp(name = "arShadowReceiver", defaultBoolean = false)
    public void setARShadowReceiver(VRTSurface view, boolean arShadowReceiver) { view.setARShadowReceiver(arShadowReceiver); }

    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START),
                ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH));
    }
}
