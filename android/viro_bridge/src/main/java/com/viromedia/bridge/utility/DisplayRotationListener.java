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