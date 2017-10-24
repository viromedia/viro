/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.module;

import android.app.Application;

import com.facebook.react.ReactApplication;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.devsupport.interfaces.DevOptionHandler;
import com.facebook.react.devsupport.interfaces.DevSupportManager;
import com.viro.renderer.jni.ViroView;

import java.lang.ref.WeakReference;

public class PerfMonitor extends ReactContextBaseJavaModule {

    private static class PerfOptionHandler implements DevOptionHandler {

        private WeakReference<PerfMonitor> mPerfMonitor;

        public PerfOptionHandler(PerfMonitor perfMonitor) {
            mPerfMonitor = new WeakReference<PerfMonitor>(perfMonitor);
        }

        @Override
        public void onOptionSelected() {
            PerfMonitor perfMonitor = mPerfMonitor.get();
            if (perfMonitor != null) {
                perfMonitor.onOptionSelected();
            }
        }
    }

    private boolean mIsShowing = false;

    private boolean mIsInitialized = false;

    private WeakReference<ViroView> mView;

    public PerfMonitor(ReactApplicationContext context) {
        super(context);
    }

    public void onOptionSelected() {
        ViroView view = mView.get();
        if (view != null) {
            view.setDebugHUDEnabled(!mIsShowing);
        }
        mIsShowing = !mIsShowing;
    }

    public void setView(ViroView view) {
        if (!mIsInitialized) {
            Application application = getCurrentActivity().getApplication();
            if (application instanceof ReactApplication) {
                ReactApplication reactApplication = (ReactApplication) application;
                DevSupportManager devSupport = reactApplication.getReactNativeHost().getReactInstanceManager().getDevSupportManager();
                devSupport.addCustomDevOption("[Viro] Toggle FPS Display", new PerfOptionHandler(this));

                mIsInitialized = true;
            }
        }

        mView = new WeakReference<ViroView>(view);
        view.setDebugHUDEnabled(mIsShowing);
    }

    @Override
    public String getName() {
        return "VRTPerfMonitor";
    }

}
