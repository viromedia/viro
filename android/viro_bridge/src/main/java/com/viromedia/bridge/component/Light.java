/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.component;

import android.content.Context;
import android.util.AttributeSet;

public class Light extends Component {
    public Light(Context context) {
        this(context, null, -1, -1);
    }

    public Light(Context context, AttributeSet attrs) {
        this(context, attrs, -1, -1);
    }

    public Light(Context context, AttributeSet attrs, int defStyleAttr) {
        this(context, attrs, defStyleAttr, -1);
    }

    public Light(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }
}
