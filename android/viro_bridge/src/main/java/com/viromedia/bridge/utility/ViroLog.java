/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.utility;


import android.util.Log;

public class ViroLog {
    private static final String TAG_PREFIX = "Viro-";

    public static String getTag(Class clazz) {
        return TAG_PREFIX + clazz.getSimpleName();
    }

    public static void debug(String tag, String message) {
        Log.d(tag, message);
    }

    public static void info(String tag, String message) {
        Log.i(tag, message);
    }

    public static void warn(String tag, String message) {
        Log.w(tag, message);
    }

    public static void error(String tag, String message) {
        Log.e(tag, message);
    }
}
