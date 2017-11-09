/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Intent;
import android.os.Handler;
import android.os.Looper;
import android.support.v4.content.LocalBroadcastManager;
import android.view.View;
import android.widget.FrameLayout;

import com.facebook.react.bridge.LifecycleEventListener;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.RendererStartListener;
import com.viro.core.ViroContext;
import com.viro.core.ViroViewGVR;
import com.viro.core.ViroViewOVR;
import com.viro.core.ViroView;
import com.viromedia.bridge.ReactViroPackage;
import com.viromedia.bridge.component.node.VRTScene;
import com.viromedia.bridge.module.MaterialManager;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.module.PerfMonitor;
import com.viromedia.bridge.utility.ViroLog;

import java.lang.ref.WeakReference;
import java.util.ArrayList;

/**
 * SceneNavigator manages the various scenes that a Viro App can navigate between.
 */
public class VRTSceneNavigator extends FrameLayout {
    private static final String TAG = ViroLog.getTag(VRTSceneNavigator.class);

    private static final String DAYDREAM = "daydream";

    protected static class InnerGLListener implements RendererStartListener {

        private WeakReference<VRTSceneNavigator> mNavigator;

        public InnerGLListener(VRTSceneNavigator navigator) {
            mNavigator = new WeakReference<VRTSceneNavigator>(navigator);
        }

        @Override
        public void onRendererStart() {
            final VRTSceneNavigator navigator = mNavigator.get();
            if (navigator == null) {
                return;
            }

            navigator.mGLInitialized = true;
            (new Handler(Looper.getMainLooper())).post(new Runnable() {
                @Override
                public void run() {
                    navigator.mGLInitialized = true;
                    navigator.setViroContext();
                }
            });
        }
    }

    private static class SceneNavigatorLifecycleListener implements LifecycleEventListener {

        private WeakReference<VRTSceneNavigator> mNavigator;

        public SceneNavigatorLifecycleListener(VRTSceneNavigator navigator) {
            mNavigator = new WeakReference<VRTSceneNavigator>(navigator);
        }

        @Override
        public void onHostResume() {
            VRTSceneNavigator navigator = mNavigator.get();
            if (navigator == null) {
                return;
            }
            navigator.onHostResume();
        }

        @Override
        public void onHostPause() {
            VRTSceneNavigator navigator = mNavigator.get();
            if (navigator == null) {
                return;
            }

            navigator.onHostPause();
        }

        @Override
        public void onHostDestroy() {
            VRTSceneNavigator navigator = mNavigator.get();
            if (navigator == null) {
                return;
            }

            navigator.onHostDestroy();
        }
    }

    /**
     * View containing our renderer
     */
    protected ViroView mViroView;

    /**
     * Currently rendered scene
     */
    private int mSelectedSceneIndex = -1;

    /**
     * Array of scenes given by the bridge for the renderer to switch to.
     */
    private final ArrayList<VRTScene> mSceneArray = new ArrayList<VRTScene>();

    /**
     * The platform that the developer has requested.
     */
    private final ReactViroPackage.ViroPlatform mPlatform;

    /**
     * This SceneNavigator's LifecycleEventListener to register for React LifecycleEvents.
     * Must be deregistered onDestroy.
     */
    private final LifecycleEventListener mLifecycleListener;

    /**
     * Context passed around to views to get render specific information.
     */
    private ViroContext mViroContext;

    private final ReactApplicationContext mReactContext;

    private boolean mViewAdded = false;
    private boolean mGLInitialized = false;

    private boolean mHasOnExitViroCallback = false;

    public VRTSceneNavigator(ReactApplicationContext reactContext,
                             ReactViroPackage.ViroPlatform platform) {
        super(reactContext.getBaseContext(), null, -1);
        mPlatform = platform;
        mReactContext = reactContext;

        // Create the ViroView
        mViroView = createViroView(reactContext);

        // Add the ViroView as a child so it's rendered.
        addView((View) mViroView);

        mViroContext = mViroView.getViroContext();

        /*
         * Set the view for the debug console.
         */
        PerfMonitor perfMonitor = reactContext.getNativeModule(PerfMonitor.class);
        perfMonitor.setView(mViroView);

        /*
         * Trigger ViroView's onActivityStarted and onActivityResumed of the vrView as
         * React creates it's views within the activity's onResume().
         */
        mViroView.onActivityStarted(reactContext.getCurrentActivity());

        notifyScenePlatformInformation();

        mLifecycleListener = new SceneNavigatorLifecycleListener(this);
        reactContext.addLifecycleEventListener(mLifecycleListener);

        /**
         * We may need to reload the materials if the renderer was destroyed, but the app was not
         * and the user resumed.
         */
        MaterialManager materialManager = reactContext.getNativeModule(MaterialManager.class);
        materialManager.reloadMaterials();
    }

    protected ViroView createViroView(ReactApplicationContext reactContext) {
        switch (mPlatform) {
            case OVR_MOBILE:
                return new ViroViewOVR(reactContext.getCurrentActivity(),
                        new InnerGLListener(this));
            case GVR:
                // default case is to use GVR
            default:
                return new ViroViewGVR(reactContext.getCurrentActivity(),
                        new InnerGLListener(this), new OnGVRExitListener(this));
        }
    }

    @Override
    public void addView(View child, int index) {
        if (child instanceof ViroView) {
            // only add a view to the childViews if it's a ViroView. This function is called
            // by the single argument addView(child) method.
            super.addView(child, index);
            return;
        } else if (!(child instanceof VRTScene)) {
            throw new IllegalArgumentException("Attempted to add a non-scene element [" + child.getClass().getSimpleName()
                    + "] to SceneNavigator!");
        }

        VRTScene childScene = (VRTScene)child;
        mSceneArray.add(index, childScene);
        childScene.setPlatformInformation(mViroView.getPlatform(), mViroView.getHeadset(),
                mViroView.getController());
        childScene.addPortalTraversalListener(mViroView.getRenderer());
        // Adding the scene view can occur after the prop type is set on the bridge.
        // Thus, refresh the selection of the current scene as needed.
        if (index == mSelectedSceneIndex){
            setCurrentSceneIndex(mSelectedSceneIndex);
        }

        mViewAdded = true;
        // In case gl was initialized before views were added.
        setViroContext();
        super.addView(child, index);
    }

    private void setViroContext() {
        if (mViewAdded && mGLInitialized && mSelectedSceneIndex < mSceneArray.size()) {
            VRTScene childScene = mSceneArray.get(mSelectedSceneIndex);
            childScene.setViroContext(mViroContext);
            childScene.setScene(childScene);
            childScene.setNativeRenderer(mViroView.getRenderer());
        }
    }

    public void setCurrentSceneIndex(int index) {
        mSelectedSceneIndex = index;
        if (index < 0 || index >= mSceneArray.size()){
            // Scene object may not yet have been initialized, so return here.
            return;
        }

        mViroView.setScene(mSceneArray.get(mSelectedSceneIndex).getNativeScene());
        mSceneArray.get(mSelectedSceneIndex).parentDidAppear();
    }

    @Override
    public void removeViewAt(int index) {
        View view = getChildAt(index);

        /*
         When removing a scene, force an immediate tear-down of all the
         views of the scene. This deletes their persistent-refs, but also
         prevents the scene's node rendering tree from being deconstructed by
         further removeViewAt calls (the various removeViewAt calls check
         if a component is torn down before modifying the rendering tree).

         We want to prevent the deconstruction of the rendering tree so that
         the scene can appropriately animate out. The scene will still be
         cleaned up (memory-wise), since it still gets cut off from the react
         view-tree from the removeViewAt calls, isolating the rendering tree
         from the bridge, and cutting the VROScene from the scene controller.
         */
        if (view instanceof VRTScene) {
            ((VRTScene) view).removePortalTraversalListener(mViroView.getRenderer());
            mSceneArray.remove(view);
            ((VRTScene) view).forceCascadeTearDown();
        }
        super.removeViewAt(index);

        if ((view instanceof VRTComponent)) {
            VRTComponent component = (VRTComponent) view;
            component.onTreeUpdate();
        }
    }

    public void setVrModeEnabled(boolean vrModeEnabled) {
        mViroView.setVRModeEnabled(vrModeEnabled);
    }

    public void setHasOnExitViroCallback(boolean hasCallback) {
        mHasOnExitViroCallback = hasCallback;
    }

    public void setDebug(boolean debug) {
        mViroView.setDebug(debug);
    }

    public void setApiKey(String apiKey) {

        if (apiKey == null || apiKey.trim().isEmpty()) {
            throw new IllegalArgumentException("The given API Key is either missing or invalid!" +
                    " If you have not signed up for accessing Viro Media platform, please do so " +
                    "at www.viromedia.com.");
        }

        mViroView.validateAPIKey(apiKey.trim());
    }

    private void notifyScenePlatformInformation() {
        for (VRTScene scene: mSceneArray) {
            scene.setPlatformInformation(mViroView.getPlatform(), mViroView.getHeadset(),
                    mViroView.getController());
        }
    }

    public void onPropsSet() {
        // no-op
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();

        for (VRTScene scene : mSceneArray) {
            scene.forceCascadeTearDown();
        }

        /**
         * If we're exiting Viro and destroying the renderer, notify the MaterialManager so that if
         * the application doesn't get killed, then the next time Viro starts, we know to reload the
         * materials.
         */
        MaterialManager materialManager = mReactContext.getNativeModule(MaterialManager.class);
        materialManager.shouldReload();

        mViroView.onActivityStopped(mReactContext.getCurrentActivity());
        mViroView.dispose();
    }

    private void onHostResume() {
        if (mViewAdded && mGLInitialized) {
            VRTScene childScene = mSceneArray.get(mSelectedSceneIndex);
            childScene.onHostResume();
        }

        if (mViroView != null){
            mViroView.onActivityResumed(mReactContext.getCurrentActivity());
        }
    }

    private void onHostPause() {
        if (mViewAdded && mGLInitialized) {
            VRTScene childScene = mSceneArray.get(mSelectedSceneIndex);
            childScene.onHostPause();
        }

        if (mViroView != null){
            mViroView.onActivityPaused(mReactContext.getCurrentActivity());
        }
    }

    private void onHostDestroy() {
        mReactContext.removeLifecycleEventListener(mLifecycleListener);
    }

    public void userDidRequestExitVR(){
        if (!mHasOnExitViroCallback){
            mReactContext.getCurrentActivity().finish();
            return;
        }

        // Notify javascript listeners (for ReactNativeJs to ViroReactJs cases)
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_EXIT_VIRO,
                null);

        // Notify Native listeners (for NativeApp to ViroReactJs cases)
        Intent intent = new Intent();
        intent.setAction(ReactViroPackage.ON_EXIT_VIRO_BROADCAST);
        LocalBroadcastManager.getInstance(mReactContext.getApplicationContext()).sendBroadcast(intent);
    }

    public void recenterTracking() {
        if (mViroView.getHeadset().equalsIgnoreCase(DAYDREAM)) {
            throw new IllegalStateException("recenterTracking should not be invoked on Daydream devices.");
        }
        mViroView.recenterTracking();
    }

    private static class OnGVRExitListener implements Runnable{
        private WeakReference<VRTSceneNavigator> mSceneNavWeak;
        public OnGVRExitListener(VRTSceneNavigator sceneNav) {
            mSceneNavWeak = new WeakReference<VRTSceneNavigator>(sceneNav);
        }

        @Override
        public void run() {
            VRTSceneNavigator view = mSceneNavWeak.get();
            if (view == null) {
                return;
            }

            view.userDidRequestExitVR();
        }
    }
}
