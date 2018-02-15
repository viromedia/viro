/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.module;


import android.graphics.Bitmap;
import android.graphics.Color;
import android.net.Uri;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;
import com.facebook.react.bridge.ReadableType;
import com.viro.core.internal.Image;
import com.viro.core.Material;
import com.viro.core.Texture;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ImageDownloader;

import java.util.HashMap;
import java.util.Map;


public class MaterialManager extends ReactContextBaseJavaModule {

    // TODO: figure out a good place to load the libraries.
    static {
        System.loadLibrary("native-lib");
    }

    private final ReactApplicationContext mContext;
    private Map<String, MaterialWrapper> mMaterialsMap;
    private Map<String, Image> mImageMap;
    private boolean mShouldReload = false;

    public MaterialManager(ReactApplicationContext reactContext) {
        super(reactContext);
        mContext = reactContext;
        mMaterialsMap = new HashMap<String, MaterialWrapper>();
        mImageMap = new HashMap<String, Image>();
    }

    @Override
    public String getName() {
        return "VRTMaterialManager";
    }

    public Material getMaterial(String name) {
        reloadMaterials();

        if (mMaterialsMap.containsKey(name)) {
            return mMaterialsMap.get(name).getNativeMaterial();
        }
        return null;
    }

    /**
     * Call this method if we should reload/recreate all the materials the
     * next time we create them.
     */
    public void shouldReload() {
        mShouldReload = true;
    }

    /**
     * This function goes through every material and reloads the material
     */
    public void reloadMaterials() {
        if (mShouldReload) {
            for (String key : mMaterialsMap.keySet()) {
                MaterialWrapper material = mMaterialsMap.get(key);
                material.recreate();
            }
            mShouldReload = false;
        }
    }

    @ReactMethod
    public void setJSMaterials(ReadableMap newMaterials) {
        loadMaterials(newMaterials);
    }

    @ReactMethod
    public void deleteMaterials(ReadableArray materials) {
        for (int i = 0; i < materials.size(); i++) {
            String materialName = materials.getString(i);
            if (mMaterialsMap.containsKey(materialName)) {
                // we need to delete the native ref before we remove the material
                mMaterialsMap.get(materialName).getNativeMaterial().dispose();
                mMaterialsMap.remove(materialName);
            }
        }
    }

    private void loadMaterials(ReadableMap newMaterials) {
        ReadableMapKeySetIterator iter = newMaterials.keySetIterator();
        while (iter.hasNextKey()) {
            String key = iter.nextKey();
            ReadableMap material = newMaterials.getMap(key);
            MaterialWrapper materialWrapper = createMaterial(material);
            mMaterialsMap.put(key, materialWrapper);
        }
    }

    private MaterialWrapper createMaterial(ReadableMap materialMap) {
        MaterialWrapper materialWrapper = new MaterialWrapper(materialMap);

        // These defaults match those in the JNI's Material.java
        Material.LightingModel lightingModel = Material.LightingModel.CONSTANT;
        int diffuseColor = Color.WHITE;
        Texture diffuseTexture = null;
        float diffuseIntensity = 1.0f;
        Texture specularTexture = null;
        float shininess = 2.0f;
        float fresnelExponent = 1.0f;
        Texture normalMap = null;
        Material.CullMode cullMode = Material.CullMode.BACK;
        Material.TransparencyMode transparencyMode= Material.TransparencyMode.A_ONE;
        Material.BlendMode blendMode = Material.BlendMode.ALPHA;
        float bloomThreshold = -1.0f;
        boolean writesToDepthBuffer = true;
        boolean readsFromDepthBuffer = true;

        ReadableMapKeySetIterator iter = materialMap.keySetIterator();
        while(iter.hasNextKey()) {
            final String materialPropertyName = iter.nextKey();

            boolean isPbrProperty = false;
            for (PBRProperties property : PBRProperties.values()){
                if (property.key.equalsIgnoreCase(materialPropertyName)){
                    isPbrProperty = true;
                    break;
                }
            }
            if (isPbrProperty) {
                continue;
            }

            if (materialPropertyName.endsWith("texture") || materialPropertyName.endsWith("Texture")) {
                if (materialPropertyName.equalsIgnoreCase("reflectiveTexture")) {
                    Texture nativeTexture = createTextureCubeMap(materialMap.getMap(materialPropertyName), Texture.Format.RGBA8);
                    // TODO Reflective texture are unsupported currently
                    continue;
                }

                String path = parseImagePath(materialMap, materialPropertyName);
                Texture.Format format = parseImageFormat(materialMap, materialPropertyName);
                boolean mipmap = parseImageMipmap(materialMap, materialPropertyName);
                boolean sRGB = !materialPropertyName.startsWith("normal");

                Uri uri = Helper.parseUri(path, mContext);
                if (path != null) {
                    if (isVideoTexture(path)) {
                        materialWrapper.addVideoTexturePath(materialPropertyName, path);
                    } else {
                        if (mImageMap.get(materialPropertyName) != null) {
                            Texture texture = parseTexture(mImageMap.get(materialPropertyName), format, sRGB, mipmap,
                                    materialPropertyName, materialMap);
                            if (materialPropertyName.equalsIgnoreCase("diffuseTexture")) {
                                diffuseTexture = texture;
                            }
                            else if (materialPropertyName.equalsIgnoreCase("specularTexture")) {
                                specularTexture = texture;
                            }
                            else if (materialPropertyName.equalsIgnoreCase("normalTexture")) {
                                normalMap = texture;
                            }
                            else {
                                throw new IllegalArgumentException("Invalid texture property received: " + materialPropertyName);
                            }
                        } else {
                            ImageDownloader downloader = new ImageDownloader(mContext);
                            downloader.setTextureFormat(format);

                            Bitmap imageBitmap = downloader.getImageSync(uri);
                            if (imageBitmap != null) {
                                Image nativeImage = new Image(imageBitmap, format);
                                Texture texture = parseTexture(nativeImage, format, sRGB, mipmap,
                                        materialPropertyName, materialMap);
                                if (materialPropertyName.equalsIgnoreCase("diffuseTexture")) {
                                    diffuseTexture = texture;
                                }
                                else if (materialPropertyName.equalsIgnoreCase("specularTexture")) {
                                    specularTexture = texture;
                                }
                                else if (materialPropertyName.equalsIgnoreCase("normalTexture")) {
                                    normalMap = texture;
                                }
                                else {
                                    throw new IllegalArgumentException("Invalid texture property received: " + materialPropertyName);
                                }
                            }
                        }
                    }
                }
            } else if (materialPropertyName.endsWith("color") || materialPropertyName.endsWith("Color")) {
                int color = materialMap.getInt(materialPropertyName);
                if (materialPropertyName.equalsIgnoreCase("diffuseColor")) {
                    diffuseColor = color;
                }
                else {
                    throw new IllegalArgumentException("Invalid color property for material: " + materialPropertyName);
                }
            } else {
                if ("shininess".equalsIgnoreCase(materialPropertyName)) {
                    shininess = (float)materialMap.getDouble(materialPropertyName);
                } else if ("fresnelExponent".equalsIgnoreCase(materialPropertyName)) {
                    fresnelExponent = (float)materialMap.getDouble(materialPropertyName);
                } else if ("lightingModel".equalsIgnoreCase(materialPropertyName)) {
                    lightingModel = Material.LightingModel.valueFromString(materialMap.getString(materialPropertyName));
                } else if ("blendMode".equalsIgnoreCase(materialPropertyName)) {
                    blendMode = Material.BlendMode.valueFromString(materialMap.getString(materialPropertyName));
                } else if ("transparencyMode".equalsIgnoreCase(materialPropertyName)) {
                    transparencyMode = Material.TransparencyMode.valueFromString(materialMap.getString(materialPropertyName));
                } else if ("writesToDepthBuffer".equalsIgnoreCase(materialPropertyName)) {
                    writesToDepthBuffer = materialMap.getBoolean(materialPropertyName);
                } else if ("readsFromDepthBuffer".equalsIgnoreCase(materialPropertyName)) {
                    readsFromDepthBuffer = materialMap.getBoolean(materialPropertyName);
                } else if ("cullMode".equalsIgnoreCase(materialPropertyName)) {
                    cullMode = Material.CullMode.valueFromString(materialMap.getString(materialPropertyName));
                } else if ("diffuseIntensity".equalsIgnoreCase(materialPropertyName)) {
                    diffuseIntensity = (float)materialMap.getDouble(materialPropertyName);
                } else if ("bloomThreshold".equalsIgnoreCase(materialPropertyName)) {
                    bloomThreshold = (float)materialMap.getDouble(materialPropertyName);
                }
            }
        }

        Material nativeMaterial = new Material(lightingModel, diffuseColor, diffuseTexture,
                diffuseIntensity, specularTexture, shininess, fresnelExponent, normalMap,
                cullMode, transparencyMode, blendMode, bloomThreshold, writesToDepthBuffer,
                readsFromDepthBuffer);
        materialWrapper.setNativeMaterial(nativeMaterial);

        // Parse stuff
        parsePBRProperties(PBRProperties.METALNESS, nativeMaterial, materialMap);
        parsePBRProperties(PBRProperties.ROUGHNESS, nativeMaterial, materialMap);
        parsePBRProperties(PBRProperties.AMBIENT_OCCLUSION_TEXTURE, nativeMaterial, materialMap);

        // We don't need to hold a Java texture reference after assigning the texture to the material
        if (diffuseTexture != null) {
            diffuseTexture.dispose();
        }
        if (specularTexture != null) {
            specularTexture.dispose();
        }
        if (normalMap != null) {
            normalMap.dispose();
        }
        return materialWrapper;
    }

    private void parsePBRProperties(PBRProperties property, Material material, ReadableMap materialMap) {
        String key = property.key;
        if (!materialMap.hasKey(key)) {
            return;
        }

        if (key.endsWith("Texture") || key.endsWith("texture")){
            String path = parseImagePath(materialMap, key);
            if (path == null) {
                throw new IllegalArgumentException("Error: Unable to parse environment light map resource uri!");
            }

            Uri uri = Helper.parseUri(path, mContext);
            ImageDownloader downloader = new ImageDownloader(mContext);
            downloader.setTextureFormat(Texture.Format.RGB9_E5);

            Bitmap imageBitmap = downloader.getImageSync(uri);
            if (imageBitmap != null) {
                Image nativeImage = new Image(imageBitmap, Texture.Format.RGBA8);
                boolean sRGB = property == PBRProperties.AMBIENT_OCCLUSION_TEXTURE;
                Texture texture = parseTexture(nativeImage, Texture.Format.RGBA8, sRGB, false,
                        key, materialMap);

                property.setMapForMaterial(material, texture);
            } else {
                throw new IllegalArgumentException("Error: Unable to get environment light map resource!");
            }
        } else {
            float value = (float)materialMap.getDouble(key);
            property.setPropertyForMaterial(material, value);
        }
    }

    private Texture parseTexture(Image image, Texture.Format format, boolean sRGB, boolean mipmap,
                                 String name, ReadableMap materialMap) {
        Texture nativeTexture = new Texture(image, format, sRGB, mipmap);
        parseTexture(nativeTexture, name, materialMap);
        return nativeTexture;
    }

    private void parseTexture(Texture nativeTexture, String materialPropertyName, ReadableMap materialMap) {
        if (materialMap.hasKey("wrapS")) {
            nativeTexture.setWrapS(Texture.WrapMode.valueFromString(materialMap.getString("wrapS")));
        }
        if (materialMap.hasKey("wrapT")) {
            nativeTexture.setWrapT(Texture.WrapMode.valueFromString(materialMap.getString("wrapT")));
        }
        if (materialMap.hasKey("minificationFilter")) {
            nativeTexture.setMinificationFilter(Texture.FilterMode.valueFromString(materialMap.getString("minificationFilter")));
        }
        if (materialMap.hasKey("magnificationFilter")) {
            nativeTexture.setMagnificationFilter(Texture.FilterMode.valueFromString(materialMap.getString("magnificationFilter")));
        }
        if (materialMap.hasKey("mipFilter")) {
            nativeTexture.setMipFilter(Texture.FilterMode.valueFromString(materialMap.getString("mipFilter")));
        }
    }

    private Texture createTextureCubeMap(ReadableMap textureMap, Texture.Format format) {
        ReadableMapKeySetIterator iter = textureMap.keySetIterator();

        if (!iter.hasNextKey()) {
            throw new IllegalArgumentException("Error creating cube map: ensure the nx, px, ny, py, nz, and pz params are passed in the body of the cube map texture");
        }

        final Map<String, Image> cubeMapImages = new HashMap<String, Image>();
        long cubeSize = -1;

        // create an image for each texture
        while (iter.hasNextKey()) {
            final String key = iter.nextKey();
            if (mImageMap.get(key) != null) {
                cubeMapImages.put(key, mImageMap.get(key));
            } else {
                ImageDownloader downloader = new ImageDownloader(mContext);
                downloader.setTextureFormat(format);
                Image nativeImage = new Image(downloader.getImageSync(textureMap), format);
                cubeMapImages.put(key, nativeImage);
            }

            Image nativeImageToValidate = cubeMapImages.get(key);
            // check that the width == height and all sides are the same size
            if (cubeSize < 0) {
                cubeSize = nativeImageToValidate.getWidth();
            }

            if (nativeImageToValidate.getWidth() != cubeSize
                    || nativeImageToValidate.getHeight() != cubeSize) {
                throw new IllegalArgumentException("Error loading cube map. Cube map must be square and uniformly sized");
            }
        }

        // check that we have all 6 sides
        if (cubeMapImages.get("px") == null ||
                cubeMapImages.get("nx") == null ||
                cubeMapImages.get("py") == null ||
                cubeMapImages.get("ny") == null ||
                cubeMapImages.get("pz") == null ||
                cubeMapImages.get("nz") == null ) {
            throw new IllegalArgumentException("Some cube map images are null. Please check and fix");
        }
        // create and return a Texture w/ all 6 sides.
        return new Texture(cubeMapImages.get("px"), cubeMapImages.get("nx"),
                              cubeMapImages.get("py"), cubeMapImages.get("ny"),
                              cubeMapImages.get("pz"), cubeMapImages.get("nz"), format);
    }

    private String parseImagePath(ReadableMap map, String key) {
        if (map.getType(key) == ReadableType.String) {
            return map.getString(key);
        } else if (map.getType(key) == ReadableType.Map) {
            if (map.getMap(key).hasKey("source") && map.getMap(key).getType("source") == ReadableType.Map) {
                return map.getMap(key).getMap("source").getString("uri");
            }
            else {
                return map.getMap(key).getString("uri");
            }
        }
        // We don't know how to parse anything else... so just return.
        return null;
    }

    private Texture.Format parseImageFormat(ReadableMap map, String key) {
        Texture.Format format = Texture.Format.RGBA8;
        if (map.getType(key) == ReadableType.Map) {
            if (map.getMap(key).hasKey("format")) {
                format = Texture.Format.forString(map.getMap(key).getString("format"));
            }
        }
        return format;
    }

    private boolean parseImageMipmap(ReadableMap map, String key) {
        boolean mipmap = true;
        if (map.getType(key) == ReadableType.Map) {
            if (map.getMap(key).hasKey("mipmap")) {
                mipmap = map.getMap(key).getBoolean("mipmap");
            }
        }
        return mipmap;
    }

    private boolean isVideoTexture(String path) {
        return path.endsWith("mp4");
    }

    /**
     * Enum properties represented within a PBR material.
     */
    enum PBRProperties {
        METALNESS("metalness"),
        METALNESS_TEXTURE("metalnessTexture"),
        ROUGHNESS("roughness"),
        ROUGHNESS_TEXTURE("roughnessTexture"),
        AMBIENT_OCCLUSION_TEXTURE("ambientOcclusionTexture");
        final String key;

        PBRProperties(String strKey){
            key = strKey;
        }

        protected void setPropertyForMaterial(Material material, float value){
            if (this == METALNESS){
                material.setMetalness(value);
            } else if (this == ROUGHNESS){
                material.setRoughness(value);
            }
        }

        protected void setMapForMaterial(Material material, Texture map){
            if (this == METALNESS_TEXTURE){
                material.setMetalnessMap(map);
            } else if (this == ROUGHNESS_TEXTURE){
                material.setRoughnessMap(map);
            } else if (this == AMBIENT_OCCLUSION_TEXTURE){
                material.setAmbientOcclusionMap(map);
            }
        }
    }

    /**
     * MaterialWrapper Class
     */
    private class MaterialWrapper {
        private Material mNativeMaterial;
        // the source map that specified this material.
        private final ReadableMap mMaterialSource;
        private Map<String, String> mVideoTextures;

        public MaterialWrapper(ReadableMap source) {
            mVideoTextures = new HashMap<String, String>();
            mMaterialSource = source;
        }

        public void setNativeMaterial(Material material) { mNativeMaterial = material; }
        public Material getNativeMaterial() {
            return mNativeMaterial;
        }

        public void addVideoTexturePath(String name, String videoTexturePath) {
            mVideoTextures.put(name, videoTexturePath);
        }

        public void recreate() {
            if (mMaterialSource != null) {
                MaterialWrapper other = createMaterial(mMaterialSource);
                mNativeMaterial.dispose();
                mNativeMaterial = other.mNativeMaterial;
                mVideoTextures = other.mVideoTextures;
            }
        }
    }
}
