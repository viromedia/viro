package com.viromedia.bridge.component.node;

import android.view.View;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.core.Node;
import com.viro.core.Portal;
import com.viro.core.PortalScene;
import com.viromedia.bridge.utility.ViroEvents;

import java.lang.ref.WeakReference;
/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */

public class VRTPortalScene extends VRTNode  {
    private PortalScene mPortalScene;
    private static class PortalDelegate implements PortalScene.Delegate  {

        private WeakReference<VRTPortalScene> mPortalScene;

        public PortalDelegate(VRTPortalScene portalScene) {
            mPortalScene = new WeakReference<VRTPortalScene>(portalScene);
        }
        @Override
        public void onPortalEnter(PortalScene portal) {
            VRTPortalScene portalScene = mPortalScene.get();
            if (portalScene == null || portalScene.isTornDown()) {
                return;
            }
            portalScene.onPortalEnter();
        }

        @Override
        public void onPortalExit(PortalScene portal) {
            VRTPortalScene portalScene = mPortalScene.get();
            if (portalScene == null || portalScene.isTornDown()) {
                return;
            }
            portalScene.onPortalExit();
        }
    }

    protected Node createNodeJni() {
        mPortalScene = new PortalScene();
        mPortalScene.setDelegate(new PortalDelegate(this));
        return mPortalScene;
    }

    @Override
    public void addView(View child, int index) {
             if(child instanceof VRTPortal) {
                 VRTPortal portal = (VRTPortal)child;
                 mPortalScene.setPortalEntrance((Portal)portal.getNodeJni());
             }
        super.addView(child, index);
    }

    public void setPassable(boolean passable) {
        mPortalScene.setPassable(passable);
    }

    private void onPortalExit() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_PORTAL_EXIT,
                null);
    }

    private void onPortalEnter() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_PORTAL_ENTER,
                null);
    }

    public VRTPortalScene(ReactApplicationContext context) {
        super(context);
    }


}
