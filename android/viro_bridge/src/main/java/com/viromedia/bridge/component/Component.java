/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.FrameLayout;

/**
 * Base class for any Viro UI Component. Equivalent to the VRTView in iOS.
 */
public class Component extends FrameLayout {

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
}
