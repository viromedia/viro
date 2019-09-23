//  Copyright © 2016 Viro Media. All rights reserved.
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

package com.viromedia.bridge.utility;

import android.content.Context;
import android.view.Display;
import android.view.OrientationEventListener;
import android.view.Surface;
import android.view.WindowManager;

import java.lang.ref.WeakReference;

/**
 * Extension of {@link OrientationEventListener} that invokes a callback whenever the Display
 * orientation changes. This effectively connects a standard OrientationEventListener, which
 * listens for sensor data, to the Display's getRotation method.
 */
public abstract class DisplayRotationListener extends OrientationEventListener {

    private int mSavedRotation = Integer.MIN_VALUE;
    private WeakReference<Context> mContext;

    public DisplayRotationListener(Context context) {
        super(context);
        mContext = new WeakReference(context);
    }

    public abstract void onDisplayRotationChanged(int rotation);

    @Override
    public void onOrientationChanged(int orientation) {
        Context context = mContext.get();
        if (context == null) {
            return;
        }

        Display display = ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
        int surfaceRotation = display.getRotation();
        if (surfaceRotation != mSavedRotation) {
            mSavedRotation = surfaceRotation;
            onDisplayRotationChanged(mSavedRotation);
        }
    }
}