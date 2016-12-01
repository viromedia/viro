/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.module;


import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;
import com.facebook.react.bridge.ReadableType;
import com.viro.renderer.jni.ImageJni;
import com.viro.renderer.jni.MaterialJni;
import com.viro.renderer.jni.TextureJni;

import java.util.HashMap;
import java.util.Map;


public class MaterialManager extends ReactContextBaseJavaModule {

    // TODO: figure out a good place to load the libraries.
    static {
        System.loadLibrary("native-lib");
    }

    private Map<String, MaterialWrapper> mMaterialsMap;
    private Map<String, ImageJni> mImageMap;

    public MaterialManager(ReactApplicationContext reactContext) {
        super(reactContext);
        mMaterialsMap = new HashMap<String, MaterialWrapper>();
        mImageMap = new HashMap<String, ImageJni>();
    }

    @Override
    public String getName() {
        return "VROMaterialManager";
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
        MaterialJni nativeMaterial = new MaterialJni();
        // default settings for material
        nativeMaterial.setWritesToDepthBuffer(true);
        nativeMaterial.setReadsFromDepthBuffer(true);

        MaterialWrapper materialWrapper = new MaterialWrapper(nativeMaterial);

        ReadableMapKeySetIterator iter = materialMap.keySetIterator();
        while(iter.hasNextKey()) {
            String materialPropertyName = iter.nextKey();

            if (materialPropertyName.endsWith("texture") || materialPropertyName.endsWith("Texture")) {
                if (materialPropertyName.equalsIgnoreCase("reflectiveTexture")) {
                    TextureJni nativeTexture = createTextureCubeMap(materialMap.getMap(materialPropertyName));
                    setTextureOnMaterial(nativeMaterial, nativeTexture, materialPropertyName);
                    continue;
                }

                String path = parseImagePath(materialMap, materialPropertyName);
                if (path != null) {
                    if (isVideoTexture(path)) {
                        materialWrapper.addVideoTexturePath(materialPropertyName, path);
                    } else {
                        TextureJni nativeTexture = new TextureJni(retrieveImage(materialMap, materialPropertyName));
                        setTextureOnMaterial(nativeMaterial, nativeTexture, materialPropertyName);
                    }
                }
            } else if (materialPropertyName.endsWith("color") || materialPropertyName.endsWith("Color")) {
                int color = materialMap.getInt(materialPropertyName);
                nativeMaterial.setColor(color, materialPropertyName);
            } else {
                if ("shininess".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setShininess(materialMap.getDouble(materialPropertyName));
                } else if ("fresnelExponent".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setFresnelExponent(materialMap.getDouble(materialPropertyName));
                } else if ("lightingModel".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setLightingModel(materialMap.getString(materialPropertyName));
                } else if ("transparencyMode".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setTransparencyMode(materialMap.getString(materialPropertyName));
                } else if ("writesToDepthBuffer".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setWritesToDepthBuffer(materialMap.getBoolean(materialPropertyName));
                } else if ("readsFromDepthBuffer".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setReadsFromDepthBuffer(materialMap.getBoolean(materialPropertyName));
                } else if ("cullMode".equalsIgnoreCase(materialPropertyName)) {
                    nativeMaterial.setCullMode(materialMap.getString(materialPropertyName));
                }
            }
        }
        return materialWrapper;
    }

    private void setTextureOnMaterial(MaterialJni nativeMaterial, TextureJni nativeTexture,
                                      String materialPropertyName) {
        nativeMaterial.setTexture(nativeTexture, materialPropertyName);
        // Since we're actually done with texture at this point, destroy the JNI object.
        nativeTexture.destroy();
    }

    private TextureJni createTextureCubeMap(ReadableMap textureMap) {
        ReadableMapKeySetIterator iter = textureMap.keySetIterator();

        if (!iter.hasNextKey()) {
            throw new IllegalArgumentException("Error creating cube map: ensure the nx, px, ny, py, nz, and pz params are passed in the body of the cube map texture");
        }

        Map<String, ImageJni> cubeMapImages = new HashMap<String, ImageJni>();
        long cubeSize = -1;

        // create an image for each texture
        while (iter.hasNextKey()) {
            String key = iter.nextKey();
            ImageJni nativeImage = retrieveImage(textureMap, key);
            cubeMapImages.put(key, nativeImage);

            // check that the width == height and all sides are the same size
            if (cubeSize < 0) {
                cubeSize = nativeImage.getWidth();
            }

            if (nativeImage.getWidth() != cubeSize || nativeImage.getHeight() != cubeSize) {
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
        // create and return a TextureJni w/ all 6 sides.
        return new TextureJni(cubeMapImages.get("px"), cubeMapImages.get("nx"),
                              cubeMapImages.get("py"), cubeMapImages.get("ny"),
                              cubeMapImages.get("pz"), cubeMapImages.get("nz"));
    }

    private ImageJni retrieveImage(ReadableMap map, String key) {
        String path = parseImagePath(map, key);
        ImageJni nativeImage = mImageMap.get(path);
        if (nativeImage == null) {
            nativeImage = new ImageJni(path);
            mImageMap.put(path, nativeImage);
        }
        return nativeImage;
    }

    private String parseImagePath(ReadableMap map, String key) {
        if (map.getType(key) == ReadableType.String) {
            return map.getString(key);
        } else if (map.getType(key) == ReadableType.Map) {
            return map.getMap(key).getString("uri");
        }
        // We don't know how to parse anything else... so just return.
        return null;
    }

    private boolean isVideoTexture(String path) {
        return path.endsWith("mp4");
    }

    /**
     * MaterialWrapper Class
     */
    private class MaterialWrapper {
        private final MaterialJni mNativeMaterial;
        private final Map<String, String> mVideoTextures;

        public MaterialWrapper(MaterialJni nativeMaterial) {
            mVideoTextures = new HashMap<String, String>();
            mNativeMaterial = nativeMaterial;
        }

        public MaterialJni getNativeMaterial() {
            return mNativeMaterial;
        }

        public void addVideoTexturePath(String name, String videoTexturePath) {
            mVideoTextures.put(name, videoTexturePath);
        }
    }
}