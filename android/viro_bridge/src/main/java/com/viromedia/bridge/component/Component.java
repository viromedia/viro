/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import com.facebook.react.views.view.ReactViewGroup;
import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.RenderContextJni;
import com.viromedia.bridge.component.node.Scene;

/**
 * Base class for any Viro UI Component. Equivalent to the VRTView in iOS.
 */
public class Component extends ReactViewGroup {
    private static String TAG = Component.class.getSimpleName();
    protected RenderContextJni mRenderContext = null;
    protected ReactApplicationContext mReactContext = null;
    protected Scene mScene = null;

    private static Boolean IS_TORN_DOWN = false;

    public Component(ReactApplicationContext reactContext) {
        this(reactContext.getBaseContext(), null, -1, -1, reactContext);
    }

    public Component(Context context) {
        this(context, null, -1, -1, null);
    }

    public Component(Context context, AttributeSet attrs, int defStyleAttr,
                     int defStyleRes, ReactApplicationContext reactContext) {
        super(context);
        mReactContext = reactContext;
    }

    public ReactApplicationContext getReactContext(){
        return mReactContext;
    }

    public void componentWillAppear() {
        // no-op
    }

    public void componentWillDisappear() {
        // no-op
    }

    public boolean hoverable() {
        return false;
    }

    public boolean tappable() {
        return false;
    }

    public void setRenderContext(RenderContextJni context){
        mRenderContext = context;

        // Update our child views with the scene as well.
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof Component){
                ((Component)child).setRenderContext(context);
            }
        }
    }

    public void setScene(Scene scene){
        mScene = scene;

        // Update our child views with the context as well.
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof Component){
                ((Component)child).setScene(scene);
            }
        }
    }

    /**
     * If in the case a React view is added / destroyed, update
     * the renderContext of child views if we can (if we have it).
     */
    @Override
    public void addView(View child, int index) {
        super.addView(child, index);

        if (!(child instanceof Component)){
            throw new IllegalArgumentException("Attempted to add a non-Component child of type: [" +
                    child.getClass().getSimpleName() + "].");
        }

        if (mRenderContext != null &&
                ((Component) child).mRenderContext == null){
            ((Component) child).setRenderContext(mRenderContext);
        }

        if (mScene != null &&
                ((Component) child).mScene == null){
            ((Component) child).setScene(mScene);
        }
    }

    /**
     * Invoked when it is safe to cleanup the view, and
     * native references (renderer references) can be safely deleted.
     * This is only invoked after the view is detached from the view
     * hierarchy by the NativeViewHierarchyManager, *and* the view has
     * been removed from the tree via removeViewAt from its parent.
     */
    public void onTearDown() {
        IS_TORN_DOWN = true;
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
        for(int i = 0; i < getChildCount(); i++) {
            if (getChildAt(i) instanceof Component) {
                ((Component) getChildAt(i)).recalcLayout();
            }
        }
    }

    /**
     * Helper method to indicate if teardown has already happened for this component.
     *
     * @return whether or not this component has already been torn down.
     */
    protected boolean isTornDown() {
        return IS_TORN_DOWN;
    }

    public void sceneWillAppear() {
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof Component){
                ((Component)child).sceneWillAppear();
            }
        }
    }

    public void sceneDidAppear() {
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof Component){
                ((Component)child).sceneDidAppear();
            }
        }
    }

    public void sceneWillDisappear() {
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof Component){
                ((Component)child).sceneWillDisappear();
            }
        }
    }

    public void sceneDidDisappear() {
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof Component){
                ((Component)child).sceneDidDisappear();
            }
        }
    }
}
