//  Copyright © 2017 Viro Media. All rights reserved.
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

import android.app.Application;

import com.facebook.react.ReactApplication;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.devsupport.interfaces.DevOptionHandler;
import com.facebook.react.devsupport.interfaces.DevSupportManager;
import com.facebook.react.module.annotations.ReactModule;
import com.viro.core.ViroView;

import java.lang.ref.WeakReference;

@ReactModule(name = "VRTPerfMonitor")
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
