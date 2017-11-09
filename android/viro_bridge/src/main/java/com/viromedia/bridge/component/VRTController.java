/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.util.Log;

import com.facebook.react.bridge.ReactApplicationContext;
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

    public VRTController(ReactApplicationContext reactContext) {
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
        mNativeController = new Controller(context);
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
