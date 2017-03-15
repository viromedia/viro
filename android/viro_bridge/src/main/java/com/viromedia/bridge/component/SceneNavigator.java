/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.os.Handler;
import android.os.Looper;
import android.view.View;
import android.widget.FrameLayout;

import com.facebook.react.bridge.LifecycleEventListener;
import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.GlListener;
import com.viro.renderer.jni.RenderContextJni;
import com.viro.renderer.jni.ViroGvrLayout;
import com.viro.renderer.jni.ViroOvrView;
import com.viro.renderer.jni.VrView;
import com.viromedia.bridge.ReactViroPackage;
import com.viromedia.bridge.component.node.Scene;
import com.viromedia.bridge.utility.ViroLog;

import java.util.ArrayList;

/**
 * SceneNavigator manages the various scenes that a Viro App can navigate between.
 */
public class SceneNavigator extends FrameLayout {
    private static final String TAG = ViroLog.getTag(SceneNavigator.class);

    /**
     * View containing our renderer
     */
    private VrView mVrView;

    /**
     * Currently rendered scene
     */
    private int mSelectedSceneIndex = -1;

    /**
     * Array of scenes given by the bridge for the renderer to switch to.
     */
    private final ArrayList<Scene> mSceneArray = new ArrayList<Scene>();

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
    private RenderContextJni mRenderContext;

    private final ReactApplicationContext mReactContext;

    private boolean mViewAdded = false;
    private boolean mGLInialized = false;

    private boolean mVrModeEnabled = true;
    private boolean mHasSetVrMode = false;

    private String mApiKey;

    public SceneNavigator(ReactApplicationContext reactContext,
                          ReactViroPackage.ViroPlatform platform) {
        super(reactContext.getBaseContext(), null, -1);
        mPlatform = platform;
        mReactContext = reactContext;

        switch (mPlatform) {
            case OVR_MOBILE:
                mVrView = new ViroOvrView(reactContext.getCurrentActivity(),
                        new InnerGlListener());
                break;
            case GVR:
                // default case is to use GVR
            default:
                ViroGvrLayout gvrLayout = new ViroGvrLayout(reactContext.getCurrentActivity(),
                        new InnerGlListener());
                mVrView = gvrLayout;
        }

        View baseVrView = (View)mVrView;
        addView(baseVrView);

        mRenderContext = mVrView.getRenderContextRef();

        /*
         * Trigger VrView's onActivityStarted and onActivityResumed of the vrView as
         * React creates it's views within the activity's onResume().
         */
        mVrView.onActivityStarted(reactContext.getCurrentActivity());
        mVrView.onActivityResumed(reactContext.getCurrentActivity());

        notifyScenePlatformInformation();

        mLifecycleListener = new LifecycleEventListener() {
            @Override
            public void onHostResume() {
                if (mViewAdded && mGLInialized) {
                    Scene childScene = mSceneArray.get(mSelectedSceneIndex);
                    childScene.onHostResume();
                }
            }

            @Override
            public void onHostPause() {
                if (mViewAdded && mGLInialized) {
                    Scene childScene = mSceneArray.get(mSelectedSceneIndex);
                    childScene.onHostPause();
                }
            }

            @Override
            public void onHostDestroy() {
                mReactContext.removeLifecycleEventListener(mLifecycleListener);
            }
        };

        reactContext.addLifecycleEventListener(mLifecycleListener);
    }

    @Override
    public void addView(View child, int index) {
        if (child instanceof VrView) {
            // only add a view to the childViews if it's a VrView. This function is called
            // by the single argument addView(child) method.
            super.addView(child, index);
            return;
        } else if (!(child instanceof Scene)) {
            throw new IllegalArgumentException("Attempted to add a non-scene element [" + child.getClass().getSimpleName()
                    + "] to SceneNavigator!");
        }

        Scene childScene = (Scene)child;
        mSceneArray.add(index, childScene);
        childScene.setPlatformInformation(mVrView.getVRPlatform(), mVrView.getHeadset(),
                mVrView.getController());

        // Adding the scene view can occur after the prop type is set on the bridge.
        // Thus, refresh the selection of the current scene as needed.
        if (index == mSelectedSceneIndex){
            setCurrentSceneIndex(mSelectedSceneIndex);
        }

        mViewAdded = true;
        // In case gl was initialized before views were added.
        setRenderContext();
        super.addView(child, index);
    }

    private void setRenderContext() {
        if (mViewAdded && mGLInialized && mSelectedSceneIndex < mSceneArray.size()) {
            Scene childScene = mSceneArray.get(mSelectedSceneIndex);
            childScene.setRenderContext(mRenderContext);
            childScene.setScene(childScene);
            childScene.setNativeRenderer(mVrView.getNativeRenderer());
        }
    }

    public void setCurrentSceneIndex(int index){
        mSelectedSceneIndex = index;
        if (index < 0 || index >= mSceneArray.size()){
            // Scene object may not yet have been initialized, so return here.
            return;
        }

        mVrView.setScene(mSceneArray.get(mSelectedSceneIndex).getNativeScene());
    }

    @Override
    public void removeViewAt(int index) {
        View view = getChildAt(index);
        if (view instanceof Scene) {
            mSceneArray.remove(view);
        }
        super.removeViewAt(index);

        if ((view instanceof Component)) {
            Component component = (Component) view;
            component.onTreeUpdate();
        }
    }

    public void setVrModeEnabled(boolean vrModeEnabled) {
        mVrModeEnabled = vrModeEnabled;
    }

    public void setApiKey(String apiKey) {

        if (apiKey == null || apiKey.trim().isEmpty()) {
            throw new IllegalArgumentException("The given API Key is either missing or invalid!" +
                    " If you have not signed up for accessing Viro Media platform, please do so " +
                    "at www.viromedia.com.");
        }

        mVrView.validateApiKey(apiKey.trim());
    }

    private class InnerGlListener implements GlListener {
        @Override
        public void onGlInitialized() {
            mGLInialized = true;
            (new Handler(Looper.getMainLooper())).post(new Runnable() {
                @Override
                public void run() {
                    mGLInialized = true;
                    setRenderContext();
                }
            });
        }
    }

    private void notifyScenePlatformInformation() {
        for (Scene scene: mSceneArray) {
            scene.setPlatformInformation(mVrView.getVRPlatform(), mVrView.getHeadset(),
                    mVrView.getController());
        }
    }

    public void onPropsSet() {
        if (!mHasSetVrMode) {
            mHasSetVrMode = true;
            mVrView.setVrModeEnabled(mVrModeEnabled);
        }
    }
}
