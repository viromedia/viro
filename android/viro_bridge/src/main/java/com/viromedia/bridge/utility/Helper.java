package com.viromedia.bridge.utility;

import android.content.Context;
import android.net.Uri;
import android.support.annotation.Nullable;

import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.views.imagehelper.ResourceDrawableIdHelper;

/**
 * Class containing few common helper methods.
 * Note - Feel free to split / rename this class ass this class gets more methods
 */

public class Helper {

    public static @Nullable float[] toFloatArray(@Nullable ReadableArray value) {
        if (value == null){
            return null;
        }

        float[] result = new float[value.size()];
        for (int i = 0; i < value.size(); i++) {
            result[i] = (float) value.getDouble(i);
        }
        return result;
    }

    public static Uri parseUri(String path, Context context) {
        if (path == null) {
            return null;
        }

        Uri tempUri = Uri.parse(path);
        // if the scheme is null, then it's a local resource
        return tempUri.getScheme() == null ? computeLocalUri(path, context) : tempUri;
    }

    private static Uri computeLocalUri(String path, Context context) {
        return ResourceDrawableIdHelper.getInstance().getResourceDrawableUri(context, path);
    }
}
