/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.PixelUtil;

import com.viro.renderer.jni.BaseGeometry;
import com.viro.renderer.jni.EventDelegateJni;
import com.viro.renderer.jni.MaterialJni;
import com.viro.renderer.jni.NodeJni;
import com.viromedia.bridge.component.AnimatedComponent;
import com.viromedia.bridge.component.Component;
import com.viromedia.bridge.component.Light;
import com.viromedia.bridge.component.node.control.Image;
import com.viromedia.bridge.component.node.control.Surface;
import com.viromedia.bridge.component.node.control.Text;
import com.viromedia.bridge.utility.ComponentEventDelegate;

import java.util.List;

import static com.viromedia.bridge.component.node.NodeManager.s2DUnitPer3DUnit;

/**
 * Node is inherited by any component which is represented by a VRONode in native
 */
public class Node extends Component {
    // Always place the children of views .01 in front of the parent. This helps with z-fighting
    // and ensures that the child is always in front of the parent for hit detection
    private static final float sZIncrementToAvoidZFighting = (float) 0.01;
    protected final static boolean DEFAULT_CAN_HOVER = false;
    protected final static boolean DEFAULT_CAN_CLICK = false;
    protected final static boolean DEFAULT_CAN_TOUCH = false;
    protected final static boolean DEFAULT_CAN_SCROLL = false;
    protected final static boolean DEFAULT_CAN_SWIPE = false;
    public final static boolean DEFAULT_HIGH_ACCURACY_GAZE = false;
    protected final static boolean DEFAULT_CAN_DRAG = false;

    private NodeJni mNodeJni;
    protected float[] mPosition;
    protected float[] mRotation;
    protected float[] mScale;
    protected float mOpacity;
    protected boolean mVisible;
    protected boolean mHighAccuracyGazeEnabled;
    protected List<MaterialJni> mMaterials;
    private EventDelegateJni mEventDelegateJni;

    // these are used to preserve the old 2D layout values which we'll store before and restore after
    // calling attemptRecalcLayout
    protected int mOldLeft;
    protected int mOldTop;
    protected int mOldRight;
    protected int mOldBottom;

    /**
     * This field is set to true when during a property setting iteration, this Node is one of the
     * highest elements in the scene tree that had its properties set/changed.
     */
    protected boolean mIsTopMostChangedElement = false;

    public Node(ReactApplicationContext reactContext) {
        this(reactContext.getBaseContext(), null, -1, -1, reactContext);
    }

    public Node(Context context, AttributeSet attrs, int defStyleAttr,
                     int defStyleRes, ReactApplicationContext reactContext) {
        super(context, attrs, defStyleAttr, defStyleRes, reactContext);
        mNodeJni = new NodeJni(context);

        // Create and attach callbacks.
        mEventDelegateJni = new EventDelegateJni();
        mEventDelegateJni.setEventDelegateCallback(new ComponentEventDelegate(this));
        mNodeJni.setEventDelegateJni(mEventDelegateJni);
    }

    public NodeJni getNodeJni(){
        return mNodeJni;
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mNodeJni != null){
            mEventDelegateJni.setEventDelegateCallback(null);
            mEventDelegateJni.destroy();
            mNodeJni.destroy();
            mNodeJni = null;
        }
    }

    // TODO: handle children separate from android viewgroup childViews
    @Override
    public void addView(View child, int index) {
        super.addView(child, index);

        if (child instanceof Light) {
            ((Light) child).addToNode(mNodeJni);
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
            ((Light) child).removeFromNode(mNodeJni);
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

        /*
         We tear down views whenever they're removed from the tree.
         */
        if (child instanceof Component) {
            Component component = (Component) child;
            //component.onTearDown();
        }
    }

    /*
     * Determines non-recursively if the child is within the given parent viewgroup.
     */
    public boolean containsChild(Node child) {
        for (int i = 0; i < this.getChildCount(); i ++) {
            View view = getChildAt(i);
            if (view == child) {
                return true;
            }
        }
        return false;
    }

    public void addNativeChild(Node child) {
        mNodeJni.addChildNode(child.getNodeJni());
    }

    public void removeNativeChild(Node child) {
        mNodeJni.removeChildNode(child.getNodeJni());
    }

    @Override
    protected void onPropsSet() {
        super.onPropsSet();
        // Because props are set from the bottom of the scene tree up, if props are set on this
        // Node, then we know its the top-most changed element.
        mIsTopMostChangedElement = true;
        setChildrenNotTopMostChangedElement();
    }

    /**
     * This function ensures that the children of this element aren't the top-most changed element.
     */
    protected void setChildrenNotTopMostChangedElement() {
        for (int i = 0; i < getChildCount(); i++) {
            View child = getChildAt(i);
            if (child instanceof Node) {
                ((Node) child).mIsTopMostChangedElement = false;
                ((Node) child).setChildrenNotTopMostChangedElement();
            }
        }
    }

    @Override
    public void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        // Only call recalcLayout if we're the top-most changed element because we don't want to
        // recalc the 3D position/size N times (N = number of changed elements).
        if (mIsTopMostChangedElement) {
            recalcLayout();
        }
    }

    @Override
    public void recalcLayout() {
        // First thing we should do is store our old 2d bounds
        store2DBounds();

        // Nodes should attempt to recalc, but if it fails it should always call super.recalcLayout() to recurse.
        attemptRecalcLayout();
        super.recalcLayout();

        // once we've laid out all the children, we need to restore the 2D bounds
        restore2DBounds();
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

        // TODO: create Flexible interface
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
        } else if (this instanceof Surface) {
            Surface surface = (Surface) this;
            surface.setWidth(width3d);
            surface.setHeight(height3d);
            surface.updateSurface();
        } else if (this instanceof Text) {
            Text text = (Text) this;
            text.setWidth(width3d);
            text.setHeight(height3d);
            text.onPropsSet();
        } else {
            this.setScale(new float[]{width3d, height3d, 1});
        }

        // normalize bounds so that they assume left & top == 0, we shouldn't set the pivots because
        // 1) they're already correct
        // 2) if you set pivots, the view will cease to use its default (center) and use the value you
        //    give it (see View's setPivotX/setPivotY javadocs)
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

    protected void setRotation(float[] rotation) {
        if (rotation.length != 3){
            throw new IllegalArgumentException("Missing a rotation value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mRotation = rotation;
        mNodeJni.setRotation(rotation);
    }

    protected void setScale(float[] scale) {
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

    public void setHighAccuracyGaze(boolean highAccuracyGazeEnabled){
        mHighAccuracyGazeEnabled = highAccuracyGazeEnabled;
        mNodeJni.setHighAccuracyGaze(highAccuracyGazeEnabled);
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

    protected void store2DBounds() {
        mOldLeft = getLeft();
        mOldTop = getTop();
        mOldRight = getRight();
        mOldBottom = getBottom();
    }

    protected void restore2DBounds() {
        setLeft(mOldLeft);
        setTop(mOldTop);
        setRight(mOldRight);
        setBottom(mOldBottom);
    }

    protected void setCanHover(boolean canHover){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_HOVER, canHover);
    }

    protected void setCanClick(boolean canClick){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_CLICK, canClick);
    }

    protected void setCanTouch(boolean canTouch){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_TOUCH, canTouch);
    }

    protected void setCanScroll(boolean canScroll){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_SCROLL, canScroll);
    }

    protected void setCanSwipe(boolean canSwipe){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_SWIPE, canSwipe);
    }

    protected void setCanDrag(boolean canDrag){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_DRAG, canDrag);
    }
}
