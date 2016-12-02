/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.support.annotation.Nullable;

import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.annotations.ReactProp;

/**
 * Abstract NodeManager for setting {@link Node} Control properties.
 * NOTE: Always extend from this class for all Node Viro controls.
 */
public abstract class NodeManager <T extends Node> extends ViroViewGroupManager<T>{
    @ReactProp(name = "position", defaultFloat = 1f)
    public void setPosition(T view, ReadableArray sources) {
        view.setPosition(toFloatArray(sources));
    }

    @ReactProp(name = "rotation", defaultFloat = 1f)
    public void setRotation(Node view, ReadableArray sources) {
        view.setRotation(toFloatArray(sources));
    }

    @ReactProp(name = "scale", defaultFloat = 1f)
    public void setScale(Node view, ReadableArray sources) {
        view.setScale(toFloatArray(sources));
    }

    @ReactProp(name = "opacity", defaultFloat = 1f)
    public void setOpacity(Node view, float opacity) {
        view.setOpacity(opacity);
    }

    @ReactProp(name = "visible", defaultBoolean = true)
    public void setVisible(Node view, boolean visibility) {
        view.setVisible(visibility);
    }

    private static @Nullable float[] toFloatArray(@Nullable ReadableArray value) {
        if (value == null){
            throw new IllegalArgumentException("Can't convert a null ReadableArray to a float[].");
        }

        float[] result = new float[value.size()];
        for (int i = 0; i < value.size(); i++) {
            result[i] = (float) value.getDouble(i);
        }
        return result;
    }

    private static int toFloatArray(ReadableArray value, float[] into) {
        int length = value.size() > into.length ? into.length : value.size();
        for (int i = 0; i < length; i++) {
            into[i] = (float) value.getDouble(i);
        }
        return value.size();
    }
}
