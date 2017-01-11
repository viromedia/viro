/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;


import android.graphics.Bitmap;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.ImageJni;
import com.viro.renderer.jni.TextureJni;
import com.viromedia.bridge.component.node.Scene;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;
import com.viromedia.bridge.utility.ViroEvents;

public class Image360 extends Component {
    private static final float[] sDefaultRotation = {0, 0, 0};

    private ReadableMap mSourceMap;
    private float[] mRotation;
    private ImageJni mLatestImage;
    private TextureJni mLatestTexture;

    public Image360(ReactApplicationContext context) {
        super(context);
    }

    public void setSource(ReadableMap source) {
        mSourceMap = source;
    }

    public void setRotation(ReadableArray rotation) {
        if (rotation == null) {
            mRotation = sDefaultRotation;
        } else {
            float[] rotationArr = {(float) rotation.getDouble(0),
                    (float) rotation.getDouble(1), (float) rotation.getDouble(2)};
            mRotation = rotationArr;
        }
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        ImageDownloader downloader = new ImageDownloader(getContext());

        if (mSourceMap != null) {
            imageDownloadDidStart();

            downloader.getImageAsync(mSourceMap, new ImageDownloadListener() {
                @Override
                public void completed(Bitmap result) {
                    if (mLatestImage != null) {
                        mLatestImage.destroy();
                    }

                    if (mLatestTexture != null) {
                        mLatestTexture.destroy();
                    }

                    mLatestImage = new ImageJni(result);
                    mLatestTexture = new TextureJni(mLatestImage);

                    if (mScene != null) {
                        mScene.setBackgroundImageTexture(mLatestTexture);
                        mScene.setBackgroundRotation(mRotation);
                    }
                    imageDownloadDidFinish();
                }
            });
        }
    }

    @Override
    public void onTearDown() {
        if (mLatestImage != null) {
            mLatestImage.destroy();
            mLatestImage = null;
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
            mScene.setBackgroundImageTexture(mLatestTexture);
            mScene.setBackgroundRotation(mRotation);
        }
    }

    private void imageDownloadDidStart() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_LOAD_START,
                null
        );
    }

    private void imageDownloadDidFinish() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_LOAD_END,
                null
        );
    }
}
