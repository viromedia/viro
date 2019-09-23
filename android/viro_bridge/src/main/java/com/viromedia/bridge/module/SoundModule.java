//  Copyright © 2017 Viro Media. All rights reserved.
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

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;
import com.facebook.react.bridge.ReadableType;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.module.annotations.ReactModule;
import com.viro.core.SoundData;
import java.util.HashMap;

@ReactModule(name = "VRTSoundModule")
public class SoundModule extends ReactContextBaseJavaModule {

    private final HashMap<String, SoundData> mSoundDataMap = new HashMap<>();

    public SoundModule(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSoundModule";
    }

    /**
     * This function takes a map of keys to sounds and creates SoundData objects
     * to prefetch the data before storing them in a local map. We currently only
     * support external urls (web-based).
     *
     * @param soundMap a map of String keys to String url's.
     */
    @ReactMethod
    public void preloadSounds(final ReadableMap soundMap, final Promise onSoundPreloadedCallback) {
        ReadableMapKeySetIterator iter = soundMap.keySetIterator();
        while(iter.hasNextKey()) {
            final String key = iter.nextKey();
            ReadableType keyType = soundMap.getType(key);
            String path;
            if (keyType == ReadableType.String){
                path = soundMap.getString(key);
            } else if (keyType == ReadableType.Map && soundMap.getMap(key).hasKey("uri")){
                path = soundMap.getMap(key).getString("uri");
            } else {
                throw new IllegalArgumentException("Invalid preloaded sound path received.");
            }

            // If a promise is provided, create SoundDataInitCallback to notify
            // the javascript layer of when the data has completed preloading.
            SoundData.SoundDataInitializeCallback callback = null;
            if (onSoundPreloadedCallback != null) {
                callback = new SoundData.SoundDataInitializeCallback() {
                    @Override
                    public void onDataIsReady() {
                        WritableMap returnMap = Arguments.createMap();
                        returnMap.putString("key", key);
                        returnMap.putBoolean("result",true);
                        returnMap.putString("msg","");
                        onSoundPreloadedCallback.resolve(returnMap);
                    }

                    @Override
                    public void onDataError(String errorMsg) {
                        WritableMap returnMap = Arguments.createMap();
                        returnMap.putString("key", key);
                        returnMap.putBoolean("result",false);
                        returnMap.putString("msg", errorMsg);
                        onSoundPreloadedCallback.resolve(returnMap);
                    }
                };
            }
            mSoundDataMap.put(key, new SoundData(path, callback));
        }
    }

    @ReactMethod
    public void unloadSounds(ReadableArray soundArray) {
        for (int i = 0; i < soundArray.size(); i++) {
            String keyToRemove = soundArray.getString(i);
            SoundData dataToRemove = mSoundDataMap.get(keyToRemove);
            if (dataToRemove != null) {
                dataToRemove.destroy();
                mSoundDataMap.remove(keyToRemove);
            }
        }
    }

    public SoundData getSoundData(String name) {
        return mSoundDataMap.get(name);
    }
}
