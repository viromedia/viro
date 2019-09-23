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

package com.viromedia.bridge.component;

import android.util.Log;

import com.facebook.react.bridge.ReactContext;
import com.viro.core.Controller;
import com.viro.core.EventDelegate;
import com.viro.core.ViroContext;
import com.viromedia.bridge.utility.ComponentEventDelegate;

public class VRTController extends VRTComponent {
    // React defaults
    protected final static boolean DEFAULT_CAN_CLICK = false;
    protected final static boolean DEFAULT_CAN_TOUCH = false;
    protected final static boolean DEFAULT_CAN_SCROLL = false;
    protected final static boolean DEFAULT_CAN_SWIPE = false;
    protected final static boolean DEFAULT_CAN_FUSE = false;
    protected final static boolean DEFAULT_CAN_GET_CONTROLLER_STATUS = false;
    protected final static boolean DEFAULT_RETICLE_VISIBILITY = true;
    protected final static boolean DEFAULT_CONTROLLER_VISIBILITY = true;
    private Controller mNativeController = null;
    private boolean mIsReticleVisible = DEFAULT_RETICLE_VISIBILITY;
    private boolean mIsControllerVisible = DEFAULT_CONTROLLER_VISIBILITY;
    private EventDelegate mEventDelegateJni;
    private ComponentEventDelegate mComponentEventDelegate;

    public VRTController(ReactContext reactContext) {
        super(reactContext);
        mEventDelegateJni = new EventDelegate();
    }
    @Override
    public void onTearDown(){
        if (mEventDelegateJni != null) {
            mEventDelegateJni.dispose();
            mEventDelegateJni = null;
        }
        super.onTearDown();
    }

    @Override
    public void setViroContext(ViroContext context) {
        super.setViroContext(context);
        mNativeController = context.getController();
        updateVisibility();

        // Create and attach callbacks.
        mComponentEventDelegate = new ComponentEventDelegate(this);
        mEventDelegateJni.setEventDelegateCallback(mComponentEventDelegate);
        mNativeController.setEventDelegate(mEventDelegateJni);
    }

    public void setReticleVisibility(boolean reticleVisibility) {
        mIsReticleVisible = reticleVisibility;
    }

    public void setControllerVisibility(boolean controllerVisibility) {
        mIsControllerVisible = controllerVisibility;
    }

    protected void setCanClick(boolean canClick){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_CLICK, canClick);
    }

    protected void setCanTouch(boolean canTouch){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_TOUCH, canTouch);
    }

    protected void setCanSwipe(boolean canSwipe){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_SWIPE, canSwipe);
    }

    protected void setCanScroll(boolean canScroll){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_SCROLL, canScroll);
    }

    protected void setCanFuse(boolean canFuse){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_FUSE, canFuse);
    }

    protected void setCanGetControllerStatus(boolean canGetControllerStatus){
        mEventDelegateJni.setEventEnabled(EventDelegate.EventAction.ON_CONTROLLER_STATUS, canGetControllerStatus);
    }

    public void getForwardVectorAsync(Controller.ControllerJniCallback callback){
        mNativeController.getControllerForwardVectorAsync(callback);
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        updateVisibility();
    }

    private void updateVisibility(){
        if (mNativeController == null){
            return;
        }
        mNativeController.setControllerVisible(mIsControllerVisible);
        mNativeController.setReticleVisible(mIsReticleVisible);
    }

}
