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

package com.viromedia.bridge.module;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.util.Log;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;
import com.facebook.react.module.annotations.ReactModule;
import com.viro.core.ARImageTarget;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;

import java.util.ArrayList;
import java.util.HashMap;

@ReactModule(name = "VRTARTrackingTargetsModule")
public class ARTrackingTargetsModule extends ReactContextBaseJavaModule {

    public interface ARTargetPromiseListener {
        void onComplete(String key, ARImageTarget target);
        void onError(Exception e);
    }

    /**
     * This class follows the Promise pattern in that it will asynchronously fetch the data while
     * allowing the function to return immediately.
     */
    public class ARTargetPromise {

        private final String mKey;
        private final ReadableMap mSource;
        private final ARImageTarget.Orientation mOrientation;
        private final float mPhysicalWidth;
        private ARImageTarget mARImageTarget;
        private ArrayList<ARTargetPromiseListener> mPromiseListeners;
        private boolean mReady;

        public ARTargetPromise(String key, ReadableMap source,
                               ARImageTarget.Orientation orientation, float physicalWidth) {
            mKey = key;
            mSource = source;
            mOrientation = orientation;
            mPhysicalWidth = physicalWidth;
            mReady = false;
            mPromiseListeners = new ArrayList<>();
        }

        /**
         * This method should be called only once and immediately after it is set.
         */
        private void fetch(Context context) {
            if (mARImageTarget != null) {
                return;
            }

            ImageDownloader downloader = new ImageDownloader(context);
            downloader.getImageAsync(mSource, new ImageDownloadListener() {
                @Override
                public boolean isValid() {
                    return true;
                }

                @Override
                public void completed(Bitmap result) {
                    mARImageTarget = new ARImageTarget(result, mOrientation, mPhysicalWidth);

                    synchronized(this) {
                        mReady = true;
                    }

                    for (ARTargetPromiseListener listener : mPromiseListeners) {
                        listener.onComplete(mKey, mARImageTarget);
                    }
                }

                @Override
                public void failed(String error) {
                    String errorMessage = "Failed to download source for target: [" + mKey
                            + "], error:\n" + error;
                    Log.e("ARTrackingTargetsModule", errorMessage);
                    for (ARTargetPromiseListener listener : mPromiseListeners) {
                        listener.onError(new IllegalStateException(errorMessage));
                    }
                    mPromiseListeners = new ArrayList<>();
                }
            });

            // fetch image, create ImageTarget and return if there was a listener.
        }

        public void wait(final ARTargetPromiseListener listener) {
            // The sync block is to guard the mReady check and the addition of the listener
            synchronized (this) {
                if (mReady) {
                    // we don't want to invoke the onComplete in the sync block, so post a Runnable
                    // on this thread to invoke it.
                    new Handler().post(new Runnable() {
                        @Override
                        public void run() {
                            listener.onComplete(mKey, mARImageTarget);
                        }
                    });
                } else {
                    mPromiseListeners.add(listener);
                }
            }
        }

    }

    /*
     TODO: change this back to non-static. Required for VIRO-3474.
     */
    private static HashMap<String, ARTargetPromise> sPromiseMap = new HashMap<>();
    public void clearTargets() {
        sPromiseMap = new HashMap<>();
    }

    public ARTrackingTargetsModule(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    public String getName() {
        return "VRTARTrackingTargetsModule";
    }

    @ReactMethod
    public void createTargets(final ReadableMap targetsMap) {
        ReadableMapKeySetIterator iter = targetsMap.keySetIterator();
        while (iter.hasNextKey()) {
            String key = iter.nextKey();
            ReadableMap targetMap = targetsMap.getMap(key);

            if (targetMap.hasKey("type") && targetMap.getString("type").equalsIgnoreCase("Object")) {
                // don't do anything for "Object" targets now
                continue;
            }

            if (!targetMap.hasKey("physicalWidth")) {
                throw new IllegalArgumentException("ARTrackingTargets - image target [" + key
                        + "] is missing `physicalWidth` prop.");
            }
            float physicalWidth = (float) targetMap.getDouble("physicalWidth");

            if (!targetMap.hasKey("orientation")) {
                throw new IllegalArgumentException("ARTrackingTargets - image target [" + key
                        + "] is missing `orientation` prop.");
            }
            String orientationValue = targetMap.getString("orientation");
            ARImageTarget.Orientation orientation =
                    ARImageTarget.Orientation.valueFromString(orientationValue);

            if (orientation == null) {
                throw new IllegalArgumentException("ARTrackingTargets - target [" + key
                        + "] has invalid orientation value [" + orientationValue + "]");
            }

            if (!targetMap.hasKey("source")) {
                throw new IllegalArgumentException("ARTrackingTargets - target [" + key
                        + "] is missing `source` prop.");
            }

            ReadableMap source = targetMap.getMap("source");

            ARTargetPromise promise = new ARTargetPromise(key, source, orientation, physicalWidth);
            promise.fetch(getReactApplicationContext());

            sPromiseMap.put(key, promise);
        }
    }

    @ReactMethod
    public void deleteTarget(final String targetName) {
        sPromiseMap.remove(targetName);
    }

    public ARTargetPromise getARTargetPromise(String targetName) {
        return sPromiseMap.get(targetName);
    }
}
