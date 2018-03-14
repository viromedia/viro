/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.provider.MediaStore;

import com.facebook.react.bridge.Dynamic;
import com.facebook.react.bridge.DynamicFromMap;
import com.facebook.react.bridge.JavaOnlyMap;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableType;
import com.facebook.react.uimanager.LayoutShadowNode;
import com.facebook.react.uimanager.ViewProps;
import com.facebook.react.common.MapBuilder;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.facebook.react.uimanager.annotations.ReactPropGroup;
import com.facebook.yoga.YogaConstants;
import com.viro.core.Material;
import com.viro.core.VideoTexture;
import com.viromedia.bridge.component.VRTViroViewGroupManager;
import com.viromedia.bridge.module.MaterialManager;
import com.viromedia.bridge.module.MaterialManager.MaterialWrapper;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.ViroLog;

import java.util.ArrayList;
import java.util.Map;

import javax.annotation.Nullable;

/**
 * Abstract NodeManager for setting {@link VRTNode} Control properties.
 * NOTE: Always extend from this class for all Node Viro controls.
 */
public abstract class VRTNodeManager<T extends VRTNode> extends VRTViroViewGroupManager<T> {

    public static final float s2DUnitPer3DUnit = 1000;
    private static final String WIDTH_NAME = "width";
    private static final String HEIGHT_NAME = "height";
    private static final String PADDING_NAME = "padding";

    public VRTNodeManager(ReactApplicationContext context) {
        super(context);
    }

    @ReactProp(name = "position")
    public void setPosition(T view, ReadableArray position) {
        view.setPosition(Helper.toFloatArray(position));
    }

    @ReactProp(name = "rotation")
    public void setRotation(VRTNode view, ReadableArray rotation) {
        view.setRotation(Helper.toFloatArray(rotation));
    }

    @ReactProp(name = "scale")
    public void setScale(VRTNode view, ReadableArray scale) {
        view.setScale(Helper.toFloatArray(scale));
    }

    @ReactProp(name = "rotationPivot")
    public void setRotationPivot(VRTNode view, ReadableArray scale) {
        view.setRotationPivot(Helper.toFloatArray(scale));
    }

    @ReactProp(name = "scalePivot")
    public void setScalePivot(VRTNode view, ReadableArray scale) {
        view.setScalePivot(Helper.toFloatArray(scale));
    }

    @ReactProp(name = "opacity", defaultFloat = 1f)
    public void setOpacity(VRTNode view, float opacity) {
        view.setOpacity(opacity);
    }
    @ReactProp(name = "visible", defaultBoolean = true)
    public void setVisible(VRTNode view, boolean visibility) {
        view.setVisible(visibility);
    }

    @ReactProp(name = "canHover", defaultBoolean = VRTNode.DEFAULT_CAN_HOVER)
    public void setCanHover(VRTNode view, boolean canHover) {
        view.setCanHover(canHover);
    }

    @ReactProp(name = "canClick", defaultBoolean = VRTNode.DEFAULT_CAN_CLICK)
    public void setCanClick(VRTNode view, boolean canClick) {
        view.setCanClick(canClick);
    }

    @ReactProp(name = "canTouch", defaultBoolean = VRTNode.DEFAULT_CAN_TOUCH)
    public void setCanTouch(VRTNode view, boolean canTouch) {
        view.setCanTouch(canTouch);
    }

    @ReactProp(name = "canScroll", defaultBoolean = VRTNode.DEFAULT_CAN_SCROLL)
    public void setCanScroll(VRTNode view, boolean canScroll) {
        view.setCanScroll(canScroll);
    }

    @ReactProp(name = "canSwipe", defaultBoolean = VRTNode.DEFAULT_CAN_SWIPE)
    public void setCanSwipe(VRTNode view, boolean canSwipe) {
        view.setCanSwipe(canSwipe);
    }

    @ReactProp(name = "canDrag", defaultBoolean = VRTNode.DEFAULT_CAN_DRAG)
    public void setCanDrag(VRTNode view, boolean canDrag) {
        view.setCanDrag(canDrag);
    }

    @ReactProp(name = "canFuse", defaultBoolean = VRTNode.DEFAULT_CAN_FUSE)
    public void setCanFuse(VRTNode view, boolean canFuse) {
        view.setCanFuse(canFuse);
    }

    @ReactProp(name = "canPinch", defaultBoolean = VRTNode.DEFAULT_CAN_PINCH)
    public void setCanPinch(VRTNode view, boolean canPinch) {
        view.setCanPinch(canPinch);
    }

    @ReactProp(name = "canRotate", defaultBoolean = VRTNode.DEFAULT_CAN_ROTATE)
    public void setCanRotate(VRTNode view, boolean canRotate) {
        view.setCanRotate(canRotate);
    }

    @ReactProp(name = "timeToFuse", defaultFloat = VRTNode.DEFAULT_TIME_TO_FUSE_MILLIS)
    public void setTimeToFuse(VRTNode view, float durationMillis) {
        view.setTimeToFuse(durationMillis);
    }

    @ReactProp(name = "dragType")
    public void setDragType(VRTNode view, String dragType) {
        view.setDragType(dragType);
    }

    @ReactProp(name = "animation")
    public void setAnimation(VRTNode view, @android.support.annotation.Nullable ReadableMap map) {
        view.setAnimation(map);
    }

    @ReactProp(name = "materials")
    public void setMaterials(VRTNode view, @Nullable ReadableArray materials) {
        // get material manager
        MaterialManager materialManager = getContext().getNativeModule(MaterialManager.class);

        ArrayList<Material> nativeMaterials = new ArrayList<>();
        if (materials != null) {
            for (int i = 0; i < materials.size(); i++) {
                Material nativeMaterial = materialManager.getMaterial(materials.getString(i));
                if (materialManager.isVideoMaterial(materials.getString(i))) {
                    if (!(nativeMaterial.getDiffuseTexture() instanceof VideoTexture)) {
                        // Recreate the material with the proper context.
                        MaterialWrapper materialWrapper = materialManager.getMaterialWrapper(materials.getString(i));
                        VideoTexture videoTexture = new VideoTexture(view.getViroContext(), materialWrapper.getVideoTextureURI());
                        materialWrapper.recreate(videoTexture);
                        nativeMaterial = materialWrapper.getNativeMaterial();
                    }
                }

                if (nativeMaterial == null) {
                    throw new IllegalArgumentException("Material [" + materials.getString(i) + "] not found. Did you create it?");
                }

                nativeMaterials.add(nativeMaterial);
            }
        }
        view.setMaterials(nativeMaterials);
    }

    @ReactProp(name = "transformBehaviors")
    public void setTransformBehaviors(VRTNode view, @Nullable ReadableArray transformBehaviors) {

        String[] behaviors = new String[0];
        if (transformBehaviors != null) {
            behaviors = new String[transformBehaviors.size()];
            for (int i = 0; i < transformBehaviors.size(); i++) {
                behaviors[i] = transformBehaviors.getString(i);
            }
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
        private final String TAG = ViroLog.getTag(VRTNodeManager.class);

        @ReactProp(name = "width", defaultFloat = 1)
        public void setWidth(Dynamic width) {
            if (width.getType() == ReadableType.String) {
                super.setWidth(width);
            } else if (width.getType() == ReadableType.Number){
                JavaOnlyMap map = JavaOnlyMap.of(WIDTH_NAME, width.asDouble() * s2DUnitPer3DUnit);
                Dynamic newWidth = DynamicFromMap.create(map, WIDTH_NAME);
                super.setWidth(newWidth);
            } else {
                ViroLog.warn(TAG, "Width is not of type Number or String. Doing nothing.");
            }
        }

        @ReactProp(name = "height", defaultFloat = 1)
        public void setHeight(Dynamic height) {
            if (height.getType() == ReadableType.String) {
                super.setHeight(height);
            } else if (height.getType() == ReadableType.Number) {
                JavaOnlyMap map = JavaOnlyMap.of(HEIGHT_NAME, height.asDouble() * s2DUnitPer3DUnit);
                Dynamic newHeight = DynamicFromMap.create(map, HEIGHT_NAME);
                super.setHeight(newHeight);
            } else {
                ViroLog.warn(TAG, "Height is not of type Number or String. Doing nothing.");
            }
        }

        @ReactPropGroup(names = {
                ViewProps.PADDING,
                ViewProps.PADDING_VERTICAL,
                ViewProps.PADDING_HORIZONTAL,
                ViewProps.PADDING_LEFT,
                ViewProps.PADDING_RIGHT,
                ViewProps.PADDING_TOP,
                ViewProps.PADDING_BOTTOM,
        }, defaultFloat = YogaConstants.UNDEFINED)
        public void setPaddings(int index, Dynamic padding) {
            if (padding.getType() == ReadableType.String) {
                super.setPaddings(index, padding);
            } else if (padding.getType() == ReadableType.Number) {
                JavaOnlyMap map = JavaOnlyMap.of(PADDING_NAME, padding.asDouble() * s2DUnitPer3DUnit);
                Dynamic newPadding = DynamicFromMap.create(map, PADDING_NAME);
                super.setPaddings(index, newPadding);
            } else {
                ViroLog.warn(TAG, "Padding is not of type Number of String. Doing nothing.");
            }
        }

        @ReactPropGroup(names = {
                ViewProps.BORDER_WIDTH,
                ViewProps.BORDER_LEFT_WIDTH,
                ViewProps.BORDER_RIGHT_WIDTH,
                ViewProps.BORDER_TOP_WIDTH,
                ViewProps.BORDER_BOTTOM_WIDTH,
        }, defaultFloat = YogaConstants.UNDEFINED)
        public void setBorderWidths(int index, float borderWidth) {
            super.setBorderWidths(index, borderWidth * s2DUnitPer3DUnit);
        }
    }

    @Override
    public Map getExportedCustomDirectEventTypeConstants() {
        Map map =  MapBuilder.of( ViroEvents.ON_HOVER, MapBuilder.of("registrationName", ViroEvents.ON_HOVER));
        map.put(ViroEvents.ON_CLICK, MapBuilder.of("registrationName", ViroEvents.ON_CLICK));
        map.put(ViroEvents.ON_TOUCH, MapBuilder.of("registrationName", ViroEvents.ON_TOUCH));
        map.put(ViroEvents.ON_SWIPE, MapBuilder.of("registrationName", ViroEvents.ON_SWIPE));
        map.put(ViroEvents.ON_SCROLL, MapBuilder.of("registrationName", ViroEvents.ON_SCROLL));
        map.put(ViroEvents.ON_FUSE, MapBuilder.of("registrationName", ViroEvents.ON_FUSE));
        map.put(ViroEvents.ON_PINCH, MapBuilder.of("registrationName", ViroEvents.ON_PINCH));
        map.put(ViroEvents.ON_ROTATE, MapBuilder.of("registrationName", ViroEvents.ON_ROTATE));
        map.put(ViroEvents.ON_DRAG, MapBuilder.of("registrationName", ViroEvents.ON_DRAG));
        map.put(ViroEvents.ON_COLLIDED, MapBuilder.of("registrationName", ViroEvents.ON_COLLIDED));
        map.put(ViroEvents.ON_TRANSFORM_DELEGATE, MapBuilder.of("registrationName", ViroEvents.ON_TRANSFORM_DELEGATE));
        map.put(ViroEvents.ON_ANIMATION_START, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_START));
        map.put(ViroEvents.ON_ANIMATION_FINISH, MapBuilder.of("registrationName", ViroEvents.ON_ANIMATION_FINISH));
        return map;
    }

    @ReactProp(name = "physicsBody")
    public void setPhysicsBody(VRTNode view, ReadableMap map) {
        view.setPhysicsBody(map);
    }

    @ReactProp(name = "canCollide", defaultBoolean = VRTNode.DEFAULT_CAN_FUSE)
    public void setCanCollide(VRTNode view, boolean canCollide) {
        view.setCanCollide(canCollide);
    }

    @ReactProp(name = "viroTag")
    public void setViroTag(VRTNode view, String tag) {
        view.setViroTag(tag);
    }

    @ReactProp(name = "hasTransformDelegate", defaultBoolean = false)
    public void setViroTag(VRTNode view, boolean hasDelegate) {
        view.setOnNativeTransformDelegate(hasDelegate);
    }

}
