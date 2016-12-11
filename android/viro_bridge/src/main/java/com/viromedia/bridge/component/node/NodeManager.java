/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.support.annotation.Nullable;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viro.renderer.jni.MaterialJni;
import com.viromedia.bridge.component.ViroViewGroupManager;
import com.viromedia.bridge.module.MaterialManager;

import java.util.ArrayList;

/**
 * Abstract NodeManager for setting {@link Node} Control properties.
 * NOTE: Always extend from this class for all Node Viro controls.
 */
public abstract class NodeManager <T extends Node> extends ViroViewGroupManager<T> {

    public NodeManager(ReactApplicationContext context) {
        super(context);
    }

    @ReactProp(name = "position")
    public void setPosition(T view, ReadableArray position) {
        view.setPosition(toFloatArray(position));
    }

    @ReactProp(name = "rotation")
    public void setRotation(Node view, ReadableArray rotation) {
        view.setRotation(toFloatArray(rotation));
    }

    @ReactProp(name = "scale")
    public void setScale(Node view, ReadableArray scale) {
        view.setScale(toFloatArray(scale));
    }

    @ReactProp(name = "opacity", defaultFloat = 1f)
    public void setOpacity(Node view, float opacity) {
        view.setOpacity(opacity);
    }

    @ReactProp(name = "visible", defaultBoolean = true)
    public void setVisible(Node view, boolean visibility) {
        view.setVisible(visibility);
    }

    @ReactProp(name = "materials")
    public void setMaterials(Node view, ReadableArray materials) {
        // get material manager
        MaterialManager materialManager = getContext().getNativeModule(MaterialManager.class);

        ArrayList<MaterialJni> nativeMaterials = new ArrayList<>();
        for (int i = 0; i < materials.size(); i++) {
            MaterialJni nativeMaterial = materialManager.getMaterial(materials.getString(i));
            if (nativeMaterial == null) {
                throw new IllegalArgumentException("Material [" + materials.getString(i) + "] not found. Did you create it?");
            }
            nativeMaterials.add(nativeMaterial);
        }
        view.setMaterials(nativeMaterials);
    }

    @ReactProp(name = "transformBehaviors")
    public void setTransformBehaviors(Node view, ReadableArray transformBehaviors) {
        String[] behaviors = new String[transformBehaviors.size()];
        for (int i = 0; i < transformBehaviors.size(); i++) {
            behaviors[i] = transformBehaviors.getString(i);
        }
        view.setTransformBehaviors(behaviors);
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
