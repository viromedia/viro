/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.component;

import android.content.Context;

import com.viro.renderer.jni.NodeJni;

public abstract class Light extends Component {

    protected int mColor;

    public Light(Context context) {
        super(context);
    }

    public int getColor() {
        return mColor;
    }

    public void setColor(int mColor) {
        this.mColor = mColor;
    }

    public abstract void addToNode(NodeJni nodeJni);

    public abstract void removeFromNode(NodeJni nodeJni);
}
