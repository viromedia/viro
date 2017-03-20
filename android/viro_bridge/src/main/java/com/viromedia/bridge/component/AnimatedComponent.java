/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.os.Handler;
import android.os.Looper;
import android.view.View;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.BaseAnimation;
import com.viromedia.bridge.component.node.Node;
import com.viromedia.bridge.module.AnimationManager;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.ViroLog;

import java.lang.ref.WeakReference;

public class AnimatedComponent extends Component {
    private static final String TAG = ViroLog.getTag(AnimatedComponent.class);

    private enum AnimationState {
        SCHEDULED, RUNNING, PAUSED, TERMINATED
    }

    private String mAnimation = null;
    private BaseAnimation mExecutableAnimation = null;
    private float mDelayInMilliseconds = 0; // milliseconds
    private boolean mLoop = false;
    private boolean mRun = true;
    private AnimationManager mManager = null;
    private AnimationState mState = AnimationState.TERMINATED;
    private Node mChildNode = null;

    /**
     * This is a handler to the main thread where we queue up our delayed runner
     */
    private Handler mMainLoopHandler = new Handler(Looper.getMainLooper());

    /**
     * This runnable is what we use to start animations w/ a delay.
     */
    private Runnable mDelayedRunner = new Runnable() {
        @Override
        public void run() {
            startAnimation();
        }
    };

    public AnimatedComponent(ReactApplicationContext context) {
        super(context);
        mManager = context.getNativeModule(AnimationManager.class);
    }

    public void setAnimation(String animationName) {
        // Terminate the old animation if there was one and reset the state
        if (mExecutableAnimation != null) {
            mExecutableAnimation.terminate();
        }
        mState = AnimationState.TERMINATED;
        mAnimation = animationName;
    }

    public void setLoop(boolean loop) {
        mLoop = loop;
    }

    public void setDelay(float delay) {
        mDelayInMilliseconds = delay;
    }

    public void setRun(boolean run) {
        mRun = run;
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
    public void removeViewAt(int index) {
        ((Node) getParent()).removeNativeChild((Node) getChildAt(index));
        mChildNode = null;

        super.removeViewAt(index);
    }

    @Override
    protected void onPropsSet() {
        executeAnimation();
    }

    /**
     * This method is provided so the manager can let this component know when its children are all
     * set. This is because props are set before children, so if the children change, then we need
     * to start/restart animations.
     */
    protected void onChildrenSet() {
        terminateAnimation();
        executeAnimation();
    }

    /**
     * This method terminates any scheduled, running or paused animation and sets the state to
     * terminated.
     */
    private void terminateAnimation() {
        if (mState == AnimationState.SCHEDULED) {
            mMainLoopHandler.removeCallbacks(mDelayedRunner);
        } else if (mState == AnimationState.RUNNING || mState == AnimationState.PAUSED) {
            if (mExecutableAnimation != null) {
                mExecutableAnimation.terminate();
            }
        }
        mState = AnimationState.TERMINATED;
    }

    /**
     * This method executes the animation.
     */
    private void executeAnimation() {
        if (getChildCount() != 1) {
            return;
        }

        if (mRun) {
            playAnimation();
        } else {
            pauseAnimation();
        }
    }

    /**
     * This method sets us up to play the animation by scheduling a new animation or starting
     * a paused animation.
     */
    private void playAnimation() {
        if (mState == AnimationState.TERMINATED) {
            mState = AnimationState.SCHEDULED;
            mMainLoopHandler.postDelayed(mDelayedRunner, (long) mDelayInMilliseconds);
        } else if(mState == AnimationState.PAUSED) {
            mExecutableAnimation.resume();
            onStartAnimation();
        } else {
            ViroLog.warn(TAG, "Unable to play animation in state " + mState.name());
        }
    }

    /**
     * This method pauses a running animation or terminates a scheduled one.
     */
    private void pauseAnimation() {
        if (mState == AnimationState.RUNNING) {
            mExecutableAnimation.pause();
            mState = AnimationState.PAUSED;
        } else if (mState == AnimationState.SCHEDULED) {
            mState = AnimationState.TERMINATED;
            mMainLoopHandler.removeCallbacks(mDelayedRunner);
        }
    }

    /**
     * This method starts a scheduled animation
     */
    private void startAnimation() {
        if (mState != AnimationState.SCHEDULED) {
            ViroLog.info(TAG, "Aborted starting new animation, was no longer scheduled");
            return;
        }
        if (mChildNode.isTornDown()) {
            ViroLog.info(TAG, "Aborted starting new animation, child is torn down");
            return;
        }

        mExecutableAnimation = mManager.getAnimation(mAnimation).copy();
        if (mExecutableAnimation == null) {
            throw new IllegalArgumentException("Animation [" + mAnimation + "] does not exist." +
                    " Have you registered it with ViroAnimations.registerAnimations()?");
        }

        onStartAnimation();

        final WeakReference<AnimatedComponent> weakSelf = new WeakReference<>(this);
        mExecutableAnimation.execute(mChildNode.getNodeJni(), new BaseAnimation.AnimationDelegate() {
            @Override
            public void onFinish() {
                AnimatedComponent self = weakSelf.get();
                if (weakSelf != null) {
                    self.onFinishAnimation();
                }
            }
        });
    }

    /**
     * This method should be called when an animation starts and notifies the JS bridge
     */
    private void onStartAnimation() {
        mState = AnimationState.RUNNING;
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_START,
                null);
    }

    /**
     * This method should be called by the JNI layer when the animation ends and notifies the JS
     * bridge. It also handles looping logic.
     */
    private void onFinishAnimation() {
        if (isTornDown()) {
            return;
        }
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_FINISH,
                null);
        mState = AnimationState.TERMINATED;
        if (mLoop && mRun) {
            playAnimation();
        }
    }
}
