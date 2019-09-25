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

package com.viromedia.bridge.module;

import android.util.Log;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;
import com.facebook.react.bridge.ReadableType;
import com.facebook.react.module.annotations.ReactModule;
import com.viro.core.internal.AnimationChain;
import com.viro.core.internal.AnimationGroup;
import com.viro.core.internal.ExecutableAnimation;
import com.viro.core.internal.ExecutableAnimation.ExecutionType;
import com.viro.core.internal.LazyMaterial;
import com.viro.core.Material;
import com.viromedia.bridge.utility.ViroLog;

import java.util.HashMap;

@ReactModule(name = "VRTAnimationManager")
public class AnimationManager extends ReactContextBaseJavaModule {

    private static final String TAG = ViroLog.getTag(AnimationManager.class);

    static class LazyMaterialReact extends LazyMaterial {

        private String mName;
        private MaterialManager mMaterialManager;

        public LazyMaterialReact(String name, MaterialManager manager) {
            mName = name;
            mMaterialManager = manager;
        }

        public LazyMaterial copy() {
            return new LazyMaterialReact(mName, mMaterialManager);
        }

        public long get() {
            Material material = mMaterialManager.getMaterial(mName);
            if (material == null) {
                return 0;
            }

            return material.getNativeRef();
        }
    }

    /*
     TODO: change this back to non-static. Required for VIRO-3474.
     */
    private static HashMap<String, ExecutableAnimation> sParsedAnimations;
    public void clearAnimations() {
        sParsedAnimations = new HashMap<>();
    }

    private ReadableMap mRawAnimations;

    public AnimationManager(ReactApplicationContext context) {
        super(context);
        Log.e("Manish", "AnimationManager constructor");
        sParsedAnimations = new HashMap<>();
    }

    @Override
    public String getName() {
        return "VRTAnimationManager";
    }

    @ReactMethod
    public void setJSAnimations(ReadableMap animationsMap) {
        mRawAnimations = animationsMap;
        parseAnimations();
    }

    public ExecutableAnimation getAnimation(String name) {
        return sParsedAnimations.get(name);
    }

    private void parseAnimations() {
        ReadableMapKeySetIterator iter = mRawAnimations.keySetIterator();
        while(iter.hasNextKey()) {
            String animationName = iter.nextKey();
            ExecutableAnimation animation = parseAnimationObjectHelper(animationName, ExecutionType.PARALLEL);
            sParsedAnimations.put(animationName, animation);
            ViroLog.debug(TAG, "Parsed animation: [" + animationName + "]");
        }
    }

    /**
     * This function helps us branch between the 3 different animation object types
     *
     * @param animationName - name of the animation to parse in the map of raw animations
     * @param executionType - execution type
     * @return the parsed animation object
     */
    private ExecutableAnimation parseAnimationObjectHelper(String animationName, ExecutionType executionType) {
        switch(mRawAnimations.getType(animationName)) {
            case Array:
                return parseAnimationObject(mRawAnimations.getArray(animationName), executionType);
            case Map:
                return parseAnimationObject(mRawAnimations.getMap(animationName), executionType);
            case String:
                return parseAnimationObject(mRawAnimations.getString(animationName), executionType);
            default:
                ViroLog.error(TAG, "Invalid animation object received of type: ["
                        + mRawAnimations.getType(animationName) + "]");
                return null;
        }
    }

    /**
     * This function helps us branch between the 3 different animation object types
     *
     * @param array - the array of animation objects
     * @param index - the index of the animation object to parse
     * @param executionType - the execution type
     * @return the parsed animation
     */
    private ExecutableAnimation parseAnimationObjectHelper(ReadableArray array, int index,
                                                     ExecutionType executionType) {
        switch(array.getType(index)) {
            case Array:
                return parseAnimationObject(array.getArray(index), executionType);
            case Map:
                return parseAnimationObject(array.getMap(index), executionType);
            case String:
                return parseAnimationObject(array.getString(index), executionType);
            default:
                ViroLog.error(TAG, "Invalid animation object received of type: ["
                        + array.getType(index) + "]");
                return null;
        }
    }

    private ExecutableAnimation parseAnimationObject(String animationName, ExecutionType executionType) {
        return parseAnimationObjectHelper(animationName, ExecutionType.SERIAL);
    }

    private ExecutableAnimation parseAnimationObject(ReadableMap animationMap, ExecutionType executionType) {
        return parseAnimationMap(animationMap);
    }

    private ExecutableAnimation parseAnimationObject(ReadableArray animationArray, ExecutionType executionType) {
        AnimationChain animationChain = new AnimationChain(executionType);
        for (int i = 0; i < animationArray.size(); i++) {
            ExecutableAnimation childAnimation = parseAnimationObjectHelper(animationArray, i, ExecutionType.SERIAL);

            if (childAnimation instanceof AnimationGroup) {
                animationChain.addAnimation((AnimationGroup) childAnimation);
            } else if (childAnimation instanceof AnimationChain) {
                animationChain.addAnimation((AnimationChain) childAnimation);
            } else {
                ViroLog.warn(TAG, "Unknown ExecutableAnimation type [" + childAnimation.getClass().getSimpleName() + "]!");
            }
        }
        return animationChain;
    }

    private ExecutableAnimation parseAnimationMap(ReadableMap animationMap) {
        ReadableMap propertyMap = animationMap.getMap("properties");
        String positionX = getFloatPropertyAsString(propertyMap, "positionX");
        String positionY = getFloatPropertyAsString(propertyMap, "positionY");
        String positionZ = getFloatPropertyAsString(propertyMap, "positionZ");
        String scaleX = getFloatPropertyAsString(propertyMap, "scaleX");
        String scaleY = getFloatPropertyAsString(propertyMap, "scaleY");
        String scaleZ = getFloatPropertyAsString(propertyMap, "scaleZ");
        String rotateX = getFloatPropertyAsString(propertyMap, "rotateX");
        String rotateY = getFloatPropertyAsString(propertyMap, "rotateY");
        String rotateZ = getFloatPropertyAsString(propertyMap, "rotateZ");
        String opacity = getFloatPropertyAsString(propertyMap, "opacity");
        String color = getIntPropertyAsString(propertyMap, "color");

        LazyMaterial lazyMaterial = null;

        // Currently we only support animating the index 0 material.
        if (propertyMap.hasKey("material")) {
            String material = propertyMap.getString("material");
            MaterialManager materialManager = getReactApplicationContext().getNativeModule(MaterialManager.class);
            lazyMaterial = new LazyMaterialReact(material, materialManager);
        }

        float durationMilliseconds = (float) getPropertyAsDouble(animationMap, "duration");
        float delayMilliseconds = (float) getPropertyAsDouble(animationMap, "delay");

        float durationSeconds = durationMilliseconds == Double.MIN_VALUE ? 0 : durationMilliseconds / 1000f;
        float delaySeconds = delayMilliseconds == Double.MIN_VALUE ? 0 : delayMilliseconds / 1000f;

        String functionType = getFloatPropertyAsString(animationMap, "easing");
        if (functionType == null) {
            functionType = "Linear";
        }
        return new AnimationGroup(positionX, positionY, positionZ,
                scaleX, scaleY, scaleZ, rotateX, rotateY, rotateZ,
                opacity, color, lazyMaterial, durationSeconds, delaySeconds, functionType);
    }

    /**
     * This method tries to return the floating-point property pointed to by the key as a String from the map.
     * @param map the property map
     * @param key the key of the property we want
     * @return a String or null
     */
    private String getFloatPropertyAsString(ReadableMap map, String key) {
        if (map.hasKey(key)) {
            if (map.getType(key) == ReadableType.String) {
                return map.getString(key);
            } else if (map.getType(key) == ReadableType.Number) {
                return Double.toString(map.getDouble(key));
            }
        }
        return null;
    }

    /**
     * This method tries to return the int property pointed to by the key as a String from the map.
     * @param map the property map
     * @param key the key of the property we want
     * @return a String or null
     */
    private String getIntPropertyAsString(ReadableMap map, String key) {
        if (map.hasKey(key)) {
            if (map.getType(key) == ReadableType.String) {
                return map.getString(key);
            } else if (map.getType(key) == ReadableType.Number) {
                return Integer.toString(map.getInt(key));
            }
        }
        return null;
    }

    /**
     * This method tries to return the property pointed to by the key as a double from the map.
     *
     * @param map the property map
     * @param key the key of the property we want
     * @return a double or MIN_VALUE denoting that we didn't find the key/value pair.
     */
    private double getPropertyAsDouble(ReadableMap map, String key) {
        if (map.hasKey(key) && map.getType(key) == ReadableType.Number) {
            return map.getDouble(key);
        }
        return Double.MIN_VALUE;
    }

}
