//  Copyright © 2017 Viro Media. All rights reserved.
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

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.facebook.react.views.view.ReactViewGroup;
import com.viro.core.ViroContext;
import com.viromedia.bridge.component.node.VRTScene;
import com.viromedia.bridge.utility.ViroEvents;

/**
 * Base class for any Viro UI Component. Equivalent to the VRTView in iOS.
 */
public class VRTComponent extends ReactViewGroup {

    private static String TAG = VRTComponent.class.getSimpleName();
    protected ViroContext mViroContext = null;
    protected ReactContext mReactContext = null;
    protected VRTScene mScene = null;
    /*
     True/False depending on whether or not the parent view has appeared.
     */
    protected boolean mParentHasAppeared = false;

    /*
     These three bools define the state of each component.
     mDropped indicates whether the Component has been dropped by
     the NativeViewHierarchyManager. mDetached indicates if the
     Component is detached from the React view tree (meaning no
     children and no parent). If both of these become true, the
     component is torn down, releasing native resources.
     */
    private boolean mTornDown = false;
    private boolean mDropped = false;
    private boolean mDetached = false;

    public VRTComponent(ReactContext reactContext) {
        this(reactContext.getBaseContext(), null, -1, -1, reactContext);
    }

    public VRTComponent(Context context) {
        this(context, null, -1, -1, null);
    }

    public VRTComponent(Context context, AttributeSet attrs, int defStyleAttr,
                        int defStyleRes, ReactContext reactContext) {
        super(context);
        mReactContext = reactContext;
    }

    public ReactContext getReactContext(){
        return mReactContext;
    }

    public ViroContext getViroContext() { return mViroContext; }

    public void setViroContext(ViroContext context) {
        mViroContext = context;

        // Update our child views with the scene as well.
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof VRTComponent){
                ((VRTComponent)child).setViroContext(context);
            }
        }
    }

    public void setScene(VRTScene scene){
        mScene = scene;

        // Update our child views with the context as well.
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof VRTComponent) {
                ((VRTComponent)child).setScene(scene);
            }
        }
    }

    /**
     * If in the case a React view is added / destroyed, update
     * the ViroContext of child views if we can (if we have it).
     */
    @Override
    public void addView(View child, int index) {
        super.addView(child, index);

        if (!(child instanceof VRTComponent)){
            throw new IllegalArgumentException("Attempted to add a non-Component child of type: [" +
                    child.getClass().getSimpleName() + "].");
        }

        VRTComponent component = (VRTComponent) child;
        if (mViroContext != null && component.mViroContext == null){
            component.setViroContext(mViroContext);
        }
        if (mScene != null && component.mScene == null){
            component.setScene(mScene);
        }

        component.onTreeUpdate();
        onTreeUpdate();

        if (shouldAppear()) {
            component.parentDidAppear();
        }
    }

    @Override
    public void removeViewAt(int index) {
        View child = getChildAt(index);
        super.removeViewAt(index);

        /*
         We tear down views whenever they're removed from the tree
         and dropped, meaning they have no children and no parent.
         */
        if (child instanceof VRTComponent) {
            VRTComponent component = (VRTComponent) child;
            component.parentDidDisappear();
            component.onTreeUpdate();
        }
        onTreeUpdate();
    }

    /**
     * Invoked when a view is attached or detached from the React
     * view tree. We check if its fully detached (no parents or children),
     * to mark its detached status.
     */
    public void onTreeUpdate() {
        mDetached = (getChildCount() == 0 && getParent() == null);
        if (mDetached && mDropped && !mTornDown) {
            onTearDown();
        }
    }

    /**
     * Invoked when a view is dropped by the NativeViewHierarchyManager.
     */
    public final void onDrop() {
        mDropped = true;
        if (mDetached && mDropped && !mTornDown) {
            onTearDown();
        }
    }

    /*
     * Invoked when the SceneNavigator is destroyed after being detached
     * from a window (as happens with reload). Forces a tear-down that
     * cascades down the view tree.
     */
    public void forceCascadeTearDown() {
        mDetached = true;
        mDropped = true;
        if (!mTornDown) {
            onTearDown();
        }

        for (int i = 0; i < getChildCount(); i++) {
            View child = getChildAt(i);
            if (child instanceof VRTComponent) {
                ((VRTComponent) child).forceCascadeTearDown();
            }
        }
    }

    /**
     * Invoked when it is safe to cleanup the view, and
     * native references (renderer references) can be safely deleted.
     * This is only invoked after the view is dropped by the
     * NativeViewHierarchyManager, *and* the view has been removed
     * from the tree via removeViewAt from its parent.
     *
     * Children should override and invoke this super method.
     */
    public void onTearDown() {
        mTornDown = true;
    }

    /**
     * Helper method to indicate if teardown has already happened for this component.
     *
     * @return whether or not this component has already been torn down.
     */
    public boolean isTornDown() {
        return mTornDown;
    }

    /**
     * Callback that will be triggered after all ReactProp are updated.
     */
    protected void onPropsSet() {
        // No-op
    }

    /**
     * This function is used by FlexViews and the controls that go inside them in order for them
     * to properly convert their 2D layout values to 3D-space. This is because React Native seems
     * to call onLayout() from the bottom of the view tree upwards which means when a component gets
     * its layout values, it is unable to get those of its parent to properly set.
     *
     * An invariant of this function is that by the end of this function, the Component's pivotX,
     * pivotY, left, top, right and bottom properties are properly set.
     */
    public void recalcLayout() {
        // Loop over every child of a scene. Ideally only particular nodes should respond.
        for (int i = 0; i < getChildCount(); i++) {
            if (getChildAt(i) instanceof VRTComponent) {
                ((VRTComponent) getChildAt(i)).recalcLayout();
            }
        }
    }

    public void sceneWillAppear() {
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof VRTComponent){
                ((VRTComponent)child).sceneWillAppear();
            }
        }
    }

    public void sceneDidAppear() {
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof VRTComponent){
                ((VRTComponent)child).sceneDidAppear();
            }
        }
    }

    public void sceneWillDisappear() {
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof VRTComponent){
                ((VRTComponent)child).sceneWillDisappear();
            }
        }
    }

    public void sceneDidDisappear() {
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof VRTComponent){
                ((VRTComponent)child).sceneDidDisappear();
            }
        }
    }

    public void onHostResume() {
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof VRTComponent){
                ((VRTComponent)child).onHostResume();
            }
        }
    }

    public void onHostPause() {
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof VRTComponent){
                ((VRTComponent)child).onHostPause();
            }
        }
    }

    /*
     The default behavior of whether or not a Component should appear is whether
     or not the parent component has appeared.
     */
    protected boolean shouldAppear() {
        return mParentHasAppeared;
    }

    /*
     This function is called when the appearance state of this Component has changed.
     */
    protected void handleAppearanceChange() {
        if (this.shouldAppear()) {
            for (int i = 0; i < getChildCount(); i++) {
                ((VRTComponent) getChildAt(i)).parentDidAppear();
            }
        } else {
            for (int i = 0; i < getChildCount(); i++) {
                ((VRTComponent) getChildAt(i)).parentDidDisappear();
            }
        }
    }

    /*
     Called by the parent Component to let child know that the Parent view has appeared.
     */
    protected void parentDidAppear() {
        mParentHasAppeared = true;
        handleAppearanceChange();
    }

    /*
     Called by the parent Component to let child know that the Parent view has disappeared.
     */
    protected void parentDidDisappear() {
        mParentHasAppeared = false;
        handleAppearanceChange();
    }

    /**
     * Invoke this control's onError callback, if it has one, with the given
     * message.
     *
     * @param error The error message.
     */
    public void onError(String error) {
        WritableMap event = Arguments.createMap();
        event.putString("error", error);

        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_ERROR,
                event);
    }
}
