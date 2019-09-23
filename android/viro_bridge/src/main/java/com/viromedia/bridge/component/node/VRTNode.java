//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

package com.viromedia.bridge.component.node;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.view.ViewParent;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.JSApplicationCausedNativeException;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableType;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.IllegalViewOperationException;
import com.facebook.react.uimanager.PixelUtil;

import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.ARNode;
import com.viro.core.ARScene;
import com.viro.core.Geometry;
import com.viro.core.EventDelegate;
import com.viro.core.Material;
import com.viro.core.Node;
import com.viro.core.VideoTexture;
import com.viro.core.ViroContext;
import com.viro.core.internal.ExecutableAnimation;
import com.viro.core.PhysicsBody;
import com.viro.core.PhysicsShape;
import com.viro.core.PhysicsShapeAutoCompound;
import com.viro.core.PhysicsShapeBox;
import com.viro.core.PhysicsShapeSphere;
import com.viro.core.Vector;
import com.viromedia.bridge.component.VRTAnimatedComponent;
import com.viromedia.bridge.component.VRTComponent;
import com.viromedia.bridge.component.VRTLight;
import com.viromedia.bridge.component.VRTManagedAnimation;
import com.viromedia.bridge.component.node.control.VRTAnimatedImage;
import com.viromedia.bridge.component.node.control.VRTImage;
import com.viromedia.bridge.component.node.control.VRTQuad;
import com.viromedia.bridge.component.node.control.VRTText;
import com.viromedia.bridge.component.node.control.VRTVideoSurface;
import com.viromedia.bridge.module.AnimationManager;
import com.viromedia.bridge.module.MaterialManager;
import com.viromedia.bridge.utility.ComponentEventDelegate;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.ViroLog;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.EnumSet;
import java.util.List;


import static com.viromedia.bridge.component.node.VRTNodeManager.s2DUnitPer3DUnit;

/**
 * Node is inherited by any component which is represented by a VRONode in native
 */
public class VRTNode extends VRTComponent {
    private static final String TAG = "Viro";
    private static final boolean DEBUG_ANCHORING = false;

    /**
     * If an anchored node is moved to a position less than this value away from the
     * anchor's position, then the VRONode will simply be moved. If the node is moved
     * further than this value, then we'll detach the existing anchor and attempt to
     * re-anchor the node.
     */
    private static final float REANCHOR_DISTANCE = 3.5f;

    /*
     * The amount of time to wait before attempting to anchor a Node again after an anchor
     * failure.
     */
    private static final long ANCHOR_DELAY_MS = 1000;

    /**
     * The maximum number of attempts to make when anchoring a Node.
     */
    private static final int MAX_ANCHORING_ATTEMPTS = 3;

    private static final class AnchorAttempt {

        private int mAttempt;
        private Vector mPosition;
        private WeakReference<VRTNode> mNode;
        private Handler mHandler;
        private boolean mCanceled = false;

        public AnchorAttempt(VRTNode node, Vector position) {
            mNode = new WeakReference<VRTNode>(node);
            mAttempt = 0;
            mPosition = position;
            mHandler = new Handler(Looper.getMainLooper());
        }

        public void makeAttempt() {
            VRTNode node = mNode.get();
            if (node == null) {
                return;
            }
            if (mCanceled) {
                if (DEBUG_ANCHORING) {
                    Log.w(TAG, "Anchor attempt canceled for " + node + "");
                }
                return;
            }
            if (mAttempt >= MAX_ANCHORING_ATTEMPTS) {
                if (DEBUG_ANCHORING) {
                    Log.w(TAG, "Failed to anchor node " + node + ": will float node without an anchor");
                }
                return;
            }
            boolean success = anchorNode(node, mPosition);
            ++mAttempt;

            if (!success) {
                // On failure, try again after a delay
                mHandler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        makeAttempt();
                    }
                }, ANCHOR_DELAY_MS);
            } else {
                // On success, remove the anchor attempt from the node
                node.mAnchorAttempt = null;
            }
        }

        /**
         * Anchor this node by creating an anchored node at the given position, then parenting this
         * node with the anchor and setting this node to the origin. Note that this node's current
         * position is ignored. If anchoring fails, then the node is simply set to the position, and
         * we try again later.
         *
         * @param position The position at which to anchor the node.
         * @return True if anchoring succeeded.
         */
        private boolean anchorNode(final VRTNode node, final Vector position) {
            if (DEBUG_ANCHORING) {
                Log.i(TAG, "Anchoring node " + node + " [attempt " + mAttempt + "]");
            }

            if (node == null || node.isTornDown() || node.getParent() == null) {
                return false;
            }

            final ARScene scene = (ARScene) ((VRTARScene) node.getParent()).getNativeScene();
            if (node.mViroContext == null) {
                if (DEBUG_ANCHORING) {
                    Log.i(TAG, "   Delaying anchoring: ViroContext is null");
                }
                return false;
            }

            node.mAnchor = scene.createAnchoredNode(position);

            if (node.mAnchor != null) {
                if (DEBUG_ANCHORING) {
                    Log.i(TAG, "Anchor created successfully at position " + position + ", repositioning Node to origin");
                }

                node.mNodeJni.removeFromParentNode();
                scene.getRootNode().addChildNode(node.mAnchor);
                node.mAnchor.addChildNode(node.mNodeJni);
                node.mNodeJni.setPosition(new Vector(0, 0, 0));

                return true;
            } else {
                // Anchor failed: simply set the node to the position and try again later
                if (DEBUG_ANCHORING) {
                    Log.i(TAG, "Failed to anchor node at " + position + " -- trying again later");
                }
                node.mNodeJni.setPosition(position);

                return false;
            }
        }

        public void setPosition(Vector position) {
            mPosition = position;
        }

        public void cancel() {
            mCanceled = true;
        }
    }

    public static class NodeAnimation extends VRTManagedAnimation {

        protected String mAnimationName;
        private AnimationManager mAnimationManager;

        public NodeAnimation(ReactContext context, VRTNode parent) {
            super(context, parent);
            super.setNode(parent);
            mAnimationManager = context.getNativeModule(AnimationManager.class);
        }

        public void setAnimationName(String name) {
            mAnimationName = name;
        }
        public String getAnimationName() { return mAnimationName; }

        @Override
        public ExecutableAnimation loadAnimation() {
            if (mAnimationName != null) {
                ExecutableAnimation animation = mAnimationManager.getAnimation(mAnimationName);
                if (animation != null) {
                    return animation.copy();
                }
                else {
                    return null;
                }
            }
            else {
                return null;
            }
        }
    }

    // Always place the children of views .01 in front of the parent. This helps with z-fighting
    // and ensures that the child is always in front of the parent for hit detection
    private static final float sZIncrementToAvoidZFighting = (float) 0.01;
    protected final static boolean DEFAULT_CAN_HOVER = false;
    protected final static boolean DEFAULT_CAN_CLICK = false;
    protected final static boolean DEFAULT_CAN_TOUCH = false;
    protected final static boolean DEFAULT_CAN_SCROLL = false;
    protected final static boolean DEFAULT_CAN_SWIPE = false;
    public final static boolean DEFAULT_HIGH_ACCURACY_EVENTS = false;
    protected final static boolean DEFAULT_CAN_DRAG = false;
    protected final static boolean DEFAULT_CAN_FUSE = false;
    protected final static boolean DEFAULT_CAN_PINCH = false;
    protected final static boolean DEFAULT_CAN_ROTATE = false;
    protected final static boolean DEFAULT_CAN_CAMERA_HIT_TEST = false;
    protected final static boolean DEFAULT_CAN_AR_POINT_CLOUD_UPDATE = false;
    protected final static boolean DEFAULT_CAN_CAMERA_TRANSFORM_UPDATE = false;
    protected final static float DEFAULT_TIME_TO_FUSE_MILLIS = 1000f;
    protected final static double TRANSFORM_DELEGATE_DISTANCE_FILTER = 0.01;
    protected final static boolean DEFAULT_IGNORE_EVENT_HANDLING = false;

    private Node mNodeJni;
    private ARNode mAnchor;
    private AnchorAttempt mAnchorAttempt;
    protected float[] mPosition;
    protected float[] mRotation;
    protected float[] mScale;
    protected float[] mRotationPivot;
    protected float[] mScalePivot;
    protected float mOpacity = 1.0f;
    protected boolean mVisible = true; // default visible value should be true
    protected int mRenderingOrder = 0;
    protected boolean mHighAccuracyEventsEnabled = false;
    protected boolean mIgnoreEventHandling = false;

    protected int mLightReceivingBitMask = 1;
    protected int mShadowCastingBitMask = 1;

    protected List<Material> mMaterials;
    protected EventDelegate mEventDelegateJni;
    private ComponentEventDelegate mComponentEventDelegate;
    private NodeTransformDelegate mTransformDelegate;
    protected NodeAnimation mNodeAnimation;

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

    // True if this node initialized and contains a physics body.
    private boolean hasPhysicsBody = false;
    private boolean mPhysicsEnabled = true;

    // Last known set physics properties for this node.
    private ReadableMap mPhysicsMap = null;
    private PhysicsBodyDelegate mPhysicsDelegate;

    public VRTNode(ReactContext reactContext) {
        this(reactContext.getBaseContext(), null, -1, -1, reactContext);
    }

    public VRTNode(Context context, AttributeSet attrs, int defStyleAttr,
                   int defStyleRes, ReactContext reactContext) {
        super(context, attrs, defStyleAttr, defStyleRes, reactContext);
        mNodeJni = createNodeJni();

        // Create and attach callbacks.
        mComponentEventDelegate = new ComponentEventDelegate(this);
        mEventDelegateJni = new EventDelegate();
        mEventDelegateJni.setEventDelegateCallback(mComponentEventDelegate);
        mNodeJni.setEventDelegate(mEventDelegateJni);

        mNodeAnimation = new NodeAnimation(reactContext, this);

        // Default position, rotation and scale values.
        mPosition = new float[] {0, 0, 0};
        mRotation = new float[] {0, 0, 0};
        mScale = new float[] {1, 1, 1};
        mRotationPivot = new float[] {0, 0, 0};
        mScalePivot = new float[] {0, 0, 0};
    }

    /**
     This function creates a Node object. Child classes should override to provide
     their own Node.
     */
    protected Node createNodeJni() {
        return new Node();
    }

    public Node getNodeJni() {
        return mNodeJni;
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mNodeJni != null){
            mTransformDelegate = null;
            clearPhysicsBody();
            mEventDelegateJni.setEventDelegateCallback(null);
            mEventDelegateJni.dispose();
            mNodeJni.dispose();
            mNodeJni = null;
        }
    }

    // TODO: handle children separate from android viewgroup childViews
    @Override
    public void addView(View child, int index) {
        if (!isTornDown()) {
            if (child instanceof VRTLight) {
                ((VRTLight) child).addToNode(mNodeJni);

            } else if (child instanceof VRTCamera) {
                final VRTCamera cameraView = (VRTCamera)child;
                mNodeJni.addChildNode(cameraView.getNodeRootTransformCamera().getNodeJni());
            } else if (child instanceof VRTNode) {
                final VRTNode childNode = (VRTNode) child;
                mNodeJni.addChildNode(childNode.mNodeJni);
            } else if (child instanceof VRTAnimatedComponent) {
                // We want the AnimatedComponent to be "invisible" to the scene graph so we need to add
                // the children of the AnimatedComponent to the parent of the AnimatedComponent natively.
                // Otherwise the parent of an AnimatedComponent has no idea that it has extra children
                // natively.
                // Note: AnimatedComponents can only have 1 child right now, so the loop is just
                // futureproofing
                VRTAnimatedComponent animatedComponent = (VRTAnimatedComponent) child;
                for (int i = 0; i < animatedComponent.getChildCount(); i++) {
                    if (!(animatedComponent.getChildAt(i) instanceof VRTNode)) {
                        continue;
                    }

                    VRTNode animatedTarget = (VRTNode) animatedComponent.getChildAt(i);
                    if (!containsChild(animatedTarget)) {
                        addNativeChild(animatedTarget);
                    }
                }
            } else {
                // TODO: Throw Error? Red Screen?
            }
        }

        super.addView(child, index);
    }

    @Override
    public void removeViewAt(int index) {
        if (!isTornDown()) {
            View child = getChildAt(index);
            if (child instanceof VRTLight) {
                ((VRTLight) child).removeFromNode(mNodeJni);
            } else if (child instanceof VRTCamera) {
                final VRTCamera childNode = (VRTCamera) child;
                childNode.getNodeRootTransformCamera().getNodeJni().removeFromParentNode();
            } else if (child instanceof VRTNode) {
                final VRTNode childNode = (VRTNode) child;
                childNode.mNodeJni.removeFromParentNode();
            } else if (child instanceof VRTAnimatedComponent) {
                VRTAnimatedComponent animatedComponent = (VRTAnimatedComponent) child;
                for (int i = 0; i < animatedComponent.getChildCount(); i++) {
                    if (!(animatedComponent.getChildAt(i) instanceof VRTNode)) {
                        continue;
                    }

                    VRTNode animatedTarget = (VRTNode) animatedComponent.getChildAt(i);
                    if (containsChild(animatedTarget)) {
                        removeNativeChild(animatedTarget);
                    }
                }
            } else {
                // TODO: Throw Error? Red Screen?
            }
        }

        super.removeViewAt(index);
    }

    /*
     * Determines non-recursively if the child is within the given parent viewgroup.
     */
    public boolean containsChild(VRTNode child) {
        for (int i = 0; i < this.getChildCount(); i ++) {
            View view = getChildAt(i);
            if (view == child) {
                return true;
            }
        }
        return false;
    }

    public void addNativeChild(VRTNode child) {
        if (isTornDown()) {
            return;
        }
        mNodeJni.addChildNode(child.getNodeJni());
    }

    public void removeNativeChild(VRTNode child) {
        if (isTornDown()) {
            return;
        }
        child.getNodeJni().removeFromParentNode();
    }

    @Override
    public void onTreeUpdate() {
        super.onTreeUpdate();

        /*
         If this node is an anchor itself (e.g. a plane), then don't parent it with an anchor, as it's
         already tracked by ARCore.
         */
        if (mNodeJni == null || mNodeJni instanceof ARNode) {
            return;
        }

        /*
         If there is already an anchoring attempt in process, cancel it.
         */
        if (mAnchorAttempt != null) {
            mAnchorAttempt.cancel();
        }

        /*
         If this node is at the root of an ARScene, then we will try to create an anchor for the
         VRONode and make the VRONode a child of the anchor. This ensures ARCore stability.
         */
        ViewParent parent = getParent();
        if (parent instanceof VRTARScene) {
            if (mAnchor == null) {
                mAnchorAttempt = new AnchorAttempt(this, new Vector(mPosition));
                mAnchorAttempt.makeAttempt();
            }
        }

        /*
         If this node *was* an anchor but now has a different parent, remove the intermediate
         anchor.
         */
        else if (mAnchor != null) {
            if (parent == null || !(parent instanceof VRTARScene)) {
                if (DEBUG_ANCHORING) {
                    Log.i(TAG, "Anchored node " + this + " is being removed or repurposed, detaching anchor");
                }
                mAnchor.detach();

                // The Node may already be torn down
                if (mNodeJni != null) {
                    mNodeJni.setPosition(new Vector(mPosition));
                }
            }
        }
    }

    @Override
    public boolean shouldAppear() {
        return super.shouldAppear() && mVisible;
    }

    @Override
    protected void handleAppearanceChange() {
        if (mNodeJni != null) {
            if (mNodeJni.getPhysicsBody() != null) {
                mNodeJni.getPhysicsBody().setEnabled(shouldAppear() && mPhysicsEnabled);
            }
            mNodeJni.setVisible(shouldAppear());
        }
        super.handleAppearanceChange();
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
            if (child instanceof VRTNode) {
                ((VRTNode) child).mIsTopMostChangedElement = false;
                ((VRTNode) child).setChildrenNotTopMostChangedElement();
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
        VRTNode superView = null;
        if (getParent() != null && getParent() instanceof VRTFlexView) {
            superView = (VRTNode) getParent();
        } else if(getParent() != null && getParent() instanceof VRTAnimatedComponent) {
            if (getParent().getParent() != null && getParent().getParent() instanceof VRTFlexView) {
                superView = (VRTNode) getParent().getParent();
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
        if (this instanceof VRTImage) {
            VRTImage image = (VRTImage) this;
            image.setWidth(width3d);
            image.setHeight(height3d);
            image.onPropsSet();
        } else if (this instanceof VRTAnimatedImage) {
            VRTAnimatedImage image = (VRTAnimatedImage) this;
            image.setWidth(width3d);
            image.setHeight(height3d);
            image.onPropsSet();
        } else if (this instanceof VRTFlexView) {
            VRTFlexView flexView = (VRTFlexView) this;
            flexView.setWidth(width3d);
            flexView.setHeight(height3d);
        } else if (this instanceof VRTVideoSurface) {
            VRTVideoSurface videoSurface = (VRTVideoSurface)this;
            videoSurface.setWidth(width3d);
            videoSurface.setHeight(height3d);
            videoSurface.onPropsSet();
        } else if (this instanceof VRTQuad) {
            VRTQuad surface = (VRTQuad) this;
            surface.setWidth(width3d);
            surface.setHeight(height3d);
            surface.updateSurface();
        } else if (this instanceof VRTText) {
            VRTText text = (VRTText) this;
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
        if (isTornDown()) {
            return;
        }
        if (position.length < 3){
            throw new IllegalArgumentException("Missing a position value: All three " +
                    "[x,y,z] axis value are needed.");
        }

        mPosition = position;
        Vector vPosition = new Vector(position);

        /*
         If this is an anchored node and it's moved, determine if we need to re-anchor the
         node or if we can just move the node. If we just move the node, we have to take into
         the account the anchor's position (e.g. set the Node position to the new position
         minus the anchor's position in order to get the correct aggregate position).
         */
        if (mAnchor != null) {
            if (DEBUG_ANCHORING) {
                Log.i(TAG, "Repositioning anchor " + this + " to " + vPosition + " from " + mAnchor.getPositionRealtime());
            }
            if (vPosition.distance(mAnchor.getPositionRealtime()) > REANCHOR_DISTANCE) {
                if (DEBUG_ANCHORING) {
                    Log.i(TAG, "   Reposition distance greater than REANCHOR_DISTANCE: re-anchoring Node");
                }
                if (mAnchorAttempt != null) {
                    mAnchorAttempt.cancel();
                }
                mAnchorAttempt = new AnchorAttempt(this, vPosition);
                mAnchorAttempt.makeAttempt();
            } else {
                if (DEBUG_ANCHORING) {
                    Log.i(TAG, "   Reposition distance less than REANCHOR_DISTANCE: moving node relative to anchor");
                }
                Vector nodePosition = vPosition.subtract(mAnchor.getPositionRealtime());
                mNodeJni.setPosition(nodePosition);
            }
        } else {
            mNodeJni.setPosition(vPosition);
        }
    }

    protected void setRotation(float[] rotation) {
        if (isTornDown()) {
            return;
        }
        if (rotation.length != 3){
            throw new IllegalArgumentException("Missing a rotation value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mRotation = rotation;
        mNodeJni.setRotation(Helper.toRadiansVector(rotation));
    }

    protected void setScale(float[] scale) {
        if (isTornDown()) {
            return;
        }
        if (scale.length != 3){
            throw new IllegalArgumentException("Missing a scale value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mScale = scale;
        mNodeJni.setScale(new Vector(scale));
    }

    protected void setRotationPivot(float[] pivot) {
        if (isTornDown()) {
            return;
        }
        if (pivot.length != 3){
            throw new IllegalArgumentException("Missing a pivot value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mRotationPivot = pivot;
        mNodeJni.setRotationPivot(new Vector(pivot));
    }

    protected void setScalePivot(float[] pivot) {
        if (isTornDown()) {
            return;
        }
        if (pivot.length != 3){
            throw new IllegalArgumentException("Missing a pivot value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mScalePivot = pivot;
        mNodeJni.setScalePivot(new Vector(pivot));
    }

    protected void setOpacity(float opacity) {
        if (isTornDown()) {
            return;
        }
        mOpacity = opacity;
        mNodeJni.setOpacity(opacity);
    }

    protected void setVisible(boolean visible) {
        if (isTornDown()) {
            return;
        }
        mVisible = visible;
        handleAppearanceChange();
    }

    protected void setRenderingOrder(int renderingOrder) {
        if (isTornDown()) {
            return;
        }
        mRenderingOrder = renderingOrder;
        mNodeJni.setRenderingOrder(renderingOrder);
    }

    public void setLightReceivingBitMask(int bitMask) {
        if (isTornDown()) {
            return;
        }
        mLightReceivingBitMask = bitMask;
        mNodeJni.setLightReceivingBitMask(bitMask);
    }

    public void setShadowCastingBitMask(int bitMask) {
        if (isTornDown()) {
            return;
        }
        mShadowCastingBitMask = bitMask;
        mNodeJni.setShadowCastingBitMask(bitMask);
    }

    public void setHighAccuracyEvents(boolean highAccuracyEventsEnabled){
        if (isTornDown()) {
            return;
        }
        mHighAccuracyEventsEnabled = highAccuracyEventsEnabled;
        mNodeJni.setHighAccuracyEvents(highAccuracyEventsEnabled);
    }

    protected void setGeometry(Geometry geometry) {
        if (isTornDown()) {
            return;
        }
        mNodeJni.setGeometry(geometry);
        if (mMaterials != null) {
            setMaterials(mMaterials);
            geometry.copyAndSetMaterials(mMaterials);
        }
    }

    public void setAnimation(ReadableMap animation) {
        mNodeAnimation.parseFromMap(animation);

        if (animation != null && animation.hasKey("name")) {
            mNodeAnimation.setAnimationName(animation.getString("name"));
        } else {
            mNodeAnimation.setAnimationName(null);
        }

        mNodeAnimation.updateAnimation();
    }

    public void setIgnoreEventHandling(boolean ignore){
        mIgnoreEventHandling = ignore;
        mNodeJni.setIgnoreEventHandling(ignore);
    }

    /**
     * Explicitly call setMaterials again with mMaterials. Allows children who override
     * setMaterials to modify them.
     */
    protected void applyMaterials() {
        setMaterials(mMaterials);
    }

    protected void setMaterials(List<Material> materials) {
        if (isTornDown()) {
            return;
        }
        mMaterials = materials;
        if (mNodeJni.getGeometry() != null) {
            mNodeJni.getGeometry().copyAndSetMaterials(materials);
        }
    }

    public void setViroContext(ViroContext context) {
        super.setViroContext(context);
        // CCheck if this material has video materials. Reset the materials if we do.
        if (mMaterials != null) {
            boolean materialsChanged = false;
            ArrayList<Material> nativeMaterials = new ArrayList<>();
            MaterialManager materialManager = getReactContext().getNativeModule(MaterialManager.class);
            for (int i = 0; i < mMaterials.size(); i++) {
                String materialName = mMaterials.get(i).getName();
                Material nativeMaterial = materialManager.getMaterial(materialName);
                if (materialManager.isVideoMaterial(materialName)) {
                    if (!(nativeMaterial.getDiffuseTexture() instanceof VideoTexture)) {
                        // Recreate the material with the proper context.
                            MaterialManager.MaterialWrapper materialWrapper = materialManager.getMaterialWrapper(materialName);
                            VideoTexture videoTexture = new VideoTexture(context, materialWrapper.getVideoTextureURI());
                            materialWrapper.recreate(videoTexture);
                            nativeMaterial = materialWrapper.getNativeMaterial();
                            materialsChanged = true;
                    }
                }

                if (nativeMaterial == null) {
                    throw new IllegalArgumentException("Material [" + materialName + "] not found. Did you create it?");
                }

                nativeMaterials.add(nativeMaterial);
            }

            if (materialsChanged) {
                setMaterials(nativeMaterials);
            }
        }

        if (hasPhysicsBody) {
            if (mPhysicsDelegate != null) {
                mNodeJni.getPhysicsBody().setCollisionListener(mPhysicsDelegate);
            } else {
                mNodeJni.getPhysicsBody().setCollisionListener(null);
            }
        }
    }

    protected void setTransformBehaviors(String[] transformBehaviors) {
        if (isTornDown()) {
            return;
        }

        EnumSet<Node.TransformBehavior> behaviors = EnumSet.noneOf(Node.TransformBehavior.class);
        for (int i = 0; i < transformBehaviors.length; i++) {
            Node.TransformBehavior behavior = null;
            if (transformBehaviors[i].equalsIgnoreCase("billboard")) {
                behavior = Node.TransformBehavior.BILLBOARD;
            }
            else if (transformBehaviors[i].equalsIgnoreCase("billboardX")) {
                behavior = Node.TransformBehavior.BILLBOARD_X;
            }
            else if (transformBehaviors[i].equalsIgnoreCase("billboardY")) {
                behavior = Node.TransformBehavior.BILLBOARD_Y;
            }
            else {
                throw new IllegalArgumentException("Received unknown transform behavior [" + transformBehaviors[i] + "]");
            }
            behaviors.add(behavior);
        }
        mNodeJni.setTransformBehaviors(behaviors);
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
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_HOVER, canHover);
    }

    protected void setCanClick(boolean canClick){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_CLICK, canClick);
    }

    protected void setCanTouch(boolean canTouch){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_TOUCH, canTouch);
    }

    protected void setCanScroll(boolean canScroll){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_SCROLL, canScroll);
    }

    protected void setCanSwipe(boolean canSwipe){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_SWIPE, canSwipe);
    }

    protected void setCanDrag(boolean canDrag){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_DRAG, canDrag);
    }

    protected void setCanFuse(boolean canFuse){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_FUSE, canFuse);
    }

    protected void setCanPinch(boolean canPinch){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_PINCH, canPinch);
    }

    protected void setCanRotate(boolean canRotate){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_ROTATE, canRotate);
    }

    protected void setCanCameraHitTest(boolean canCameraHitTest){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_CAMERA_AR_HIT_TEST, canCameraHitTest);
    }

    protected void setTimeToFuse(float durationInMillis){
        mEventDelegateJni.setTimeToFuse(durationInMillis);
    }

    public void setDragType(String dragType) {
        if (isTornDown()) {
            return;
        }

        mNodeJni.setDragType(Node.DragType.valueFromString(dragType));
    }

    public void setDragPlane(ReadableMap map) {
        if (isTornDown()) {
            return;
        }

        if (map.hasKey("planePoint") && map.hasKey("planeNormal")) {
            ReadableArray planePointArr = map.getArray("planePoint");
            ReadableArray planeNormalArr = map.getArray("planeNormal");
            mNodeJni.setDragPlanePoint(new Vector(
                    planePointArr.getDouble(0), planePointArr.getDouble(1), planePointArr.getDouble(2)));
            mNodeJni.setDragPlaneNormal(new Vector(
                    planeNormalArr.getDouble(0), planeNormalArr.getDouble(1), planeNormalArr.getDouble(2)));
        }

        if (map.hasKey("maxDistance")) {
            mNodeJni.setDragMaxDistance((float) map.getDouble("maxDistance"));
        }

    }

    public void setPhysicsBody(ReadableMap map) {
        // If un-setting the physicsBody, clear it from the node.
        if (map == null) {
            clearPhysicsBody();
            mPhysicsMap = map;
            return;
        }

        // Else update the current physicsBody with the new properties, recreating
        // the body if needed.
        recreatePhysicsBodyIfNeeded(map);
        updatePhysicsBodyProperties(map);
        applyForcesOnBody(map);

        // Finally save a copy of the last known set physics properties.
        mPhysicsMap = map;
    }

    public void setCanCollide(boolean canCollide) {
        if (canCollide && mPhysicsDelegate == null) {
            mPhysicsDelegate = new PhysicsBodyDelegate(this);
        } else if (!canCollide) {
            mPhysicsDelegate = null;
        }

        if (!hasPhysicsBody){
            return;
        }

        if (mPhysicsDelegate != null){
            mNodeJni.getPhysicsBody().setCollisionListener(mPhysicsDelegate);
        } else {
            mNodeJni.getPhysicsBody().setCollisionListener(null);
        }
    }

    public void setViroTag(String tag){
        mNodeJni.setTag(tag);
    }

    private void recreatePhysicsBodyIfNeeded(ReadableMap map){
        float mass = 0;
        if (map.hasKey("mass")){
            mass = (float) map.getDouble("mass");
        }

        // Determine if the physics body type has changed
        String bodyTypeProp = map.getString("type");
        String bodyTypeCurrent = (mPhysicsMap != null && mPhysicsMap.hasKey("type")) ?
                mPhysicsMap.getString("type") : null;
        boolean hasBodyTypeChanged = bodyTypeProp != bodyTypeCurrent;
        if (bodyTypeProp != null){
            hasBodyTypeChanged = !bodyTypeProp.equals(bodyTypeCurrent);
        }

        String bodyTypeError = PhysicsBody.checkIsValidBodyType(bodyTypeProp, mass);
        if (bodyTypeError != null){
            throw new JSApplicationCausedNativeException(bodyTypeError);
        }

        PhysicsBody.RigidBodyType rigidBodyType;
        if (bodyTypeProp.equalsIgnoreCase("Dynamic")) {
            rigidBodyType = PhysicsBody.RigidBodyType.DYNAMIC;
        }
        else if (bodyTypeProp.equalsIgnoreCase("Static")) {
            rigidBodyType = PhysicsBody.RigidBodyType.STATIC;
        }
        else if (bodyTypeProp.equalsIgnoreCase("Kinematic")) {
            rigidBodyType = PhysicsBody.RigidBodyType.KINEMATIC;
        }
        else {
            throw new JSApplicationCausedNativeException("Invalid physics body type [" + bodyTypeProp + "]");
        }

        // Determine if the physics shape has changed
        ReadableMap shapeTypeProp = map.hasKey("shape") ? map.getMap("shape") : null;
        ReadableMap shapeTypeCurrent = (mPhysicsMap != null && mPhysicsMap.hasKey("shape")) ?
                mPhysicsMap.getMap("shape") : null;
        boolean hasShapeChanged = shapeTypeProp != shapeTypeCurrent;
        if (shapeTypeProp != null){
            hasShapeChanged = !shapeTypeProp.equals(shapeTypeCurrent);
        }

        // Create or update the VROPhysicsBody only if needed
        if (!hasPhysicsBody || hasBodyTypeChanged || hasShapeChanged){
            String propShapeType = null;
            float params[] = {};
            PhysicsShape shape = null;

            // Recreate a physics shape with the latest properties by grabbing
            // the current shapeType (required in JS if providing a physics shape)
            if (shapeTypeProp != null) {
                propShapeType = shapeTypeProp.getString("type");
                if (shapeTypeProp.hasKey("params")) {
                    ReadableArray readableParams = shapeTypeProp.getArray("params");
                    params = new float[readableParams.size()];
                    for (int i = 0; i < readableParams.size(); i++) {
                        params[i] = (float) readableParams.getDouble(i);
                    }
                }

                String error = PhysicsBody.checkIsValidShapeType(propShapeType, params);
                if (error != null){
                    throw new JSApplicationCausedNativeException(error);
                }

                if (propShapeType.equalsIgnoreCase("sphere")) {
                    shape = new PhysicsShapeSphere(params[0]);
                }
                else if (propShapeType.equalsIgnoreCase("box")) {
                    shape = new PhysicsShapeBox(params[0], params[1], params[2]);
                }
                else if (propShapeType.equalsIgnoreCase("compound")) {
                    shape = new PhysicsShapeAutoCompound();
                }
                else {
                    throw new JSApplicationCausedNativeException("Invalid shape type [" + propShapeType + "]");
                }
            }

            if (!hasPhysicsBody || hasBodyTypeChanged){
                clearPhysicsBody();
                createPhysicsBody(rigidBodyType, mass, shape);
            } else {
                mNodeJni.getPhysicsBody().setShape(shape);
            }
        }
    }

    private void updatePhysicsBodyProperties(ReadableMap map){
        if (map.hasKey("mass")) {
            float mass = (float)map.getDouble("mass");
            String bodyType = map.getString("type");
            String bodyTypeError = PhysicsBody.checkIsValidBodyType(bodyType, mass);
            if (bodyTypeError != null){
                throw new JSApplicationCausedNativeException(bodyTypeError);
            }
            mNodeJni.getPhysicsBody().setMass(mass);
        }

        if (map.hasKey("inertia")){
            ReadableArray paramsArray = map.getArray("inertia");
            float inertiaArray[] = new float[paramsArray.size()];
            for (int i = 0; i < paramsArray.size(); i ++){
                inertiaArray[i] = (float) paramsArray.getDouble(i);
            }

            if (inertiaArray.length != 3){
                throw new JSApplicationCausedNativeException("Incorrect parameters " +
                        "provided for inertia, expected: [x, y, z]!");
            }

            mNodeJni.getPhysicsBody().setMomentOfInertia(new Vector(inertiaArray));
        }

        if (map.hasKey("friction")) {
            mNodeJni.getPhysicsBody().setFriction((float)map.getDouble("friction"));
        }

        if (map.hasKey("restitution")) {
            mNodeJni.getPhysicsBody().setRestitution((float)map.getDouble("restitution"));
        }

        if (map.hasKey("enabled")) {
            mPhysicsEnabled = map.getBoolean("enabled");
        } else {
            mPhysicsEnabled = true;
        }
        mNodeJni.getPhysicsBody().setEnabled(shouldAppear() && mPhysicsEnabled);

        if (map.hasKey("useGravity")) {
            String bodyType = map.getString("type");
            if (!bodyType.equalsIgnoreCase("dynamic")){
                ViroLog.warn(TAG,"Attempted to set useGravity for non-dynamic phsyics bodies.");
            } else {
                mNodeJni.getPhysicsBody().setUseGravity(map.getBoolean("useGravity"));
            }
        }

        if (map.hasKey("velocity")) {
            ReadableArray paramsArray = map.getArray("velocity");
            float velocityArray[] = new float[paramsArray.size()];
            for (int i = 0; i < paramsArray.size(); i ++){
                velocityArray[i] = (float) paramsArray.getDouble(i);
            }

            if (velocityArray.length != 3){
                throw new JSApplicationCausedNativeException("Incorrect parameters " +
                        "provided for velocity, expected: [x, y, z]!");
            }

            mNodeJni.getPhysicsBody().setVelocity(new Vector(velocityArray), true);
        } else {
            mNodeJni.getPhysicsBody().setVelocity(new Vector(0, 0, 0), true);
        }
    }

    private void applyForcesOnBody(ReadableMap map){
        // Determine if the applied torque has changed
        ReadableArray torqueProp = map.hasKey("torque") ? map.getArray("torque") : null;
        boolean currentMapHasTorque = mPhysicsMap != null && mPhysicsMap.hasKey("torque");
        ReadableArray currentTorque = currentMapHasTorque ? mPhysicsMap.getArray("torque") : null;
        boolean hasTorqueChanged = torqueProp != currentTorque;
        if (torqueProp != null){
            hasTorqueChanged = !torqueProp.equals(currentTorque);
        }

        // Determine if the applied force has changed
        ArrayList<ReadableMap> forcesListProp = getForcesFromReactMap(map);
        ArrayList<ReadableMap> forcesListCurrent = getForcesFromReactMap(mPhysicsMap);
        boolean hasForceChanged = !forcesListProp.equals(forcesListCurrent);

        // If nothing has changed, return
        if (!hasForceChanged && !hasTorqueChanged){
            return;
        }

        mNodeJni.getPhysicsBody().clearForce();

        // Apply Torque
        if (map.hasKey("torque")){
            ReadableArray paramsArray = map.getArray("torque");
            float torqueArray[] = new float[paramsArray.size()];
            for (int i = 0; i < paramsArray.size(); i ++){
                torqueArray[i] = (float) paramsArray.getDouble(i);
            }

            if (torqueArray.length != 3){
                throw new JSApplicationCausedNativeException("Incorrect parameters " +
                        "provided for torque, expected: [x, y, z]!");
            }

            mNodeJni.getPhysicsBody().applyTorque(new Vector(torqueArray));
        }

        // Apply force
        for (ReadableMap forceMap : forcesListProp){
            float forceArray[];
            if (forceMap.hasKey("value")){
                ReadableArray paramsArray = forceMap.getArray("value");
                forceArray = new float[paramsArray.size()];
                for (int i = 0; i < paramsArray.size(); i ++){
                    forceArray[i] = (float) paramsArray.getDouble(i);
                }

                if (forceArray.length != 3){
                    throw new JSApplicationCausedNativeException("Incorrect parameters " +
                            "provided for force's value, expected: [x, y, z]!");
                }
            } else {
                throw new JSApplicationCausedNativeException("Incorrect parameters: missing" +
                        " value of format [x, y, z] for force!");
            }

            float positionArray[];
            if (forceMap.hasKey("position")){
                ReadableArray paramsArray = forceMap.getArray("position");
                positionArray = new float[paramsArray.size()];
                for (int i = 0; i < paramsArray.size(); i ++){
                    positionArray[i] = (float) paramsArray.getDouble(i);
                }

                if (positionArray.length != 3){
                    throw new JSApplicationCausedNativeException("Incorrect parameters " +
                            "provided for force's position, expected: [x, y, z]!");
                }
            } else {
                // Fallback to a central force if no position is provided
                positionArray = new float[]{0,0,0};
            }

            mNodeJni.getPhysicsBody().applyForce(new Vector(forceArray), new Vector(positionArray));
        }
    }

    // Parse, check and return a list of valid force props from a given Readable map.
    private ArrayList<ReadableMap> getForcesFromReactMap(ReadableMap map){
        ArrayList<ReadableMap> maps = new ArrayList<ReadableMap>();
        if (map != null && map.hasKey("force")){
            ReadableType type = map.getType("force");
            if (type.equals(ReadableType.Array)){
                ReadableArray reactForceArray = map.getArray("force");
                for (int i =0; i < reactForceArray.size(); i ++){
                    maps.add(reactForceArray.getMap(i));
                }
            } else if (type.equals(ReadableType.Map)){
                maps.add(map.getMap("force"));
            } else if (!type.equals(ReadableType.Null)) {
                throw new IllegalViewOperationException("Invalid force format!");
            }
        }
        return maps;
    }

    private void createPhysicsBody(PhysicsBody.RigidBodyType bodyType, float mass, PhysicsShape shape){
        mNodeJni.initPhysicsBody(bodyType, mass, shape);
        if (mViroContext != null) {
            if (mPhysicsDelegate != null) {
                mNodeJni.getPhysicsBody().setCollisionListener(mPhysicsDelegate);
            } else {
                mNodeJni.getPhysicsBody().setCollisionListener(null);
            }
        }
        hasPhysicsBody = true;
    }

    private void clearPhysicsBody(){
        if (mNodeJni.getPhysicsBody() != null && mPhysicsDelegate != null) {
            mNodeJni.getPhysicsBody().setCollisionListener(null);
        }

        mNodeJni.clearPhysicsBody();
        hasPhysicsBody = false;
    }

    @Override
    public void setScene(VRTScene scene) {
        super.setScene(scene);
    }

    public void applyImpulse(float[] force, float[] position){
        if (!hasPhysicsBody){
            ViroLog.error(TAG, "Attempted to set an impulse force on a non-physics node");
            return;
        }
        mNodeJni.getPhysicsBody().applyImpulse(new Vector(force), new Vector(position));
    }

    public void applyTorqueImpulse(float[] torque){
        if (!hasPhysicsBody){
            ViroLog.error(TAG, "Attempted to set an impulse force on a non-physics node");
            return;
        }
        mNodeJni.getPhysicsBody().applyTorqueImpulse(new Vector(torque));
    }

    public void setVelocity(float[] velocity, boolean isConstant){
        if (!hasPhysicsBody){
            ViroLog.error(TAG, "Attempted to set a velocity on a non-physics node");
            return;
        }

        mNodeJni.getPhysicsBody().setVelocity(new Vector(velocity), isConstant);
    }

    protected class PhysicsBodyDelegate implements PhysicsBody.CollisionListener {
        private WeakReference<VRTComponent> weakComponent;
        public PhysicsBodyDelegate(VRTComponent component){
            weakComponent = new WeakReference<VRTComponent>(component);
        }

        @Override
        public void onCollided(String collidedTag, Vector collidedPoint, Vector collidedNormal) {
            VRTComponent node = weakComponent.get();
            if (node == null){
                return;
            }

            WritableArray points = Arguments.createArray();
            points.pushDouble(collidedPoint.x);
            points.pushDouble(collidedPoint.y);
            points.pushDouble(collidedPoint.z);

            WritableArray normals = Arguments.createArray();
            normals.pushDouble(collidedNormal.x);
            normals.pushDouble(collidedNormal.y);
            normals.pushDouble(collidedNormal.z);

            WritableMap event = Arguments.createMap();
            event.putString("viroTag", collidedTag);
            event.putArray("collidedPoint", points);
            event.putArray("collidedNormal", normals);

            node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                    node.getId(),
                    ViroEvents.ON_COLLIDED,
                    event);
        }
    }

    public void setOnNativeTransformDelegate(boolean hasDelegate){
        if (hasDelegate){
            mTransformDelegate = new NodeTransformDelegate(this);
            mNodeJni.setTransformListener(mTransformDelegate, TRANSFORM_DELEGATE_DISTANCE_FILTER);
        } else {
            mTransformDelegate = null;
            mNodeJni.removeTransformListener();
        }
    }

    protected class NodeTransformDelegate implements Node.TransformListener {
        private WeakReference<VRTComponent> weakComponent;
        public NodeTransformDelegate(VRTComponent component){
            weakComponent = new WeakReference<VRTComponent>(component);
        }

        @Override
        public void onPositionUpdate(Vector pos) {
            VRTComponent node = weakComponent.get();
            if (node == null){
                return;
            }

            WritableArray position = Arguments.createArray();
            position.pushDouble(pos.x);
            position.pushDouble(pos.y);
            position.pushDouble(pos.z);

            WritableMap event = Arguments.createMap();
            event.putArray("position", position);

            node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                    node.getId(),
                    ViroEvents.ON_TRANSFORM_DELEGATE,
                    event);
        }
    }
}
