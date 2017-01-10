package com.viromedia.bridge.utility;

import android.support.annotation.Nullable;

import com.facebook.react.bridge.ReadableArray;

/**
 * Class containing few common helper methods.
 * Note - Feel free to split / rename this class ass this class gets more methods
 */

public class Helper {

    public static @Nullable float[] toFloatArray(@Nullable ReadableArray value) {
        if (value == null){
            throw new IllegalArgumentException("Can't convert a null ReadableArray to a float[].");
        }

        float[] result = new float[value.size()];
        for (int i = 0; i < value.size(); i++) {
            result[i] = (float) value.getDouble(i);
        }
        return result;
    }
}
