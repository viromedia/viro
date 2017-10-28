/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.module;


import android.graphics.Bitmap;
import android.net.Uri;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;
import com.facebook.react.bridge.ReadableType;
import com.viro.renderer.jni.Image;
import com.viro.renderer.jni.Material;
import com.viro.renderer.jni.Texture.TextureFormat;
import com.viro.renderer.jni.Texture;
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
        final Material nativeMaterial = new Material();
        MaterialWrapper materialWrapper = new MaterialWrapper(nativeMaterial, materialMap);

        ReadableMapKeySetIterator iter = materialMap.keySetIterator();
        while(iter.hasNextKey()) {
            final String materialPropertyName = iter.nextKey();

            if (materialPropertyName.endsWith("texture") || materialPropertyName.endsWith("Texture")) {
                if (materialPropertyName.equalsIgnoreCase("reflectiveTexture")) {
                    Texture nativeTexture = createTextureCubeMap(materialMap.getMap(materialPropertyName), TextureFormat.RGBA8);
                    setTextureOnMaterial(nativeMaterial, nativeTexture, materialPropertyName, materialMap);
                    continue;
                }

                String path = parseImagePath(materialMap, materialPropertyName);
                TextureFormat format = parseImageFormat(materialMap, materialPropertyName);
                boolean mipmap = parseImageMipmap(materialMap, materialPropertyName);
                boolean sRGB = !materialPropertyName.startsWith("normal");

                Uri uri = Helper.parseUri(path, mContext);
                if (path != null) {
                    if (isVideoTexture(path)) {
                        materialWrapper.addVideoTexturePath(materialPropertyName, path);
                    } else {
                        if (mImageMap.get(materialPropertyName) != null) {
                            setImageOnMaterial(mImageMap.get(materialPropertyName), format, sRGB, mipmap, nativeMaterial,
                                    materialPropertyName, materialMap);
                        } else {
                            ImageDownloader downloader = new ImageDownloader(mContext);
                            downloader.setTextureFormat(format);

                            Bitmap imageBitmap = downloader.getImageSync(uri);
                            if (imageBitmap != null) {
                                Image nativeImage = new Image(imageBitmap, format);
                                setImageOnMaterial(nativeImage, format, sRGB, mipmap, nativeMaterial, materialPropertyName, materialMap);
                            }
                        }
                    }
                }
            } else if (materialPropertyName.endsWith("color") || materialPropertyName.endsWith("Color")) {
                int color = materialMap.getInt(materialPropertyName);
                if (materialPropertyName.equalsIgnoreCase("diffuseColor")) {
                    nativeMaterial.setDiffuseColor(color);
                }
                else {
                    throw new IllegalArgumentException("Invalid color property for material: " + materialPropertyName);
                }
            } else {
                if ("shininess".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setShininess((float)materialMap.getDouble(materialPropertyName));
                } else if ("fresnelExponent".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setFresnelExponent((float)materialMap.getDouble(materialPropertyName));
                } else if ("lightingModel".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setLightingModel(Material.LightingModel.valueFromString(materialMap.getString(materialPropertyName)));
                } else if ("transparencyMode".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setTransparencyMode(Material.TransparencyMode.valueFromString(materialMap.getString(materialPropertyName)));
                } else if ("writesToDepthBuffer".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setWritesToDepthBuffer(materialMap.getBoolean(materialPropertyName));
                } else if ("readsFromDepthBuffer".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setReadsFromDepthBuffer(materialMap.getBoolean(materialPropertyName));
                } else if ("cullMode".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setCullMode(Material.CullMode.valueFromString(materialMap.getString(materialPropertyName)));
                } else if ("diffuseIntensity".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setDiffuseIntensity((float)materialMap.getDouble(materialPropertyName));
                } else if ("bloomThreshold".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setBloomThreshold((float)materialMap.getDouble(materialPropertyName));
                }
            }
        }
        return materialWrapper;
    }

    private void setImageOnMaterial(Image image, TextureFormat format, boolean sRGB, boolean mipmap,
                                    Material material, String name, ReadableMap materialMap) {
        Texture nativeTexture = new Texture(image, format, sRGB, mipmap);
        setTextureOnMaterial(material, nativeTexture, name, materialMap);
    }

    private void setTextureOnMaterial(Material nativeMaterial, Texture nativeTexture,
                                      String materialPropertyName, ReadableMap materialMap) {
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

        if (materialPropertyName.equalsIgnoreCase("diffuseTexture")) {
            nativeMaterial.setDiffuseTexture(nativeTexture);
        }
        else if (materialPropertyName.equalsIgnoreCase("specularTexture")) {
            nativeMaterial.setSpecularTexture(nativeTexture);
        }
        else if (materialPropertyName.equalsIgnoreCase("normalTexture")) {
            nativeMaterial.setNormalMap(nativeTexture);
        }
        else {
            throw new IllegalArgumentException("Invalid texture property received: " + materialPropertyName);
        }
        // Since we're actually done with texture at this point, destroy the JNI object.
        nativeTexture.dispose();
    }

    private Texture createTextureCubeMap(ReadableMap textureMap, TextureFormat format) {
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

    private TextureFormat parseImageFormat(ReadableMap map, String key) {
        TextureFormat format = TextureFormat.RGBA8;
        if (map.getType(key) == ReadableType.Map) {
            if (map.getMap(key).hasKey("format")) {
                format = TextureFormat.forString(map.getMap(key).getString("format"));
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
     * MaterialWrapper Class
     */
    private class MaterialWrapper {
        private Material mNativeMaterial;
        // the source map that specified this material.
        private final ReadableMap mMaterialSource;
        private Map<String, String> mVideoTextures;

        public MaterialWrapper(Material nativeMaterial, ReadableMap source) {
            mVideoTextures = new HashMap<String, String>();
            mNativeMaterial = nativeMaterial;
            mMaterialSource = source;
        }

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
