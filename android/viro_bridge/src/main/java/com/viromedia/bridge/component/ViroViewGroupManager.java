/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.component;

import android.util.Log;
import android.view.ViewGroup;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.LayoutShadowNode;
import com.facebook.react.uimanager.ViewGroupManager;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.Scene;

import javax.annotation.Nullable;

/**
 * This is the base manager class for ALL Viro UI Components. Because we share some common
 * UI Component properties with React Native itself, we'll simply just override them here.
 */
public abstract class ViroViewGroupManager<T extends ViewGroup>
        extends ViewGroupManager<T> {

    private final ReactApplicationContext mContext;

    public ViroViewGroupManager(ReactApplicationContext context) {
        mContext = context;
    }

    public ReactApplicationContext getContext() {
        return mContext;
    }

    @Override
    public LayoutShadowNode createShadowNodeInstance() {
        return new ViroLayoutShadowNode();
    }

    /* Override React Properties */

    @ReactProp(name = "position")
    public void setPosition(T view, ReadableArray position) {
        // no-op
    }

    private class ViroLayoutShadowNode extends LayoutShadowNode {

        @ReactProp(name = "position")
        public void setPosition(@Nullable ReadableArray position) {
            // no-op
        }
    }
}
