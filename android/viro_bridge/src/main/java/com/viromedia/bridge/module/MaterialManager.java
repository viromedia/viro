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


import android.graphics.Bitmap;
import android.graphics.Color;
import android.net.Uri;
import android.util.Log;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;
import com.facebook.react.bridge.ReadableType;
import com.facebook.react.module.annotations.ReactModule;
import com.viro.core.VideoTexture;
import com.viro.core.internal.Image;
import com.viro.core.Material;
import com.viro.core.Texture;
import com.viromedia.bridge.component.VRTMaterialVideo;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ImageDownloader;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@ReactModule(name = "VRTMaterialManager")
public class MaterialManager extends ReactContextBaseJavaModule {

    // TODO: figure out a good place to load the libraries.
    static {
        System.loadLibrary("viro_renderer");
    }

    // TODO: make the following non-static!
    /*
     The following two static Maps and the clearMaterials function are a result of VIRO-3474, which
     is caused by a React Native bug, if/when they fix it, we need to revert the maps back to instance
     variables.
     */
    private static Map<String, MaterialWrapper> sMaterialsMap;
    private static Map<String, Image> sImageMap;
    private static Map<String, WeakReference<MaterialChangeListener>> sMaterialChangeListeners;
    public void clearMaterials() {
        sMaterialsMap = new HashMap<>();
        sImageMap = new HashMap<>();
        sMaterialChangeListeners = new HashMap<>();
    }

    private final ReactApplicationContext mContext;
    private boolean mShouldReload = false;

    public MaterialManager(ReactApplicationContext reactContext) {
        super(reactContext);
        mContext = reactContext;
        sMaterialsMap = new HashMap<String, MaterialWrapper>();
        sImageMap = new HashMap<String, Image>();
        sMaterialChangeListeners = new HashMap<String, WeakReference<MaterialChangeListener>>();
    }

    @Override
    public String getName() {
        return "VRTMaterialManager";
    }

    public Material getMaterial(String name) {
        reloadMaterials();
        if (sMaterialsMap.containsKey(name)) {
            return sMaterialsMap.get(name).getNativeMaterial();
        }
        return null;
    }

    public MaterialWrapper getMaterialWrapper(String name) {
        reloadMaterials();
        if (sMaterialsMap.containsKey(name)) {
            return sMaterialsMap.get(name);
        }
        return null;
    }

    public boolean isVideoMaterial(String name) {
        if (sMaterialsMap.containsKey(name)) {
            if (sMaterialsMap.get(name).hasVideoTextures()) {
                return true;
            }
        }
        return false;
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
            for (String key : sMaterialsMap.keySet()) {
                MaterialWrapper material = sMaterialsMap.get(key);
                material.recreate();
            }
            mShouldReload = false;
        }
    }

    public void setMaterialChangeListener(String name, MaterialChangeListener listener) {
        sMaterialChangeListeners.put(name, new WeakReference<MaterialChangeListener>(listener));
    }

    @ReactMethod
    public void setJSMaterials(ReadableMap newMaterials) {
        loadMaterials(newMaterials);
    }

    @ReactMethod
    public void deleteMaterials(ReadableArray materials) {
        for (int i = 0; i < materials.size(); i++) {
            String materialName = materials.getString(i);
            if (sMaterialsMap.containsKey(materialName)) {
                // we need to delete the native ref before we remove the material
                sMaterialsMap.get(materialName).getNativeMaterial().dispose();
                sMaterialsMap.remove(materialName);
            }
        }
    }

    private void loadMaterials(ReadableMap newMaterials) {
        ReadableMapKeySetIterator iter = newMaterials.keySetIterator();
        while (iter.hasNextKey()) {
            String key = iter.nextKey();
            ReadableMap material = newMaterials.getMap(key);
            MaterialWrapper materialWrapper = createMaterial(key, material);
            sMaterialsMap.put(key, materialWrapper);
        }
    }

    private MaterialWrapper createMaterial(String materialName, ReadableMap materialMap) {
        return createMaterial(materialName, materialMap, null);
    }

    private MaterialWrapper createMaterial(String materialName, ReadableMap materialMap, VideoTexture videoTexture) {
        MaterialWrapper materialWrapper = new MaterialWrapper(materialName, materialMap);

        // These defaults match those in the JNI's Material.java
        Material.LightingModel lightingModel = Material.LightingModel.CONSTANT;
        int diffuseColor = Color.WHITE;
        int chromaKeyFilteringColor = Color.GREEN;
        boolean chromaFilteringEnabled = false;
        Texture diffuseTexture = null;
        float diffuseIntensity = 1.0f;
        Texture specularTexture = null;
        float shininess = 2.0f;
        float fresnelExponent = 1.0f;
        Texture normalMap = null;
        Material.CullMode cullMode = Material.CullMode.BACK;
        Material.TransparencyMode transparencyMode= Material.TransparencyMode.A_ONE;
        Material.BlendMode blendMode = Material.BlendMode.ALPHA;
        EnumSet<Material.ColorWriteMask> colorWriteMask = EnumSet.of(Material.ColorWriteMask.ALL);
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
                String type = parseAssetType(materialMap, materialPropertyName);
                Texture.Format format = parseImageFormat(materialMap, materialPropertyName);
                boolean mipmap = parseImageMipmap(materialMap, materialPropertyName);
                boolean sRGB = !materialPropertyName.startsWith("normal");

                Uri uri = Helper.parseUri(path, mContext);
                if (path != null) {
                    if (isVideoTexture(path, type)) {
                        materialWrapper.addVideoTexturePath(materialPropertyName, uri);
                        diffuseTexture = videoTexture;
                    } else {
                        if (sImageMap.get(materialPropertyName) != null) {
                            Texture texture = parseTexture(sImageMap.get(materialPropertyName), sRGB, mipmap,
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
                                Texture texture = parseTexture(nativeImage, sRGB, mipmap,
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
                else if(materialPropertyName.equalsIgnoreCase("chromaKeyFilteringColor")) {
                    chromaKeyFilteringColor = color;
                    chromaFilteringEnabled = true;
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
                } else if ("colorWriteMask".equalsIgnoreCase(materialPropertyName)) {
                    ReadableArray colorWriteArray = materialMap.getArray(materialPropertyName);
                    List<Material.ColorWriteMask> colorWriteList = new ArrayList<Material.ColorWriteMask>();
                    for (int c = 0; c < colorWriteArray.size(); c++) {
                        colorWriteList.add(Material.ColorWriteMask.valueFromString(colorWriteArray.getString(c)));
                    }

                    colorWriteMask = EnumSet.noneOf(Material.ColorWriteMask.class);
                    colorWriteMask.addAll(colorWriteList);
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
                readsFromDepthBuffer, colorWriteMask);

                nativeMaterial.setName(materialName);
        if (chromaFilteringEnabled) {
            nativeMaterial.setChromaKeyFilteringEnabled(chromaFilteringEnabled);
            nativeMaterial.setChromaKeyFilteringColor(chromaKeyFilteringColor);
        }

        materialWrapper.setNativeMaterial(nativeMaterial);

        // Parse stuff
        parsePBRProperties(PBRProperties.METALNESS, nativeMaterial, materialMap);
        parsePBRProperties(PBRProperties.ROUGHNESS, nativeMaterial, materialMap);
        parsePBRProperties(PBRProperties.AMBIENT_OCCLUSION_TEXTURE, nativeMaterial, materialMap);

        // We don't need to hold a Java texture reference after assigning the texture to the material.
        // Make an exception for the videoTexture as we use the nativeref to play,pause, loop the video.
        if (diffuseTexture != null && videoTexture == null) {
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
                Texture texture = parseTexture(nativeImage, sRGB, false,
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

    private Texture parseTexture(Image image, boolean sRGB, boolean mipmap,
                                 String name, ReadableMap materialMap) {
        Texture nativeTexture = new Texture(image, sRGB, mipmap);
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
            if (sImageMap.get(key) != null) {
                cubeMapImages.put(key, sImageMap.get(key));
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
                              cubeMapImages.get("pz"), cubeMapImages.get("nz"));
    }

    private String parseAssetType(ReadableMap map, String key) {
        if ((map.getType(key) == ReadableType.Map) && map.getMap(key).hasKey("type")) {
            String type = map.getMap(key).getString("type");
            if (type != null && !type.equalsIgnoreCase("unknown")) {
                return type;
            }
        }
        return null;
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

    private boolean isVideoTexture(String path, String type) {
        if (path.contains(("mp4"))) {
            return path.contains("mp4");
        }

        if (type != null && type.contains("mp4")) {
            return true;
        }

        return false;
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

    public interface MaterialChangeListener {
        // invoked when the diffuse video texture changed of the given material.
        public void onVideoTextureChanged(String materialName);
    }

    /**
     * MaterialWrapper Class
     */
    public class MaterialWrapper {
        private String mMaterialName;
        private Material mNativeMaterial;
        // the source map that specified this material.
        private final ReadableMap mMaterialSource;
        private Map<String, Uri> mVideoTextures;

        public MaterialWrapper(String materialName, ReadableMap source) {
            mVideoTextures = new HashMap<String, Uri>();
            mMaterialSource = source;
            mMaterialName = materialName;
        }

        public void setNativeMaterial(Material material) { mNativeMaterial = material; }
        public Material getNativeMaterial() {
            return mNativeMaterial;
        }

        public void addVideoTexturePath(String name, Uri videoUri) {
            mVideoTextures.put(name, videoUri);
        }

        public boolean hasVideoTextures() {
            if(!mVideoTextures.isEmpty()) {
                return true;
            }
            return false;
        }

        /**
         * Get the URI for the diffuse video texture. Return null if this material is not
         * a video material.
         * @return URI of video texture if this a video material. Null otherwise.
         */
        public Uri getVideoTextureURI(){
            return mVideoTextures.get("diffuseTexture");
        }

        public void recreate(VideoTexture videoTexture) {
            if (mMaterialSource != null) {
                MaterialWrapper other = createMaterial(mMaterialName, mMaterialSource, videoTexture);
                mNativeMaterial.dispose();
                mNativeMaterial = other.mNativeMaterial;
                mVideoTextures = other.mVideoTextures;
                if (sMaterialChangeListeners.get(mMaterialName) != null &&
                        sMaterialChangeListeners.get(mMaterialName).get() != null) {
                    MaterialChangeListener listener = sMaterialChangeListeners.get(mMaterialName).get();
                    listener.onVideoTextureChanged(mMaterialName);
                }
            }
        }

        public void recreate() {
            if (mMaterialSource != null) {
                MaterialWrapper other = createMaterial(mMaterialName, mMaterialSource);
                mNativeMaterial.dispose();
                mNativeMaterial = other.mNativeMaterial;
                mVideoTextures = other.mVideoTextures;
            }
        }
    }
}
