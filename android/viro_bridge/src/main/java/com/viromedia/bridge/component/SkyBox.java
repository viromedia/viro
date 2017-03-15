/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Looper;

import com.facebook.react.bridge.JSApplicationIllegalArgumentException;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;
import com.facebook.react.bridge.ReadableType;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.ImageJni;
import com.viro.renderer.jni.TextureFormat;
import com.viro.renderer.jni.TextureJni;
import com.viromedia.bridge.component.node.Scene;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;
import com.viromedia.bridge.utility.ViroLog;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.CountDownLatch;

public class SkyBox extends Component {
    private static final long COLOR_NOT_SET = 0;

    private static final String TAG = ViroLog.getTag(SkyBox.class);
    private final ReactApplicationContext mContext;
    private ReadableMap mSourceMap;
    private Map<String, ImageJni> mImageJniMap = new HashMap<>();
    private TextureJni mLatestTexture;
    private ImageDownloader mImageDownloader;
    private long mColor;
    private TextureFormat mFormat = TextureFormat.RGBA8;
    private boolean mSkyboxNeedsUpdate = false;
    private boolean mUseTextureForSkybox = true;

    public SkyBox(ReactApplicationContext context) {
        super(context);
        mContext = context;
        mImageDownloader = new ImageDownloader(getContext());
        mColor = COLOR_NOT_SET;
    }

    public void setSource(ReadableMap source) {
        mSourceMap = source;
        mUseTextureForSkybox = (source != null);
        mSkyboxNeedsUpdate = true;
    }

    public void setColor(long color) {
        mColor = color;
        mUseTextureForSkybox = (color != COLOR_NOT_SET);
        mSkyboxNeedsUpdate = true;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();

        if (mSkyboxNeedsUpdate) {
            if (mUseTextureForSkybox && mSourceMap != null) {
                imageDownloadDidStart();
                ReadableMapKeySetIterator iterator = mSourceMap.keySetIterator();
                // We'll use this latch to find out when all the 6 images for the skybox have downloaded
                CountDownLatch latch = new CountDownLatch(6);

                while (iterator.hasNextKey()) {
                    String key = iterator.nextKey();
                    ReadableType type = mSourceMap.getType(key);
                    if (type.name().equals(ReadableType.Map.name())) {
                        getImageForCubeFace(key, mSourceMap.getMap(key), latch);
                    }
                }
            } else if (mColor != COLOR_NOT_SET) {
                if (mScene != null) {
                    mScene.setBackgroundCubeWithColor(mColor);
                }
            }

            mSkyboxNeedsUpdate = false;
        }
    }


    private void getImageForCubeFace(final String cubeFaceName, ReadableMap map, final CountDownLatch latch) {

        if (map != null) {
            mImageDownloader.getImageAsync(map, new ImageDownloadListener() {
                @Override
                public void completed(Bitmap result) {
                    if (isTornDown()) {
                        // Prevent memory leak if tear down already happened on this component
                        return;
                    }

                    ImageJni cubeFaceImage = mImageJniMap.get(cubeFaceName);
                    if (cubeFaceImage != null) {
                        cubeFaceImage.destroy();
                    }

                    cubeFaceImage = new ImageJni(result, mFormat);
                    mImageJniMap.put(cubeFaceName, cubeFaceImage);
                    latch.countDown();

                    if (latch.getCount() == 0) {
                        // All 6 skybox images finished downloading.
                        imageDownloadDidFinish();

                        if(result != null) {
                            final int width = result.getWidth();
                            final int height = result.getHeight();

                            if(width != height) {
                              ViroLog.error(TAG, "Width and height for skybox textures must be square, current image dimensions are (" + width + "," + height + ")");
                            }
                        }
                    }
                }
            });
        }
    }

    @Override
    public void onTearDown() {
        if (!mImageJniMap.isEmpty()) {
            ReadableMapKeySetIterator iterator = mSourceMap.keySetIterator();
            while (iterator.hasNextKey()) {
                String key = iterator.nextKey();
                mImageJniMap.get(key).destroy();
                mImageJniMap.remove(key);
            }
        }

        if (mLatestTexture != null) {
            mLatestTexture.destroy();
            mLatestTexture = null;
        }
    }

    @Override
    public void setScene(Scene scene) {
        super.setScene(scene);
        if (mLatestTexture != null) {
            mScene.setBackgroundCubeImageTexture(mLatestTexture);
        } else if (mColor != COLOR_NOT_SET) {
            mScene.setBackgroundCubeWithColor(mColor);
        }
    }

    public void setFormat(String format) {
        mFormat = TextureFormat.forString(format);
        mImageDownloader.setTextureFormat(mFormat);
        mSkyboxNeedsUpdate = true;
    }

    private void imageDownloadDidStart() {
        mContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                SkyBoxManager.SKYBOX_LOAD_START,
                null
        );

    }

    private void imageDownloadDidFinish() {

        if (mLatestTexture != null) {
            mLatestTexture.destroy();
        }
        mLatestTexture = new TextureJni(mImageJniMap.get("px"), mImageJniMap.get("nx"),
                mImageJniMap.get("py"), mImageJniMap.get("ny"),
                mImageJniMap.get("pz"), mImageJniMap.get("nz"), mFormat);

        if (mScene != null && mUseTextureForSkybox) {
            mScene.setBackgroundCubeImageTexture(mLatestTexture);
        }
        mContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                SkyBoxManager.SKYBOX_LOAD_END,
                null
        );
    }
}
