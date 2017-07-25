package com.viromedia.bridge.component;

import android.os.Handler;
import android.os.Looper;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viromedia.bridge.component.node.Node;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.ViroLog;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.ExecutableAnimationJni;
import com.viro.renderer.jni.NodeJni;

import java.lang.ref.WeakReference;

/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
public class ManagedAnimation {
    private static final String TAG = ViroLog.getTag(ManagedAnimation.class);

    private enum AnimationState {
        SCHEDULED, RUNNING, PAUSED, TERMINATED
    }

    private ReactApplicationContext mReactContext;
    private ExecutableAnimationJni mExecutableAnimation = null;
    private float mDelayInMilliseconds = 0; // milliseconds
    private boolean mLoop = false;
    private boolean mRun = true;
    private AnimationState mState = AnimationState.TERMINATED;

    /**
     * The node on which the animation is run.
     */
    private Node mNode = null;

    /**
     * The Component on which the onStart and onFinish events are set.
     */
    private Component mParentComponent;

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

    public ManagedAnimation(ReactApplicationContext context, Component parent) {
        mReactContext = context;
        mParentComponent = parent;
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
     * Set the node on which the animation runs.
     */
    public void setNode(Node node) {
        mNode = node;
    }

    public void setAnimation(ExecutableAnimationJni animation) {
        // Terminate the old animation if there was one and reset the state
        if (mExecutableAnimation != null) {
            mExecutableAnimation.terminate();
        }

        mState = AnimationState.TERMINATED;
        mExecutableAnimation = animation;
    }

    public void updateAnimation() {
        if (mRun) {
            playAnimation();
        }
        else {
            pauseAnimation();
        }
    }

    /**
     * This method sets us up to play the animation by scheduling a new animation or starting
     * a paused animation.
     */
    private void playAnimation() {
        if (mState == AnimationState.PAUSED) {
            mExecutableAnimation.resume();
            mState = AnimationState.RUNNING;
        }
        else if (mState == AnimationState.TERMINATED) {
            mState = AnimationState.SCHEDULED;
            // invoke startAnimation() right away if there is no delay provided.
            if(mDelayInMilliseconds <= 0) {
                startAnimation();
            }else {
                mMainLoopHandler.postDelayed(mDelayedRunner, (long) mDelayInMilliseconds);
            }
        }
        else {
            ViroLog.warn(TAG, "Unable to play animation in state " + mState.name());
        }
    }

    /**
     * This method pauses a running animation or terminates a scheduled one.
     */
    private void pauseAnimation() {
        if (mExecutableAnimation == null) {
            return;
        }
        if (mState == AnimationState.RUNNING) {
            mExecutableAnimation.pause();
            mState = AnimationState.PAUSED;
        }
        else if (mState == AnimationState.SCHEDULED) {
            mState = AnimationState.TERMINATED;
            mMainLoopHandler.removeCallbacks(mDelayedRunner);
        }
    }

    /**
     * This method terminates any scheduled, running or paused animation and sets the state to
     * terminated.
     */
    private void terminateAnimation() {
        if (mState == AnimationState.SCHEDULED) {
            mMainLoopHandler.removeCallbacks(mDelayedRunner);
        }
        else if (mState == AnimationState.RUNNING || mState == AnimationState.PAUSED) {
            if (mExecutableAnimation != null) {
                mExecutableAnimation.terminate();
            }
        }
        mState = AnimationState.TERMINATED;
    }

    /**
     * This method starts a scheduled animation
     */
    private void startAnimation() {
        if (mState != AnimationState.SCHEDULED) {
            ViroLog.info(TAG, "Aborted starting new animation, was no longer scheduled");
            return;
        }
        if (mNode == null) {
            ViroLog.info(TAG, "Aborted starting new animation, node is not set");
            return;
        }
        if (mNode.isTornDown()) {
            ViroLog.info(TAG, "Aborted starting new animation, node is torn down");
            return;
        }

        onStartAnimation();

        final WeakReference<ManagedAnimation> weakSelf = new WeakReference<>(this);
        mExecutableAnimation.execute(mNode.getNodeJni(), new ExecutableAnimationJni.AnimationDelegate() {
            @Override
            public void onFinish(ExecutableAnimationJni animation) {
                ManagedAnimation self = weakSelf.get();
                if (self != null) {
                    self.onFinishAnimation(animation);
                }
            }
        });
        mState = AnimationState.RUNNING;
    }

    /**
     * This method should be called when an animation starts and notifies the JS bridge
     */
    private void onStartAnimation() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                mParentComponent.getId(),
                ViroEvents.ON_START,
                null);
    }

    /**
     * This method should be called by the JNI layer when the animation ends and notifies the JS
     * bridge. It also handles looping logic.
     */
    private void onFinishAnimation(ExecutableAnimationJni animation) {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                mParentComponent.getId(),
                ViroEvents.ON_FINISH,
                null);

        // If the animation changed, we won't terminate or loop
        if (mExecutableAnimation == animation) {
            mState = AnimationState.TERMINATED;
            if (mLoop && mRun) {
                playAnimation();
            }
        }
    }
}
