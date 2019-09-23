//  Copyright © 2018 Viro Media. All rights reserved.
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

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.Material;
import com.viro.core.VideoTexture;
import com.viromedia.bridge.module.MaterialManager;
import com.viromedia.bridge.utility.ViroEvents;

import java.lang.ref.WeakReference;

public class VRTMaterialVideo extends VRTComponent implements MaterialManager.MaterialChangeListener{

    private boolean mPaused = false;
    private boolean mLoop = false;
    private boolean mMuted = false;
    private float mVolume = 1;
    private VideoTexture mVideoTexture = null;
    private VideoTexture.PlaybackListener mDelegate = null;
    private String mMaterial;
    private boolean mMaterialChanged = false;

    private static class VideoSurfaceDelegate implements VideoTexture.PlaybackListener {

        private WeakReference<VRTMaterialVideo> mMaterialVideo;

        public VideoSurfaceDelegate(VRTMaterialVideo materialVideo) {
            mMaterialVideo = new WeakReference<VRTMaterialVideo>(materialVideo);
        }

        @Override
        public void onVideoBufferStart(VideoTexture video) {
            VRTMaterialVideo materialVideo = mMaterialVideo.get();
            if (materialVideo == null || materialVideo.isTornDown()) {
                return;
            }
            materialVideo.playerBufferStart();
        }

        @Override
        public void onVideoBufferEnd(VideoTexture video) {
            VRTMaterialVideo materialVideo = mMaterialVideo.get();
            if (materialVideo == null || materialVideo.isTornDown()) {
                return;
            }
            materialVideo.playerBufferEnd();
        }

        @Override
        public void onVideoFinish(VideoTexture video) {
            VRTMaterialVideo materialVideo = mMaterialVideo.get();
            if (materialVideo == null || materialVideo.isTornDown()) {
                return;
            }
            materialVideo.playerDidFinishPlaying();
        }

        @Override
        public void onReady(VideoTexture video) {

        }

        @Override
        public void onVideoFailed(String error) {
            VRTMaterialVideo materialVideo = mMaterialVideo.get();
            if (materialVideo == null || materialVideo.isTornDown()) {
                return;
            }
            materialVideo.onError(error);
        }

        @Override
        public void onVideoUpdatedTime(VideoTexture video, float currentTime, float totalVideoTime) {
            VRTMaterialVideo materialVideo = mMaterialVideo.get();
            if (materialVideo == null || materialVideo.isTornDown()) {
                return;
            }
            materialVideo.playerOnUpdateTime(currentTime, totalVideoTime);
        }
    }

    public VRTMaterialVideo(ReactContext reactContext) {
        super(reactContext.getBaseContext(), null, -1, -1, reactContext);
    }

    public void onVideoTextureChanged(String materialName) {
        updateVideoTexture();
        //update props
        if (mVideoTexture != null) {
            mVideoTexture.setPlaybackListener(mDelegate);
            mVideoTexture.setLoop(mLoop);
            mVideoTexture.setMuted(mMuted);
            mVideoTexture.setVolume(mVolume);
            this.setPaused(mPaused);
        }
    }

    public void setPaused(boolean paused) {
        mPaused = paused;
        if (mVideoTexture == null) {
            return;
        }

        if (mPaused || !shouldAppear()) {
            mVideoTexture.pause();
        } else {
            mVideoTexture.play();
        }
    }

    public void setLoop(boolean loop) {
        mLoop = loop;
        if (mVideoTexture != null) {
            mVideoTexture.setLoop(loop);

            if (!mPaused) {
                mVideoTexture.play();
            }
        }
    }

    public void setMaterial(String material) {
        mMaterial = material;
        mMaterialChanged = true;
        MaterialManager materialManager = this.getReactContext().getNativeModule(MaterialManager.class);
        materialManager.setMaterialChangeListener(mMaterial, this);
        //pause previous video texture if it exists.
        if(mVideoTexture != null) {
            mVideoTexture.pause();
        }
    }

    @Override
    public void onHostPause(){
        super.onHostPause();
        if (mVideoTexture != null) {
            mVideoTexture.pause();
        }
    }

    @Override
    public void sceneWillDisappear() {
        if (mVideoTexture != null){
            mVideoTexture.pause();
        }
    }

    @Override
    public void onHostResume(){
        super.onHostResume();
        setPaused(mPaused);
    }

    private void updateVideoTexture() {
        mVideoTexture = null;
          // get material manager
        MaterialManager materialManager = this.getReactContext().getNativeModule(MaterialManager.class);
        Material nativeMaterial = materialManager.getMaterial(mMaterial);

        if (nativeMaterial.getDiffuseTexture() instanceof VideoTexture) {
            mDelegate = new VRTMaterialVideo.VideoSurfaceDelegate(this);
            mVideoTexture = ((VideoTexture) nativeMaterial.getDiffuseTexture());
            mVideoTexture.setPlaybackListener(mDelegate);
        }
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if (mMaterialChanged) {
            updateVideoTexture();
            mMaterialChanged = false;
        }
        if (mVideoTexture != null) {
            mVideoTexture.setPlaybackListener(mDelegate);
            mVideoTexture.setLoop(mLoop);
            mVideoTexture.setMuted(mMuted);
            mVideoTexture.setVolume(mVolume);
            this.setPaused(mPaused);
        }
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mVideoTexture != null) {
            mVideoTexture.dispose();
            mVideoTexture = null;
        }
    }

    public void setMuted(boolean muted) {
        mMuted = muted;
        if (mVideoTexture != null) {
            mVideoTexture.setMuted(muted);
        }
    }

    public void setVolume(float volume) {
        mVolume = volume;
        if (mVideoTexture != null) {
            mVideoTexture.setVolume(volume);
        }
    }

    public void seekToTime(float time) {
        if (mVideoTexture != null) {
            mVideoTexture.seekToTime(time);
            if (!mPaused) {
                mVideoTexture.play();
            }
        }
    }

    private void playerBufferStart() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_BUFFER_START,
                null);
    }

    private void playerBufferEnd() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_BUFFER_END,
                null);
    }

    private void playerDidFinishPlaying() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_FINISH,
                null);
    }

    private void playerOnUpdateTime(float currentTime, float totalTime) {
        WritableMap event = Arguments.createMap();
        event.putDouble("currentTime", (double) currentTime);
        event.putDouble("totalTime", (double) totalTime);

        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_UPDATE_TIME,
                event);
    }

}


