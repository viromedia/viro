/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.support.annotation.Nullable;

import com.facebook.csslayout.CSSConstants;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.LayoutShadowNode;
import com.facebook.react.uimanager.ViewProps;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.facebook.react.uimanager.annotations.ReactPropGroup;
import com.viro.renderer.jni.MaterialJni;
import com.viromedia.bridge.component.ViroViewGroupManager;
import com.viromedia.bridge.module.MaterialManager;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.ArrayList;
import java.util.Map;

/**
 * Abstract NodeManager for setting {@link Node} Control properties.
 * NOTE: Always extend from this class for all Node Viro controls.
 */
public abstract class NodeManager <T extends Node> extends ViroViewGroupManager<T> {

    public static final float s2DUnitPer3DUnit = 1000;

    public NodeManager(ReactApplicationContext context) {
        super(context);
    }

    @ReactProp(name = "position")
    public void setPosition(T view, ReadableArray position) {
        view.setPosition(Helper.toFloatArray(position));
    }

    @ReactProp(name = "rotation")
    public void setRotation(Node view, ReadableArray rotation) {
        view.setRotation(Helper.toFloatArray(rotation));
    }

    @ReactProp(name = "scale")
    public void setScale(Node view, ReadableArray scale) {
        view.setScale(Helper.toFloatArray(scale));
    }

    @ReactProp(name = "opacity", defaultFloat = 1f)
    public void setOpacity(Node view, float opacity) {
        view.setOpacity(opacity);
    }

    @ReactProp(name = "visible", defaultBoolean = true)
    public void setVisible(Node view, boolean visibility) {
        view.setVisible(visibility);
    }

    @ReactProp(name = "canTap", defaultBoolean = Node.DEFAULT_CAN_TAP)
    public void setCanTap(Node view, boolean canTap) {
        view.setCanTap(canTap);
    }

    @ReactProp(name = "canGaze", defaultBoolean = Node.DEFAULT_CAN_GAZE)
    public void setCanGaze(Node view, boolean canGaze) {
        view.setCanGaze(canGaze);
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

    @Override
    public LayoutShadowNode createShadowNodeInstance() {
        return new FlexEnabledShadowNode();
    }

    @Override
    public Class<? extends LayoutShadowNode> getShadowNodeClass() {
        return FlexEnabledShadowNode.class;
    }

    /**
     * This shadow node is so that views associated with FlexViews (and FlexViews themselves) have
     * their properties properly converted from 3D to 2D units. It's easiest if we just make all Nodes
     * have FlexEnabledShadowNodes, and the components can choose whether or not
     */
    protected class FlexEnabledShadowNode extends ViroLayoutShadowNode {

        @ReactProp(name = "width", defaultFloat = 1)
        public void setWidth(float width) {
            super.setWidth(width * s2DUnitPer3DUnit);
        }

        @ReactProp(name = "height", defaultFloat = 1)
        public void setHeight(float height) {
            super.setHeight(height * s2DUnitPer3DUnit);
        }

        @ReactPropGroup(names = {
                ViewProps.PADDING,
                ViewProps.PADDING_VERTICAL,
                ViewProps.PADDING_HORIZONTAL,
                ViewProps.PADDING_LEFT,
                ViewProps.PADDING_RIGHT,
                ViewProps.PADDING_TOP,
                ViewProps.PADDING_BOTTOM,
        }, defaultFloat = CSSConstants.UNDEFINED)
        public void setPaddings(int index, float padding) {
            super.setPaddings(index, padding * s2DUnitPer3DUnit);
        }

        @ReactPropGroup(names = {
                ViewProps.BORDER_WIDTH,
                ViewProps.BORDER_LEFT_WIDTH,
                ViewProps.BORDER_RIGHT_WIDTH,
                ViewProps.BORDER_TOP_WIDTH,
                ViewProps.BORDER_BOTTOM_WIDTH,
        }, defaultFloat = CSSConstants.UNDEFINED)
        public void setBorderWidths(int index, float borderWidth) {
            super.setBorderWidths(index, borderWidth * s2DUnitPer3DUnit);
        }
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        return MapBuilder.of(
                ViroEvents.ON_GAZE, MapBuilder.of("registrationName", ViroEvents.ON_GAZE),
                ViroEvents.ON_TAP, MapBuilder.of("registrationName", ViroEvents.ON_TAP));
    }
}
