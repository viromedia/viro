/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.facebook.react.uimanager.PixelUtil;

import com.viro.renderer.jni.BaseGeometry;
import com.viro.renderer.jni.EventDelegateJni;
import com.viro.renderer.jni.MaterialJni;
import com.viro.renderer.jni.NodeJni;
import com.viromedia.bridge.component.AnimatedComponent;
import com.viromedia.bridge.component.Component;
import com.viromedia.bridge.component.Light;
import com.viromedia.bridge.component.node.control.Image;
import com.viromedia.bridge.utility.ViroEvents;

import java.lang.ref.WeakReference;
import java.util.List;

import static com.viromedia.bridge.component.node.NodeManager.s2DUnitPer3DUnit;

/**
 * Node is inherited by any component which is represented by a VRONode in native
 */
public class Node extends Component {
    // Always place the children of views .01 in front of the parent. This helps with z-fighting
    // and ensures that the child is always in front of the parent for hit detection
    private static final float sZIncrementToAvoidZFighting = (float) 0.01;
    protected final static boolean DEFAULT_CAN_TAP = false;
    protected final static boolean DEFAULT_CAN_GAZE = false;
    private NodeJni mNodeJni;
    protected float[] mPosition;
    protected float[] mRotation;
    protected float[] mScale;
    protected float mOpacity;
    protected boolean mVisible;
    protected List<MaterialJni> mMaterials;
    private EventDelegateJni mEventDelegateJni;

    public Node(ReactApplicationContext reactContext) {
        this(reactContext.getBaseContext(), null, -1, -1, reactContext);
    }

    public Node(Context context, AttributeSet attrs, int defStyleAttr,
                     int defStyleRes, ReactApplicationContext reactContext) {
        super(context, attrs, defStyleAttr, defStyleRes, reactContext);
        mNodeJni = new NodeJni(context);

        // Create and attach callbacks.
        mEventDelegateJni = new EventDelegateJni();
        mEventDelegateJni.setEventDelegateCallback(new NodeEventDelegate(this));
        mNodeJni.setEventDelegateJni(mEventDelegateJni);
    }

    public NodeJni getNodeJni(){
        return mNodeJni;
    }

    @Override
    protected void onTearDown(){
        super.onTearDown();
        if (mNodeJni != null){
            mEventDelegateJni.setEventDelegateCallback(null);
            mEventDelegateJni.destroy();
            mNodeJni.destroy();
        }
    }

    // TODO: handle children separate from android viewgroup childViews
    @Override
    public void addView(View child, int index) {
        super.addView(child, index);

        if (child instanceof Light) {
            ((Light)child).addToNode(mNodeJni);
        } else if (child instanceof Node) {
            final Node childNode = (Node) child;
            mNodeJni.addChildNode(childNode.mNodeJni);
        } else if (child instanceof AnimatedComponent) {
            // We want the AnimatedComponent to be "invisible" to the scene graph so we need to add
            // the children of the AnimatedComponent to the parent of the AnimatedComponent natively.
            // Otherwise the parent of an AnimatedComponent has no idea that it has extra children
            // natively.
            // Note: AnimatedComponents can only have 1 child right now, so the loop is just
            // futureproofing
            AnimatedComponent animatedComponent = (AnimatedComponent) child;
            for (int i = 0; i < animatedComponent.getChildCount(); i++) {
                if (!(animatedComponent.getChildAt(i) instanceof Node)) {
                    continue;
                }

                Node animatedTarget = (Node) animatedComponent.getChildAt(i);
                if (!containsChild(animatedTarget)) {
                    addNativeChild(animatedTarget);
                }
            }
        } else {
            // TODO: Throw Error? Red Screen?
        }
    }

    @Override
    public void removeViewAt(int index) {

        View child = getChildAt(index);

        if (child instanceof Light) {

            ((Light)child).removeFromNode(mNodeJni);
        } else if (child instanceof Node) {

            final Node childNode = (Node) child;
            mNodeJni.removeChildNode(childNode.mNodeJni);
        } else if (child instanceof AnimatedComponent) {

            AnimatedComponent animatedComponent = (AnimatedComponent) child;

            for (int i = 0; i < animatedComponent.getChildCount(); i++) {

                if (!(animatedComponent.getChildAt(i) instanceof Node)) {

                    continue;
                }

                Node animatedTarget = (Node) animatedComponent.getChildAt(i);

                if (containsChild(animatedTarget)) {

                    removeNativeChild(animatedTarget);
                }
            }
        } else {
            // TODO: Throw Error? Red Screen?
        }

        super.removeViewAt(index);

    }

    public boolean containsChild(Node child) {
        return mNodeJni.containsChild(child.getNodeJni());
    }

    public void addNativeChild(Node child) {
        mNodeJni.addChildNode(child.getNodeJni());
    }

    public void removeNativeChild(Node child) {
        mNodeJni.removeChildNode(child.getNodeJni());
    }

    @Override
    public void recalcLayout() {
        // Nodes should attempt to recalc, but if it fails it should always call super.recalcLayout() to recurse.
        attemptRecalcLayout();
        super.recalcLayout();
    }

    protected void attemptRecalcLayout() {
        // Attempt to find a FlexView parent OR FlexView grandparent if the parent is an AnimatedComponent
        Node superView = null;
        if (getParent() != null && getParent() instanceof FlexView) {
            superView = (Node) getParent();
        } else if(getParent() != null && getParent() instanceof AnimatedComponent) {
            if (getParent().getParent() != null && getParent().getParent() instanceof FlexView) {
                superView = (Node) getParent().getParent();
            }
        }

        // if we failed to find an acceptable parent, just return.
        if (superView == null) {
            return;
        }

        float[] position = get2DPosition();
        float[] size = get2DSize();

        float[] parentPosition = superView.get2DPosition();
        float[] parentSize = superView.get2DSize();

        // flip y because in our 3D coordinates, y increases as you go up, rather than downwards on mobile/web.
        float transformedY = parentSize[1] - position[1];

        // transform by subtracting by center of superview
        float transformedX = position[0] - parentPosition[0];
        transformedY = transformedY - parentPosition[1];

        // we need the factor that the CSS algorithm applies to the original width/height.
        float pixelsPerDIP = PixelUtil.toPixelFromDIP(1.0);

        // calculate 3D size/position
        float width3d = size[0] / pixelsPerDIP / s2DUnitPer3DUnit;
        float height3d = size[1] / pixelsPerDIP / s2DUnitPer3DUnit;
        transformedX = transformedX / pixelsPerDIP / s2DUnitPer3DUnit;
        transformedY = transformedY / pixelsPerDIP / s2DUnitPer3DUnit;

        // set position.
        setPosition(new float[]{transformedX, transformedY, sZIncrementToAvoidZFighting});

        // set the size (either through scale or width/height if the node supports it).
        if (this instanceof Image) {
            Image image = (Image) this;
            image.setWidth(width3d);
            image.setHeight(height3d);
            image.onPropsSet();
        } else if (this instanceof FlexView) {
            FlexView flexView = (FlexView) this;
            flexView.setWidth(width3d);
            flexView.setHeight(height3d);
        } else {
            this.setScale(new float[]{width3d, height3d, 1});
        }

        // normalize the center point and bounds so that they assume left & top == 0;
        setPivotX(size[0]/2);
        setPivotY(size[1]/2);
        setLeft(0);
        setTop(0);
        setRight((int) size[0]);
        setBottom((int) size[1]);
    }

    protected void setPosition(float[] position) {
        if (position.length != 3){
            throw new IllegalArgumentException("Missing a position value: All three " +
                    "[x,y,z] axis value are needed.");
        }

        mPosition = position;
        mNodeJni.setPosition(position);
    }

    protected void setRotation(float[] rotation){
        if (rotation.length != 3){
            throw new IllegalArgumentException("Missing a rotation value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mRotation = rotation;
        mNodeJni.setRotation(rotation);
    }

    protected void setScale(float[] scale){
        if (scale.length != 3){
            throw new IllegalArgumentException("Missing a scale value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mScale = scale;
        mNodeJni.setScale(scale);
    }

    protected void setOpacity(float opacity) {
        mOpacity = opacity;
        mNodeJni.setOpacity(opacity);
    }

    protected void setVisible(boolean visible) {
        mVisible = visible;
        mNodeJni.setVisible(visible);
    }

    protected void setGeometry(BaseGeometry geometry) {
        mNodeJni.setGeometry(geometry);
        if (mMaterials != null) {
            mNodeJni.setMaterials(mMaterials);
        }
    }

    protected void setMaterials(List<MaterialJni> materials) {
        mMaterials = materials;
        mNodeJni.setMaterials(materials);
    }

    protected void setTransformBehaviors(String[] transformBehaviors) {
        mNodeJni.setTransformBehaviors(transformBehaviors);
    }

    protected float[] get2DPosition() {
        float[] arr = {getLeft() + getPivotX(), getTop() + getPivotY()};
        return arr;
    }

    protected float[] get2DSize() {
        float[] arr = {getRight() - getLeft(), getBottom() - getTop()};
        return arr;
    }

    protected void setCanGaze(boolean canGaze){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventType.ON_GAZE, canGaze);
    }

    protected void setCanTap(boolean canTap){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventType.ON_TAP, canTap);
    }

    private static class NodeEventDelegate implements EventDelegateJni.EventDelegateCallback {
        private WeakReference<Node> weakComponent;
        public NodeEventDelegate(Node node){
            weakComponent = new WeakReference<Node>(node);
        }

        @Override
        public void onTapped() {
            Node node = weakComponent.get();
            if (node == null){
                return;
            }

            node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                    node.getId(),
                    ViroEvents.ON_TAP,
                    null);
        }

        @Override
        public void onGaze(boolean isGazing) {
            Node node = weakComponent.get();
            if (node == null){
                return;
            }

            WritableMap event = Arguments.createMap();
            event.putBoolean("isGazing", isGazing);
            node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                    node.getId(),
                    ViroEvents.ON_GAZE,
                    event);
        }
    }
}
