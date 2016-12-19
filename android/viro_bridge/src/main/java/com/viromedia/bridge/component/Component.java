/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.FrameLayout;
import com.viro.renderer.jni.RenderContextJni;
import com.viromedia.bridge.component.node.Scene;

/**
 * Base class for any Viro UI Component. Equivalent to the VRTView in iOS.
 */
public class Component extends FrameLayout {
    private static String TAG = Component.class.getSimpleName();
    protected RenderContextJni mRenderContext = null;
    protected Scene mScene = null;

    private static Boolean IS_TORN_DOWN = false;

    public Component(Context context) {
        this(context, null, -1, -1);
    }

    public Component(Context context, AttributeSet attrs) {
        this(context, attrs, -1, -1);
    }

    public Component(Context context, AttributeSet attrs, int defStyleAttr) {
        this(context, attrs, defStyleAttr, -1);
    }

    public Component(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
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
            throw new IllegalArgumentException("Attempted to add a non-Component child.");
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
     * Called when view is detached from view hierarchy and allows
     * for some additional cleanup.
     */
    protected void onTearDown() {
        IS_TORN_DOWN = true;
    }

    /**
     * Callback that will be triggered after all ReactProp are updated.
     */
    protected void onPropsSet() {
        // No-op
    }

    /**
     * Helper method to indicate if teardown has already happened for this component. Package
     * protected since this should be needed only from com.viromedia.bridge.component. Extend if
     * needed later
     *
     * @return
     */
    boolean isTornDown() {
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
