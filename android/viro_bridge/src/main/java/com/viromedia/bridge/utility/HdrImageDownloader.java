/*
 * Copyright © 2018 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.utility;

import android.content.Context;
import android.net.Uri;
import android.os.AsyncTask;
import android.support.annotation.UiThread;
import android.util.Log;
import com.facebook.react.bridge.ReadableMap;
import com.viro.core.Texture;

/**
 * Helper class for downloading HDR image data into a {@link Texture} object.
 */
public class HdrImageDownloader {
    private static final String TAG = ViroLog.getTag(HdrImageDownloader.class);
    private static final String URI_KEY = "uri";
    private static final String HDR_KEY = ".hdr";

    public static boolean isHDR(Uri uri) {
         return !(uri == null || uri.getPath() == null || !uri.getPath().toLowerCase().endsWith(HDR_KEY));
    }

    public static boolean isHDRReadableMap(ReadableMap map, Context context) {
        if (!map.hasKey(URI_KEY)) {
            return false;
        }

        return isHDR(Helper.parseUri(map.getString(URI_KEY), context));
    }

    public static void getHdrTextureAsync(ReadableMap map, DownloadListener listener, Context context) {
        boolean isHDR = isHDRReadableMap(map, context);
        if (!isHDR) {
            throw new IllegalArgumentException("Invalid HDR uri file path provided.");
        }

        Uri uri = Helper.parseUri(map.getString(URI_KEY), context);
        DownloadHDRAsyncTask task = new DownloadHDRAsyncTask(listener);
        task.execute(uri);
    }

    /**
     * Async Task for downloading and initializing HDR Image data into a Texture object on
     * the background thread.
     */
    private static class DownloadHDRAsyncTask extends AsyncTask<Uri, Integer, Texture> {
        private DownloadListener mListener;

        private DownloadHDRAsyncTask(DownloadListener listener) {
            mListener = listener;
        }

        protected Texture doInBackground(Uri... uriArray) {
            if (uriArray.length == 0) {
                return null;
            }

            // Do a sanity check.
            Uri uri = uriArray[0];
            if (uri == null || uri.getPath() == null || !uri.getPath().toLowerCase().endsWith(".hdr")) {
                Log.e(TAG,"Invalid HDR uri file path provided.");
                return null;
            }

            return Texture.loadRadianceHDRTexture(uri);
        }

        protected void onPostExecute(Texture result) {
            if (mListener == null || !mListener.isValid()) {
                return;
            }

            mListener.completed(result);
        }
    }

    public interface DownloadListener {
        /**
         * Whether or not the download listener is still valid
         */
        @UiThread
        public boolean isValid();
        public void completed(Texture result);
    }

}
