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

package com.viromedia.bridge.component;

import android.graphics.Bitmap;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;
import com.facebook.react.bridge.ReadableType;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.Node;
import com.viro.core.PortalScene;
import com.viro.core.internal.Image;
import com.viro.core.Texture;
import com.viromedia.bridge.component.node.VRTNode;
import com.viromedia.bridge.component.node.VRTScene;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;
import com.viromedia.bridge.utility.ViroLog;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.concurrent.CountDownLatch;

public class VRTSkyBox extends VRTNode {
    private static final long COLOR_NOT_SET = 0;

    private static final String TAG = ViroLog.getTag(VRTSkyBox.class);
    private final ReactContext mContext;
    private ReadableMap mSourceMap;
    private Map<String, Image> mImageMap = new HashMap<>();
    private Texture mLatestTexture;
    private ImageDownloader mImageDownloader;
    private long mColor;
    private Texture.Format mFormat = Texture.Format.RGBA8;
    private boolean mSkyboxNeedsUpdate = false;
    private boolean mUseTextureForSkybox = true;
    private HashSet<SkyboxImageDownloadListener> mDownloadListeners = new HashSet<>();

    public VRTSkyBox(ReactContext context) {
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
                setBackgroundCubeImageColor(mColor);
            }

            mSkyboxNeedsUpdate = false;
        }
    }


    private void getImageForCubeFace(final String cubeFaceName, ReadableMap map, final CountDownLatch latch) {
        if (map != null) {
            SkyboxImageDownloadListener listener = new SkyboxImageDownloadListener(cubeFaceName, latch);
            mDownloadListeners.add(listener);
            mImageDownloader.getImageAsync(map, listener);
        }
    }

    @Override
    public void onTearDown() {
        super.onTearDown();

        for (SkyboxImageDownloadListener listener : mDownloadListeners) {
            listener.invalidate();
        }

        if (!mImageMap.isEmpty() && mSourceMap != null) {
            ReadableMapKeySetIterator iterator = mSourceMap.keySetIterator();
            while (iterator.hasNextKey()) {
                String key = iterator.nextKey();
                mImageMap.get(key).destroy();
                mImageMap.remove(key);
            }
        }

        if (mLatestTexture != null) {
            mLatestTexture.dispose();
            mLatestTexture = null;
        }
    }

    @Override
    public void setScene(VRTScene scene) {
        super.setScene(scene);
        if (mLatestTexture != null) {
            setBackgroundCubeImageTexture(mLatestTexture);
        } else if (mColor != COLOR_NOT_SET) {
            setBackgroundCubeImageColor(mColor);
        }
    }

    public void setFormat(String format) {
        mFormat = Texture.Format.forString(format);
        mImageDownloader.setTextureFormat(mFormat);
        mSkyboxNeedsUpdate = true;
    }

    private void imageDownloadDidStart() {
        mContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                VRTSkyBoxManager.SKYBOX_LOAD_START,
                null
        );

    }

    private void setBackgroundCubeImageColor(long color){
        Node node = getNodeJni();
        if (node == null || isTornDown()) {
            return;
        }

        PortalScene portal = node.getParentPortalScene();
        if (portal != null) {
            portal.setBackgroundCubeWithColor(color);
            portal.setBackgroundRotation(Helper.toRadiansVector(mRotation));
        }
    }


    private void setBackgroundCubeImageTexture(Texture texture){
        Node node = getNodeJni();
        if (node == null || isTornDown()) {
            return;
        }

        PortalScene portal = node.getParentPortalScene();
        if (portal != null) {
            portal.setBackgroundCubeTexture(texture);
            portal.setBackgroundRotation(Helper.toRadiansVector(mRotation));
        }
    }

    private void imageDownloadDidFinish(boolean success) {
        if (mLatestTexture != null) {
            mLatestTexture.dispose();
        }

        if (success){
            mLatestTexture = new Texture(mImageMap.get("px"), mImageMap.get("nx"),
                    mImageMap.get("py"), mImageMap.get("ny"),
                    mImageMap.get("pz"), mImageMap.get("nz"));

            if (mUseTextureForSkybox) {
                setBackgroundCubeImageTexture(mLatestTexture);
            }
        }

        WritableMap event = Arguments.createMap();
        event.putBoolean("success", success);
        mContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                VRTSkyBoxManager.SKYBOX_LOAD_END,
                event
        );
    }

    private class SkyboxImageDownloadListener implements ImageDownloadListener {
        private boolean mIsValid = true;
        private CountDownLatch mLatch;
        private final String mCubeFaceName;

        public SkyboxImageDownloadListener(String cubeFaceName, CountDownLatch latch) {
            mCubeFaceName = cubeFaceName;
            mLatch = latch;
        }

        public void invalidate() {
            mIsValid = false;
            mLatch = null;
        }

        @Override
        public boolean isValid() {
            return mIsValid;
        }

        @Override
        public void completed(Bitmap result) {
            if (!mIsValid) {
                return;
            }

            Image cubeFaceImage = mImageMap.get(mCubeFaceName);
            if (cubeFaceImage != null) {
                cubeFaceImage.destroy();
            }

            cubeFaceImage = new Image(result, mFormat);
            mImageMap.put(mCubeFaceName, cubeFaceImage);
            mLatch.countDown();

            if (mLatch.getCount() == 0) {
                // All 6 skybox images finished downloading.
                imageDownloadDidFinish(true);

                if(result != null) {
                    final int width = result.getWidth();
                    final int height = result.getHeight();

                    if(width != height) {
                        ViroLog.error(TAG, "Width and height for skybox textures must be square, current image dimensions are (" + width + "," + height + ")");
                    }
                }
            }

            mDownloadListeners.remove(this);
        }

        @Override
        public void failed(String error) {
            imageDownloadDidFinish(false);
        }
    }
}
