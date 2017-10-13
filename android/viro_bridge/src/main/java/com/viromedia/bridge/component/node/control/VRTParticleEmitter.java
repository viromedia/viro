/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.Handler;
import android.os.Looper;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.viro.renderer.jni.Image;
import com.viro.renderer.jni.Node;
import com.viro.renderer.jni.ParticleEmitter;
import com.viro.renderer.jni.ViroContext;
import com.viro.renderer.jni.Surface;
import com.viro.renderer.jni.TextureFormat;
import com.viro.renderer.jni.Texture;
import com.viromedia.bridge.component.node.VRTScene;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;

import java.util.ArrayList;

public class VRTParticleEmitter extends VRTControl {
    // Default constants for this particle emitter.
    public final static int DEFAULT_SPAWN_RATE_SEC = 10;
    public final static int DEFAULT_SPAWN_RATE_METER = 0;
    public final static int DEFAULT_PARTICLE_LIFETIME = 2000;
    public final static int DEFAULT_MAX_PARTICLE = 500;
    public final static float DEFAULT_DELAY = 0;
    public final static float DEFAULT_DURATION = 2000;
    public final static boolean DEFAULT_RUN = false;
    public final static boolean DEFAULT_LOOP = true;
    public final static boolean DEFAULT_FIXED_TO_EMITTER = true;
    private boolean mFixedToEmitter = DEFAULT_FIXED_TO_EMITTER;
    private boolean mRun = DEFAULT_RUN;
    private boolean mLoop = DEFAULT_LOOP;
    private float mDelay = DEFAULT_DELAY;
    private float mDuration = DEFAULT_DURATION;

    // Native components representing this emitter, and it's particle geometry.
    private ParticleEmitter mNativeEmitter = null;
    private Surface mNativeSurface = null;

    // True if the image source, or it's configuration has changed.
    private boolean mImageNeedsUpdate = false;

    // True if changes made to this emitter's properties requires a restart.
    private boolean mEmitterNeedsRestart = false;

    // Components tracking the loaded image for particles in this emitter.
    private ReadableMap mImage = null;
    private String mCurrentImageUri = null;
    private Image mLatestImage;
    private Texture mLatestTexture;

    // Components that download the particle's image.
    private Handler mMainHandler;
    private ImageParticleDownloadListener mImageDownloadListener = null;

    // Particle Emitter Modifiers
    private ReadableMap mSpawnBehavior = null;
    private ReadableMap mParticleAppearance = null;
    private ReadableMap mParticlePhysics = null;

    // Default particle emitter configurations
    private ParticleEmitter.ParticleModifier mDefaultAlphaMod;
    private ParticleEmitter.ParticleModifier mDefaultScaleMode;
    private ParticleEmitter.ParticleModifier mDefaultRotMod;
    private ParticleEmitter.ParticleModifier mDefaultColorMod;
    private ParticleEmitter.ParticleModifier mDefaultVelocity;
    private ParticleEmitter.ParticleModifier mDefaultAccelerationMod;

    public VRTParticleEmitter(ReactApplicationContext reactContext) {
        super(reactContext);
        mDefaultAlphaMod = new ParticleEmitter.ParticleModifier(new float[]{1, 0, 0});
        mDefaultScaleMode = new ParticleEmitter.ParticleModifier(new float[]{1, 1, 1});
        mDefaultRotMod = new ParticleEmitter.ParticleModifier(new float[]{0, 0, 0});
        mDefaultColorMod = new ParticleEmitter.ParticleModifier(new float[]{1, 1, 1});
        mDefaultVelocity = new ParticleEmitter.ParticleModifier(new float[]{-0.5f, 1, 0}, new float[]{0.5f, 1, 0});
        mDefaultAccelerationMod = new ParticleEmitter.ParticleModifier(new float[]{0, 0, 0});
        mMainHandler = new Handler(Looper.getMainLooper());
        mNativeSurface = new Surface(1,1, 0, 0, 1, 1);
    }

    @Override
    public void onTearDown(){
        if (mScene != null && !mScene.isTornDown() && mNativeEmitter != null){
            mScene.getNativeScene().removeParticleEmitter(mNativeEmitter);
        }

        if (mNativeEmitter != null){
            mNativeEmitter.destroy();
            mNativeEmitter = null;
        }

        if (mNativeSurface != null){
            mNativeSurface.dispose();
            mNativeSurface = null;
        }

        if (mImageDownloadListener != null){
            mImageDownloadListener.invalidate();
        }

        super.onTearDown();

        if (mLatestImage != null) {
            mLatestImage.destroy();
            mLatestImage = null;
        }

        if (mLatestTexture != null) {
            mLatestTexture.destroy();
            mLatestTexture = null;
        }
    }

    /*
     Basic emitter properties
     */
    public void setRun(boolean run){
        mRun = run;
    }

    public void setLoop(boolean loop){
        mLoop = loop;
    }

    public void setDelay(float delay){
        mDelay = delay;
        mEmitterNeedsRestart = true;
    }

    public void setDuration(float duration){
        mDuration = duration;
        mEmitterNeedsRestart = true;
    }

    public void setImage(ReadableMap image) {
        mImage = image;
        mImageNeedsUpdate = true;
    }

    public void setFixedToEmitter(boolean fixedToEmitter){
        mFixedToEmitter = fixedToEmitter;
    }

    /*
     Emitter modifier properties affecting appearance, movement and spawning behavior.
     */
    public void setSpawnBehavior(ReadableMap spawn) {
        mSpawnBehavior = spawn;
        mEmitterNeedsRestart = true;
    }

    public void setParticleAppearance(ReadableMap appearance) {
        mParticleAppearance = appearance;
        mEmitterNeedsRestart = true;
    }

    public void setParticlePhysics(ReadableMap physics) {
        mParticlePhysics = physics;
        mEmitterNeedsRestart = true;
    }

    @Override
    public void setViroContext(ViroContext context) {
        super.setViroContext(context);

        // Refresh the emitter when a new render context is set
        if (mNativeEmitter == null){
            onPropsSet();
        }
    }

    @Override
    public void setScene(VRTScene scene){
        super.setScene(scene);

        // Refresh the emitter when a new scene is set
        if (mNativeEmitter == null){
            onPropsSet();
        }
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if (isTornDown()) {
            return;
        }

        final Node node = getNodeJni();
        if (mViroContext == null || mScene == null || mScene.getNativeScene() == null|| node == null) {
            return;
        }

        if (mImage == null || !mImage.hasKey("source")){
            onError("Viro: Missing required Image for a Viro Particle Emitter!");
            return;
        }

        downloadImageIfNeeded();

        // If a particle source image is provided, wait for it to be loaded
        // into the texture before initializing the emitter with it.
        if (mLatestTexture == null) {
            return;
        }

        // Refresh the images set on the quad particles if needed.
        setImageOnParticle();

        // Create emitter if we haven't yet done so.
        if (mNativeEmitter == null) {
            mNativeEmitter = new ParticleEmitter(mViroContext, node, mNativeSurface);
            mScene.getNativeScene().addParticleEmitter(mNativeEmitter);
        }

        // Update modifiers.
        updateSpawnModifier();
        updateAppearanceModifier();
        updatePhysicsModifier();

        // update emitter states.
        updateEmitterState();

        // restart emitter if need be.
        if (mEmitterNeedsRestart){
            mNativeEmitter.resetEmissionCycle();
            mEmitterNeedsRestart = false;
        }
    }

    private void downloadImageIfNeeded(){
        ReadableMap imageMap = mImage.getMap("source");
        String imageUri = imageMap.getString("uri");

        // Return if nothing has changed.
        if ((mCurrentImageUri != null && imageUri != null && mCurrentImageUri.equals(imageUri))
                || (mCurrentImageUri == null && imageUri == null)){
            return;
        }

        mCurrentImageUri = imageUri;

        final ImageDownloader downloader = new ImageDownloader(getContext());
        if (mCurrentImageUri != null){
            mImageDownloadListener = new ImageParticleDownloadListener();
            downloader.getImageAsync(imageMap, mImageDownloadListener);
        } else {
            mImageDownloadListener.invalidate();
            mImageDownloadListener = null;
        }
    }

    private void setImageOnParticle(){
        if (!mImageNeedsUpdate) {
            return;
        }

        double width = (mImage != null && mImage.hasKey("width")) ? mImage.getDouble("width") : 1.0;
        double height = (mImage != null && mImage.hasKey("height")) ? mImage.getDouble("height") : 1.0;
        mNativeSurface.setWidth((float)width);
        mNativeSurface.setHeight((float)height);
        if (mLatestTexture != null){
            mNativeSurface.setImageTexture(mLatestTexture);
        }

        mImageNeedsUpdate = false;
    }

    private void imageDownloadDidFinish(Bitmap result) {
        if (isTornDown()){
            return;
        }

        if (mLatestImage != null) {
            mLatestImage.destroy();
        }

        if (mLatestTexture != null) {
            mLatestTexture.destroy();
        }

        mImageNeedsUpdate = true;
        mLatestImage = new Image(result, TextureFormat.RGBA8);
        mLatestTexture = new Texture(mLatestImage, TextureFormat.RGBA8, true, false, null);

        mImageDownloadListener.invalidate();
        mImageDownloadListener = null;
        onPropsSet();
    }

    private void updateEmitterState(){
        mNativeEmitter.setDelay(mDelay);
        mNativeEmitter.setDuration(mDuration);
        mNativeEmitter.setLoop(mLoop);
        mNativeEmitter.setRun(mRun);
        mNativeEmitter.setFixedToEmitter(mFixedToEmitter);


        if (mImage != null && mImage.hasKey("blendMode")){
            String strBlendMode = mImage.getString("blendMode");
            if (strBlendMode == null || !mNativeEmitter.setBlendMode(strBlendMode)){
                onError("Viro: Attempted to set an invalid Blend mode!");
                return;
            }
        } else {
            mNativeEmitter.setBlendMode("Add");
        }

        if (mImage != null && mImage.hasKey("bloomThreshold")){
            float threshold = (float) mImage.getDouble("bloomThreshold");
            mNativeEmitter.setParticleBloomThreshold(threshold);
        } else {
            mNativeEmitter.setParticleBloomThreshold(-1);
        }
    }

    private void updateSpawnModifier(){
        if (mSpawnBehavior == null){
            mNativeEmitter.setEmissionRatePerSecond(new int[]{DEFAULT_SPAWN_RATE_SEC, DEFAULT_SPAWN_RATE_SEC});
            mNativeEmitter.setEmissionRatePerMeter(new int[]{DEFAULT_SPAWN_RATE_METER,DEFAULT_SPAWN_RATE_METER});
            mNativeEmitter.setParticleLifetime(new int[]{DEFAULT_PARTICLE_LIFETIME, DEFAULT_PARTICLE_LIFETIME});
            mNativeEmitter.setMaxParticles(DEFAULT_MAX_PARTICLE);
            mNativeEmitter.setParticleBursts(new ArrayList<ParticleEmitter.ParticleBursts>());
            mNativeEmitter.setSpawnVolume("Point", null, false);
            return;
        }

        if (mSpawnBehavior.hasKey("emissionRatePerSecond")){
            ReadableArray readableParams = mSpawnBehavior.getArray("emissionRatePerSecond");
            int params[] = new int[readableParams.size()];
            for (int i = 0; i < readableParams.size(); i++) {
                params[i] = readableParams.getInt(i);
            }

            if (params.length != 2){
                onError("Invalid Emission Rate Per Second parameters, expected a [min, max]!");
                return;
            }

            mNativeEmitter.setEmissionRatePerSecond(params);
        } else {
            mNativeEmitter.setEmissionRatePerSecond(new int[]{DEFAULT_SPAWN_RATE_SEC, DEFAULT_SPAWN_RATE_SEC});
        }

        if (mSpawnBehavior.hasKey("emissionRatePerMeter")){
            ReadableArray readableParams = mSpawnBehavior.getArray("emissionRatePerMeter");
            int params[] = new int[readableParams.size()];
            for (int i = 0; i < readableParams.size(); i++) {
                params[i] = readableParams.getInt(i);
            }

            if (params.length != 2){
                onError("Invalid Emission Rate Per Metter parameters, expected a [min, max]!");
                return;
            }

            mNativeEmitter.setEmissionRatePerMeter(params);
        } else {
            mNativeEmitter.setEmissionRatePerMeter(new int[]{DEFAULT_SPAWN_RATE_METER,DEFAULT_SPAWN_RATE_METER});
        }

        if (mSpawnBehavior.hasKey("particleLifetime")){
            ReadableArray readableParams = mSpawnBehavior.getArray("particleLifetime");
            int params[] = new int[readableParams.size()];
            for (int i = 0; i < readableParams.size(); i++) {
                params[i] = readableParams.getInt(i);
            }

            if (params.length != 2){
                onError("Invalid particle lifetime parameters, expected a [min, max]!");
                return;
            }

            mNativeEmitter.setParticleLifetime(params);
        } else {
            mNativeEmitter.setParticleLifetime(new int[]{DEFAULT_PARTICLE_LIFETIME, DEFAULT_PARTICLE_LIFETIME});
        }

        if (mSpawnBehavior.hasKey("maxParticles")){
            int maxParticles = mSpawnBehavior.getInt("maxParticles");
            mNativeEmitter.setMaxParticles(maxParticles);
        } else {
            mNativeEmitter.setMaxParticles(DEFAULT_MAX_PARTICLE);
        }

        // Set emission bursts values.
        ArrayList<ParticleEmitter.ParticleBursts> bursts = new ArrayList<ParticleEmitter.ParticleBursts>();
        if (mSpawnBehavior.hasKey("emissionBurst")) {
            ReadableArray burstArray = mSpawnBehavior.getArray("emissionBurst");
            for (int i = 0; i < burstArray.size(); i++) {
                ReadableMap burstmap = burstArray.getMap(i);

                String referenceFactor;
                double valueStart;
                double coolPeriod;
                double min, max;
                double cycles;
                if (burstmap.hasKey("time")) {
                    referenceFactor = "time";
                    valueStart = burstmap.getDouble("time");
                    coolPeriod = burstmap.getDouble("cooldownPeriod");
                } else if (burstmap.hasKey("distance")) {
                    referenceFactor = "distance";
                    valueStart = burstmap.getDouble("distance");
                    coolPeriod = burstmap.getDouble("cooldownDistance");
                } else {
                    onError("Invalid Burst parameters provided!");
                    return;
                }

                min = burstmap.getDouble("min");
                max = burstmap.getDouble("max");
                cycles = burstmap.getDouble("cycles");

                ParticleEmitter.ParticleBursts burst = new ParticleEmitter.ParticleBursts(referenceFactor,
                        valueStart, coolPeriod, min, max, cycles);
                bursts.add(burst);
            }
        }
        mNativeEmitter.setParticleBursts(bursts);

        // Set Spawn Volume values.
        if (mSpawnBehavior.hasKey("spawnVolume")) {
            ReadableMap spawnVolume = mSpawnBehavior.getMap("spawnVolume");

            if (!spawnVolume.hasKey("shape")){
                onError("Viro: Spawn Volume missing required shape parameter!");
                return;
            }

            String stringShapeName = spawnVolume.getString("shape").toLowerCase();
            boolean spawnOnSurface = spawnVolume.hasKey("spawnOnSurface") ?
                    spawnVolume.getBoolean("spawnOnSurface") : false;

            ReadableArray vecParamsReadableArray = spawnVolume.getArray("params");
            float shapeParams[] = null;
            if (vecParamsReadableArray != null){
                shapeParams = new float[vecParamsReadableArray.size()];
                for (int vecIndex = 0; vecIndex < vecParamsReadableArray.size(); vecIndex++) {
                    shapeParams[vecIndex] = (float) vecParamsReadableArray.getDouble(vecIndex);
                }
            }

            if (!stringShapeName.equals("box")
                    && !stringShapeName.equals("sphere") && !stringShapeName.equals("point")){
                onError("Viro: Invalid spawn shape provided for particle burst parameter!");
                return;
            }

            mNativeEmitter.setSpawnVolume(stringShapeName, shapeParams, spawnOnSurface);
        } else {
            // set point as default shape
            mNativeEmitter.setSpawnVolume("Point", null, false);
        }
    }

    private void updatePhysicsModifier(){
        ParticleEmitter.ParticleModifier velocityMod = null;
        ParticleEmitter.ParticleModifier accelerationMod = null;

        if (mParticlePhysics != null){
            velocityMod = getModifier(mParticlePhysics, "velocity", true, false, false);
            accelerationMod = getModifier(mParticlePhysics, "acceleration", true, false, false);
        }

        velocityMod = velocityMod == null ? mDefaultVelocity : velocityMod;
        accelerationMod = accelerationMod == null ? mDefaultAccelerationMod : accelerationMod;

        // Set modifiers
        mNativeEmitter.setVelocityModifier(velocityMod);
        mNativeEmitter.setAccelerationModifier(accelerationMod);

        // Update explosive impulses
        if (mParticlePhysics != null && mParticlePhysics.hasKey("explosiveImpulse")) {
            ReadableMap map = mParticlePhysics.getMap("explosiveImpulse");

            if (!map.hasKey("position") || !map.hasKey("impulse")){
                onError("Viro: Missing required explosion impulse force and location for emitter!");
                return;
            }

            // Grab the position from which to perform detonation calculations.
            ReadableArray readableParams = map.getArray("position");
            if (readableParams.size() != 3) {
                onError("Property position for explosiveImpulse requires an array of 3 values.");
                return;
            }
            float explodePosition[] = new float[readableParams.size()];
            for (int i = 0; i < readableParams.size(); i++) {
                explodePosition[i] = (float) readableParams.getDouble(i);
            }

            // Grab the explosive impulse force to apply
            float explodeImpulse = (float) map.getDouble("impulse");

            // Grab reverse accleration if provided.
            float reverseAccel;
            if (map.hasKey("decelerationPeriod")) {
                reverseAccel = (float) map.getDouble("decelerationPeriod");
            } else {
                reverseAccel = -1;
            }
            mNativeEmitter.setExplosiveImpulse(explodeImpulse, explodePosition, reverseAccel);
        } else {
            mNativeEmitter.setExplosiveImpulse(-1, new float[]{0,0,0} ,-1);
        }
    }

    private void updateAppearanceModifier(){
        ParticleEmitter.ParticleModifier opacityMod = null;
        ParticleEmitter.ParticleModifier scaleMode = null;
        ParticleEmitter.ParticleModifier rotMod = null;
        ParticleEmitter.ParticleModifier colorMod = null;

        if (mParticleAppearance != null){
            opacityMod = getModifier(mParticleAppearance, "opacity", false, false, true);
            scaleMode = getModifier(mParticleAppearance, "scale", true, false, false);
            rotMod = getModifier(mParticleAppearance, "rotation", true, false, false);
            colorMod = getModifier(mParticleAppearance, "color", false, true, false);
        }

        // Apply defaults
        opacityMod = opacityMod == null ? mDefaultAlphaMod : opacityMod;
        scaleMode = scaleMode == null ? mDefaultScaleMode : scaleMode;
        rotMod = rotMod == null ? mDefaultRotMod : rotMod;
        colorMod = colorMod == null ? mDefaultColorMod : colorMod;

        mNativeEmitter.setOpacityModifier(opacityMod);
        mNativeEmitter.setScaleModifier(scaleMode);
        mNativeEmitter.setRotationModifier(rotMod);
        mNativeEmitter.setColorModifier(colorMod);
    }

    private ParticleEmitter.ParticleModifier getModifier(ReadableMap appearanceMap,
                                                            String property, boolean isVec3,
                                                            boolean isColor, boolean isFloat){
        if (!appearanceMap.hasKey(property)){
            return null;
        }

        final ReadableMap map = appearanceMap.getMap(property);

        // Both [min, max] initial values are required for particle modifiers.
        if (!map.hasKey("initialRange") || map.getArray("initialRange").size() != 2){
            onError("Incorrect parameters provided for the intervalRange on "+ property+", expected: [min, max]!");
            return null;
        }

        float initialRange[][] = getValueArrayFromDict(map, "initialRange", isVec3, isColor, isFloat);
        float interpolatedIntervals[][] = null;
        float interpolatedPoints[][] = null;

        if (initialRange == null){
            onError("Incorrect parameters provided for InitialRange!");
            return null;
        }

        // Parse out the interpolation data
        if (map.hasKey("interpolation")){
            ReadableArray interpolatedArray = map.getArray("interpolation");
            interpolatedIntervals = new float[interpolatedArray.size()][2];
            interpolatedPoints = new float[interpolatedArray.size()][3];

            for (int i = 0; i < interpolatedArray.size(); i ++){
                ReadableMap interpolatePoint = interpolatedArray.getMap(i);

                // Grab the interval representing this interpolation frame
                ReadableArray intervalArrayReadable = interpolatePoint.getArray("interval");
                if (intervalArrayReadable.size() != 2){
                    onError("Invalid interval provided for " + property + "! Expected a [max, min].");
                    return null;
                }
                float interval[] = new float[2];
                interval[0] = (float) intervalArrayReadable.getDouble(0);
                interval[1] = (float) intervalArrayReadable.getDouble(1);

                // Grab the targeted value to interpolate towards
                float[] value = getVecValueFromDict(interpolatePoint, "endValue", isVec3, isColor, isFloat);

                interpolatedIntervals[i] = interval;
                interpolatedPoints[i] = value;
            }
        }

        // Set the modifier on this emitter through JNI
        String factor = map.hasKey("factor") ? map.getString("factor") : "time";
        return new ParticleEmitter.ParticleModifier(factor, initialRange, interpolatedIntervals, interpolatedPoints);
    }

    private float[][] getValueArrayFromDict(ReadableMap map, String key,
                                          boolean isVec3, boolean isColor, boolean isFloat){
        if (!map.hasKey(key)){
            onError("Attempted to read an invalid property: " + key);
            return null;
        }

        ReadableArray valuesReadableArray = map.getArray(key);
        float[][] returnList = new float [valuesReadableArray.size()][3];
        for (int i = 0; i < valuesReadableArray.size(); i ++){
            if (isVec3){
                ReadableArray vecParamsReadableArray = valuesReadableArray.getArray(i);
                float vecParams[] = new float[vecParamsReadableArray.size()];
                for (int vecIndex = 0; vecIndex < vecParamsReadableArray.size(); vecIndex++) {
                    vecParams[vecIndex] = (float) vecParamsReadableArray.getDouble(vecIndex);
                }

                if (vecParams.length != 3){
                    onError("Property: " + key + " requires an array of 3 values.");
                    return null;
                } else {
                    returnList[i] = vecParams;
                }
            }

            if (isColor){
                int colorInt = valuesReadableArray.getInt(i);
                returnList[i] = new float[]{Color.red(colorInt), Color.green(colorInt), Color.blue(colorInt)};
            }

            if (isFloat){
                float value = (float)valuesReadableArray.getDouble(i);
                returnList[i] = new float[]{value, 0, 0};
            }
        }

        return returnList;
    }

    private float[] getVecValueFromDict(ReadableMap map, String key,
                                        boolean isVec3, boolean isColor, boolean isFloat){
        if (!map.hasKey(key)){
            onError("Attempted to read an invalid property: " + key);
            return new float[]{0,0,0};
        }

        if (isVec3){
            ReadableArray readableParams = map.getArray(key);
            float params[] = new float[readableParams.size()];
            for (int i = 0; i < readableParams.size(); i++) {
                params[i] = (float) readableParams.getDouble(i);
            }

            if (params.length != 3){
                onError("Property: " + key + " requires an array of 3 values.");
                return new float[]{0,0,0};
            }

            return params;
        }

        if (isColor){
            int colorInt = map.getInt(key);
            return new float[]{Color.red(colorInt), Color.green(colorInt), Color.blue(colorInt)};
        }

        if (isFloat){
            float value = (float)map.getDouble(key);
            return new float[]{value, 0, 0};
        }

        onError("In correct data type provided for property: " + key);
        return new float[]{0,0,0};
    }

    private class ImageParticleDownloadListener implements ImageDownloadListener {
        private boolean mIsValid = true;

        public void invalidate() {
            mIsValid = false;
        }

        @Override
        public boolean isValid() {
            return mIsValid;
        }

        @Override
        public void completed(final Bitmap result) {
            mMainHandler.post(new Runnable() {
                public void run() {
                    if (!isValid()) {
                        return;
                    }
                    imageDownloadDidFinish(result);
                }
            });
        }

        @Override
        public void failed(String error) {
            if (!isValid()) {
                return;
            }
            onError(error);
        }
    }
}