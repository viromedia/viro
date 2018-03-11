/**
 * Copyright © 2018 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.module;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.util.Log;

import com.amazonaws.mobileconnectors.s3.transfermanager.Download;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;
import com.viro.core.ARImageTarget;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;

import java.util.ArrayList;
import java.util.HashMap;

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

    private final HashMap<String, ARTargetPromise> mPromiseMap = new HashMap<>();

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

            if (!targetMap.hasKey("physicalWidth")) {
                throw new IllegalArgumentException("ARTrackingTargets - target [" + key
                        + "] is missing `physicalWidth` prop.");
            }
            float physicalWidth = (float) targetMap.getDouble("physicalWidth");

            if (!targetMap.hasKey("orientation")) {
                throw new IllegalArgumentException("ARTrackingTargets - target [" + key
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

            mPromiseMap.put(key, promise);
        }
    }

    @ReactMethod
    public void deleteTarget(final String targetName) {
        mPromiseMap.remove(targetName);
    }

    public ARTargetPromise getARTargetPromise(String targetName) {
        return mPromiseMap.get(targetName);
    }
}
