/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.ControllerJni;
import com.viro.renderer.jni.EventDelegateJni;
import com.viro.renderer.jni.RenderContextJni;
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
    private ControllerJni mNativeController = null;
    private boolean mIsReticleVisible = DEFAULT_RETICLE_VISIBILITY;
    private boolean mIsControllerVisible = DEFAULT_CONTROLLER_VISIBILITY;
    private EventDelegateJni mEventDelegateJni;

    public VRTController(ReactApplicationContext reactContext) {
        super(reactContext);
        mEventDelegateJni = new EventDelegateJni();
    }
    @Override
    public void onTearDown(){
        if (mEventDelegateJni != null) {
            mEventDelegateJni.destroy();
            mEventDelegateJni = null;
        }
        super.onTearDown();
    }

    @Override
    public void setRenderContext(RenderContextJni context) {
        super.setRenderContext(context);
        mNativeController = new ControllerJni(context);
        updateVisibility();

        // Create and attach callbacks.
        mEventDelegateJni.setEventDelegateCallback(new ComponentEventDelegate(this));
        mNativeController.setEventDelegate(mEventDelegateJni);
    }

    public void setReticleVisibility(boolean reticleVisibility) {
        mIsReticleVisible = reticleVisibility;
    }

    public void setControllerVisibility(boolean controllerVisibility) {
        mIsControllerVisible = controllerVisibility;
    }

    protected void setCanClick(boolean canClick){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_CLICK, canClick);
    }

    protected void setCanTouch(boolean canTouch){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_TOUCH, canTouch);
    }

    protected void setCanSwipe(boolean canSwipe){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_SWIPE, canSwipe);
    }

    protected void setCanScroll(boolean canScroll){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_SCROLL, canScroll);
    }

    protected void setCanFuse(boolean canFuse){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_FUSE, canFuse);
    }

    protected void setCanGetControllerStatus(boolean canGetControllerStatus){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_CONTROLLER_STATUS, canGetControllerStatus);
    }

    public void getForwardVectorAsync(ControllerJni.ControllerJniCallback callback){
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

        mNativeController.setControllerVisibility(mIsControllerVisible);
        mNativeController.setReticleVisibility(mIsReticleVisible);
    }

}
