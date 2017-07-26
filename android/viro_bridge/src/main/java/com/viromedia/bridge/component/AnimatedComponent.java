/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.os.Handler;
import android.os.Looper;
import android.view.View;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.ExecutableAnimationJni;
import com.viromedia.bridge.component.node.Node;
import com.viromedia.bridge.module.AnimationManager;
import com.viromedia.bridge.utility.ViroLog;

public class AnimatedComponent extends Component {
    private static final String TAG = ViroLog.getTag(AnimatedComponent.class);

    private static class AnimatedComponentAnimation extends ManagedAnimation {

        private String mAnimationName;
        private AnimationManager mAnimationManager;

        public AnimatedComponentAnimation(ReactApplicationContext context, Component parent) {
            super(context, parent);
            mAnimationManager = context.getNativeModule(AnimationManager.class);
        }

        public void setAnimationName(String name) {
            mAnimationName = name;
            super.updateAnimation();
        }

        @Override
        public ExecutableAnimationJni loadAnimation() {
            if (mAnimationName != null) {
                return mAnimationManager.getAnimation(mAnimationName).copy();
            }
            else {
                return null;
            }
        }
    }

    private AnimatedComponentAnimation mManagedAnimation = null;
    private AnimationManager mAnimationManager = null;
    private Node mChildNode = null;

    public AnimatedComponent(ReactApplicationContext context) {
        super(context);
        mManagedAnimation = new AnimatedComponentAnimation(context, this);
        mAnimationManager = context.getNativeModule(AnimationManager.class);
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        mManagedAnimation.onTearDown();
    }

    public void setRun(boolean run) {
        mManagedAnimation.setRun(run);;
    }

    public void setLoop(boolean loop) {
        mManagedAnimation.setLoop(loop);
    }

    public void setDelay(float delay) {
        mManagedAnimation.setDelay(delay);
    }

    public void setAnimation(String animationName) {
        if (mAnimationManager.getAnimation(animationName) == null) {
            throw new IllegalArgumentException("Unable to find animation with name [" + animationName + "]");
        }

        mManagedAnimation.setAnimationName(animationName);
    }

    /**
     * AnimatedComponents have their children's native nodes added to the parent, making this
     * component virtually invisible in terms of the scene graph.
     */
    @Override
    public void addView(View child, int index) {
        if (!(child instanceof Node)) {
            throw new IllegalArgumentException("Can not add " + child.getClass().getSimpleName()
                    + " as child of ViroAnimatedComponent");
        }
        super.addView(child, index);

        if (getChildCount() > 1) {
            throw new IllegalStateException("ViroAnimatedComponent can only have 1 child, found more than 1.");
        }

        mChildNode = (Node) child;
        mManagedAnimation.setNode((Node) child);
        // Check if our child is already a child of our parent node, if not, then add it
        if (getParent() != null) {
            Node parent = (Node) getParent();
            if (!parent.containsChild(mChildNode)) {
                parent.addNativeChild(mChildNode);
            }
        }

        // TODO: for now since AnimatedComponent can only have 1 child, we know that after this
        // method is called once, we have all our childen set. If we accept more than 1 child in
        // the future, we'll need to figure out a better way to figure this out.
        onChildrenSet();
    }

    @Override
    public void removeView(View child) {
        super.removeView(child);
        ((Node) getParent()).removeNativeChild((Node) child);
        mChildNode = null;
    }

    @Override
    protected void onPropsSet() {
        mManagedAnimation.updateAnimation();
    }

    /**
     * This method is provided so the manager can let this component know when its children are all
     * set. This is because props are set before children, so if the children change, then we need
     * to start/restart animations.
     */
    protected void onChildrenSet() {
        mManagedAnimation.updateAnimation();
    }
}
