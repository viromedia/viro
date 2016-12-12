/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import android.content.Context;

import com.facebook.react.uimanager.annotations.ReactProp;

public class Camera extends Component {
    private float[] mPosition;

    public Camera(Context context) {
        super(context);
    }

    public void setPosition(float[] position) {
        mPosition = position;
    }

    public float[] getPosition() {
        return mPosition;
    }

    public String getRotationType() {
        return "Standard";
    }
}
