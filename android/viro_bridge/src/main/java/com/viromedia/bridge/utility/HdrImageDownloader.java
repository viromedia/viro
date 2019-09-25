//  Copyright © 2018 Viro Media. All rights reserved.
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
import android.os.AsyncTask;
import androidx.annotation.UiThread;
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

    public static void getHdrTextureAsync(ReadableMap map, DownloadListener listener, Context context) {
        if (!map.hasKey(URI_KEY)) {
            throw new IllegalArgumentException("Missing HDR uri file path.");
        }

        Uri hdrUri = Helper.parseUri(map.getString(URI_KEY), context);
        if (hdrUri == null || hdrUri.getPath() == null ) {
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
            if (uri == null || uri.getPath() == null) {
                Log.e(TAG,"Invalid HDR uri file path provided when executing download hdr task!");
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
