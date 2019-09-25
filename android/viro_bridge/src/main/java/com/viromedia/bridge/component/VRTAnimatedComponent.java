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

package com.viromedia.bridge.component;

import android.view.View;

import com.facebook.react.bridge.ReactContext;
import com.viro.core.internal.ExecutableAnimation;
import com.viromedia.bridge.component.node.VRTNode;
import com.viromedia.bridge.module.AnimationManager;
import com.viromedia.bridge.utility.ViroLog;

public class VRTAnimatedComponent extends VRTComponent {
    private static final String TAG = ViroLog.getTag(VRTAnimatedComponent.class);

    private static class AnimatedComponentAnimation extends VRTManagedAnimation {

        private String mAnimationName;
        private AnimationManager mAnimationManager;

        public AnimatedComponentAnimation(ReactContext context, VRTComponent parent) {
            super(context, parent);
            mAnimationManager = context.getNativeModule(AnimationManager.class);
        }

        public void setAnimationName(String name) {
            mAnimationName = name;
            super.updateAnimation();
        }

        @Override
        public ExecutableAnimation loadAnimation() {
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
    private VRTNode mChildNode = null;

    public VRTAnimatedComponent(ReactContext context) {
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
        if (!(child instanceof VRTNode)) {
            throw new IllegalArgumentException("Can not add " + child.getClass().getSimpleName()
                    + " as child of ViroAnimatedComponent");
        }
        super.addView(child, index);

        if (getChildCount() > 1) {
            throw new IllegalStateException("ViroAnimatedComponent can only have 1 child, found more than 1.");
        }

        mChildNode = (VRTNode) child;
        mManagedAnimation.setNode((VRTNode) child);
        // Check if our child is already a child of our parent node, if not, then add it
        if (getParent() != null) {
            VRTNode parent = (VRTNode) getParent();
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
        ((VRTNode) getParent()).removeNativeChild((VRTNode) child);
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
