/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.module;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;
import com.facebook.react.bridge.ReadableType;
import com.viro.renderer.jni.AnimationChainJni;
import com.viro.renderer.jni.AnimationGroupJni;
import com.viro.renderer.jni.BaseAnimation;
import com.viro.renderer.jni.BaseAnimation.ExecutionType;
import com.viro.renderer.jni.LazyMaterialJni;
import com.viro.renderer.jni.MaterialJni;
import com.viromedia.bridge.utility.ViroLog;

import java.util.HashMap;


public class AnimationManager extends ReactContextBaseJavaModule {

    private static final String TAG = ViroLog.getTag(AnimationManager.class);

    static class LazyMaterialReact extends LazyMaterialJni {

        private String mName;
        private MaterialManager mMaterialManager;

        public LazyMaterialReact(String name, MaterialManager manager) {
            mName = name;
            mMaterialManager = manager;
        }

        public long get() {
            MaterialJni material = mMaterialManager.getMaterial(mName);
            if (material == null) {
                return 0;
            }

            return material.getNativeRef();
        }
    }

    private final HashMap<String, BaseAnimation> mParsedAnimations;
    private ReadableMap mRawAnimations;

    public AnimationManager(ReactApplicationContext context) {
        super(context);
        mParsedAnimations = new HashMap<>();
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

    public BaseAnimation getAnimation(String name) {
        return mParsedAnimations.get(name);
    }

    private void parseAnimations() {
        ReadableMapKeySetIterator iter = mRawAnimations.keySetIterator();
        while(iter.hasNextKey()) {
            String animationName = iter.nextKey();
            BaseAnimation animation = parseAnimationObjectHelper(animationName, ExecutionType.PARALLEL);
            mParsedAnimations.put(animationName, animation);
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
    private BaseAnimation parseAnimationObjectHelper(String animationName, ExecutionType executionType) {
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
    private BaseAnimation parseAnimationObjectHelper(ReadableArray array, int index,
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

    private BaseAnimation parseAnimationObject(String animationName, ExecutionType executionType) {
        return parseAnimationObjectHelper(animationName, ExecutionType.SERIAL);
    }

    private BaseAnimation parseAnimationObject(ReadableMap animationMap, ExecutionType executionType) {
        return parseAnimationMap(animationMap);
    }

    private BaseAnimation parseAnimationObject(ReadableArray animationArray, ExecutionType executionType) {
        AnimationChainJni animationChain = new AnimationChainJni(executionType);
        for (int i = 0; i < animationArray.size(); i++) {
            BaseAnimation childAnimation = parseAnimationObjectHelper(animationArray, i, ExecutionType.SERIAL);

            if (childAnimation instanceof AnimationGroupJni) {
                animationChain.addAnimation((AnimationGroupJni) childAnimation);
            } else if (childAnimation instanceof AnimationChainJni) {
                animationChain.addAnimation((AnimationChainJni) childAnimation);
            } else {
                ViroLog.warn(TAG, "Unknown BaseAnimation type [" + childAnimation.getClass().getSimpleName() + "]!");
            }
        }
        return animationChain;
    }

    private BaseAnimation parseAnimationMap(ReadableMap animationMap) {
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

        LazyMaterialJni lazyMaterial = null;

        // Currently we only support animating the index 0 material.
        if (propertyMap.hasKey("material")) {
            String material = propertyMap.getString("material");
            MaterialManager materialManager = getReactApplicationContext().getNativeModule(MaterialManager.class);
            lazyMaterial = new LazyMaterialReact(material, materialManager);
        }

        int durationMilliseconds = getPropertyAsInt(animationMap, "duration");
        int delayMilliseconds = getPropertyAsInt(animationMap, "delay");

        float durationSeconds = durationMilliseconds == Integer.MIN_VALUE ? 0 : durationMilliseconds / 1000f;
        float delaySeconds = delayMilliseconds == Integer.MIN_VALUE ? 0 : delayMilliseconds / 1000f;

        String functionType = getFloatPropertyAsString(animationMap, "easing");
        if (functionType == null) {
            functionType = "Linear";
        }
        return new AnimationGroupJni(positionX, positionY, positionZ,
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
     * This method tries to return the property pointed to by the key as an int from the map.
     *
     * @param map the property map
     * @param key the key of the property we want
     * @return an int or MIN_VALUE denoting that we didn't find the key/value pair.
     */
    private int getPropertyAsInt(ReadableMap map, String key) {
        if (map.hasKey(key) && map.getType(key) == ReadableType.Number) {
            return map.getInt(key);
        }
        return Integer.MIN_VALUE;
    }

}
