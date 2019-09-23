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
import android.net.Uri;
import androidx.annotation.Nullable;

import com.facebook.react.bridge.ReadableArray;
import com.viro.core.Vector;

/**
 * Class containing few common helper methods.
 * Note - Feel free to split / rename this class as this class gets more methods
 */

public class Helper {

    private static final String RESOURCE_SCHEME = "res";

    public static float[] toFloatArray(@Nullable ReadableArray value, float[] defaultValue) {
        float[] parsedValue = toFloatArray(value);
        if (parsedValue == null){
            return defaultValue;
        }

        return parsedValue;
    }

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

    public static Vector toVector(ReadableArray value) {
        if (value == null) {
            return null;
        }

        if (value.size() != 3) {
            throw new IllegalArgumentException("Vectors require 3 values.");
        }

        return new Vector((float) value.getDouble(0), (float) value.getDouble(1), (float) value.getDouble(2));
    }

    public static Vector toRadiansVector(float[] degreesArray) {
        return new Vector(Math.toRadians(degreesArray[0]),
                Math.toRadians(degreesArray[1]), Math.toRadians(degreesArray[2]));
    }

    /**
     * This method takes a path and creates a Uri for it. If given a normal http path,
     * it'll leave it alone, but given a resource file name (that React gives us), it'll
     * return the corresponding android-resource:// uri (this is what React Native's
     * Image uses/does).
     */
    public static Uri parseUri(String path, Context context) {
        if (path == null) {
            return null;
        }
        Uri tempUri = Uri.parse(path);
        // if the scheme is null, then it's a local resource
        return tempUri.getScheme() == null ? getResourceUri(context, path) : tempUri;
    }

    private static Uri getResourceUri(Context context, @javax.annotation.Nullable String name) {
        int resId = getResourceId(context, name);
        return resId > 0 ? new Uri.Builder()
                .scheme(RESOURCE_SCHEME)
                .path(String.valueOf(resId))
                .build() : Uri.EMPTY;
    }

    private static int getResourceId(Context context, @javax.annotation.Nullable String name) {
        if (name == null || name.isEmpty()) {
            return 0;
        }
        name = name.toLowerCase().replace("-", "_");

        // name could be a resource id.
        try {
            return Integer.parseInt(name);
        } catch (NumberFormatException e) {
            // Do nothing.
        }

        int id = context.getResources().getIdentifier(
                name,
                "drawable",
                context.getPackageName());
        // Found the given resource name in drawable resource
        if (id > 0) return id;

        // Check if the resource name exists in raw resources
        return context.getResources().getIdentifier(
                name,
                "raw",
                context.getPackageName());
    }
}
