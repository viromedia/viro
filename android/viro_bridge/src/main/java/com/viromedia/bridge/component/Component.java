/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.FrameLayout;
import com.viro.renderer.jni.RenderContextJni;

/**
 * Base class for any Viro UI Component. Equivalent to the VRTView in iOS.
 */
public class Component extends FrameLayout {
    protected RenderContextJni mRenderContext = null;

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

        // Update our child views with the context as well.
        for (int i = getChildCount() - 1; i >= 0; i--) {
            final View child = getChildAt(i);
            if (child instanceof Component){
                ((Component)child).setRenderContext(context);
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

        if (mRenderContext != null &&
                (child instanceof Component) &&
                ((Component) child).mRenderContext == null){
            ((Component) child).setRenderContext(mRenderContext);
        }
    }
}
