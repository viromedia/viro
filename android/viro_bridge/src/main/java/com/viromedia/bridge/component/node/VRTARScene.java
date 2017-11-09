/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.ViewParent;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.ARAnchor;
import com.viro.core.internal.ARDeclarativeNode;
import com.viro.core.internal.ARDeclarativePlane;

import com.viro.core.ARNode;
import com.viro.core.ARScene;
import com.viro.core.internal.Image;
import com.viro.core.Scene;
import com.viro.core.EventDelegate;

import com.viro.core.Surface;
import com.viro.core.Texture;
import com.viro.core.Vector;
import com.viro.core.ViroViewARCore;
import com.viromedia.bridge.component.VRTARSceneNavigator;
import com.viromedia.bridge.utility.ARUtils;
import com.viromedia.bridge.utility.ImageDownloadListener;
import com.viromedia.bridge.utility.ImageDownloader;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.ArrayList;
import java.util.EnumSet;

public class VRTARScene extends VRTScene implements ARScene.Listener {

    private static final String AMBIENT_LIGHT_INFO_KEY = "ambientLightInfo";
    private static final String INTENSITY_KEY = "intensity";
    private static final String COLOR_TEMPERATURE_KEY = "colorTemperature";
    private Surface mPointCloudSurface;
    private PointCloudImageDownloadListener mImageDownloadListener;
    private Handler mMainHandler;

    public VRTARScene(ReactApplicationContext reactContext) {
        super(reactContext);
        mMainHandler = new Handler(Looper.getMainLooper());
    }

    @Override
    protected Scene createSceneJni() {
        ARScene sceneControllerJni = new ARScene(true);
        sceneControllerJni.setListener(this);
        return sceneControllerJni;
    }

    public void setDisplayPointCloud(boolean displayPointCloud) {
        ((ARScene) mNativeScene).displayPointCloud(displayPointCloud);
    }

    public void setPointCloudImage(ReadableMap pointCloudImage) {
        // If there's another download still running, invalidate it.
        if (mImageDownloadListener != null) {
            mImageDownloadListener.invalidate();
        }

        if (pointCloudImage == null) {
            ((ARScene) mNativeScene).resetPointCloudSurface();

            if (mPointCloudSurface != null) {
                mPointCloudSurface.dispose();
                mPointCloudSurface = null;
            }

            return;
        }

        final ImageDownloader downloader = new ImageDownloader(getContext());
        downloader.setTextureFormat(Texture.TextureFormat.RGBA8);

        mImageDownloadListener = new PointCloudImageDownloadListener();
        downloader.getImageAsync(pointCloudImage, mImageDownloadListener);
    }

    public void setPointCloudScale(Vector pointCloudScale) {
        ((ARScene) mNativeScene).setPointCloudSurfaceScale(pointCloudScale);
    }

    public void setPointCloudMaxPoints(int maxPoints) {
        ((ARScene) mNativeScene).setPointCloudMaxPoints(maxPoints);
    }

    public void addARNode(ARDeclarativeNode node) {
        ((ARScene) mNativeScene).addARDeclarativeNode(node);
    }

    public void updateARNode(ARDeclarativeNode node) {
        ((ARScene) mNativeScene).updateARDeclarativeNode(node);
    }

    public void removeARNode(ARDeclarativeNode node) {
        ((ARScene) mNativeScene).removeARDeclarativeNode(node);
    }

    public void setAnchorDetectionTypes(ReadableArray types) {
        ViewParent parent = getParent();
        if (parent instanceof VRTARSceneNavigator) {
            VRTARSceneNavigator navigator = (VRTARSceneNavigator) parent;
            ViroViewARCore viroViewARCore = navigator.getARView();
            if (viroViewARCore != null) {
                EnumSet<ViroViewARCore.AnchorDetectionType> typesList = EnumSet.noneOf(ViroViewARCore.AnchorDetectionType.class);
                if (types != null) {
                    for (int i = 0; i < types.size(); i++) {
                        typesList.add(ViroViewARCore.AnchorDetectionType.valueFromString(types.getString(i)));
                    }
                }
                viroViewARCore.setAnchorDetectionTypes(typesList);
            }
        }
    }

    public void setCanCameraARHitTest(boolean canCameraHitTest){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_CAMERA_AR_HIT_TEST, canCameraHitTest);
    }


    // -- ARSceneDelegate Implementation --

    @Override
    public void onTrackingInitialized() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_TRACKING_INITIALIZED,
                null);
    }

    @Override
    public void onAmbientLightUpdate(float lightIntensity, float colorTemperature) {
        WritableMap lightInfoMap = Arguments.createMap();
        lightInfoMap.putDouble(INTENSITY_KEY, (double) lightIntensity);
        lightInfoMap.putDouble(COLOR_TEMPERATURE_KEY, (double) colorTemperature);

        WritableMap event = Arguments.createMap();
        event.putMap(AMBIENT_LIGHT_INFO_KEY, lightInfoMap);

        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_AMBIENT_LIGHT_UPDATE,
                event);
    }

    @Override
    public void onAnchorFound(ARAnchor arAnchor, ARNode node) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putMap("anchor", ARUtils.mapFromARAnchor(arAnchor));

        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
            getId(),
            ViroEvents.ON_ANCHOR_FOUND,
            returnMap);
    }

    @Override
    public void onAnchorUpdated(ARAnchor arAnchor, ARNode node) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putMap("anchor", ARUtils.mapFromARAnchor(arAnchor));

        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
            getId(),
            ViroEvents.ON_ANCHOR_UPDATED,
            returnMap);
    }

    @Override
    public void onAnchorRemoved(ARAnchor arAnchor, ARNode node) {
        WritableMap returnMap = Arguments.createMap();
        returnMap.putMap("anchor", ARUtils.mapFromARAnchor(arAnchor));

        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
            getId(),
            ViroEvents.ON_ANCHOR_REMOVED,
            returnMap);
    }

    private class PointCloudImageDownloadListener implements ImageDownloadListener {
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
                @Override
                public void run() {
                    if (!mIsValid) {
                        return;
                    }

                    if (mPointCloudSurface == null) {
                        mPointCloudSurface = new Surface(1, 1);
                    }

                    Image image = new Image(result, Texture.TextureFormat.RGBA8);
                    Texture texture = new Texture(image, Texture.TextureFormat.RGBA8, true, false, null);
                    mPointCloudSurface.setImageTexture(texture);

                    if (!isTornDown()) {
                        ((ARScene) mNativeScene).setPointCloudSurface(mPointCloudSurface);
                    }
                }
            });
        }

        @Override
        public void failed(String error) {
            throw new IllegalStateException("Unable to download point cloud image. Error: [" + error + "].");
        }
    }
}
