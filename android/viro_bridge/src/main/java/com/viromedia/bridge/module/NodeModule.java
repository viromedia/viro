/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.module;

import android.view.View;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.uimanager.IllegalViewOperationException;
import com.facebook.react.uimanager.NativeViewHierarchyManager;
import com.facebook.react.uimanager.UIBlock;
import com.facebook.react.uimanager.UIManagerModule;
import com.viromedia.bridge.component.node.Node;

public class NodeModule extends ReactContextBaseJavaModule {

    public NodeModule(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTNodeModule";
    }

    @ReactMethod
    public void applyImpulse(final int viewTag, final ReadableArray force, final ReadableArray position) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View viroView = nativeViewHierarchyManager.resolveView(viewTag);
                if (!(viroView instanceof Node)){
                    throw new IllegalViewOperationException("Invalid view returned when applying force: expected a node-type control!");
                }

                if (force == null || force.size() != 3){
                    throw new IllegalViewOperationException("Invalid impulse force parameters provided!");
                }

                if (position != null && position.size() != 3){
                    throw new IllegalViewOperationException("Invalid impulse force position parameters provided!");
                }

                // If no offset is given, default to a central impulse.
                float[] forcePosition = {0,0,0};
                if (position != null){
                    forcePosition[0] = (float)position.getDouble(0);
                    forcePosition[1] = (float)position.getDouble(1);
                    forcePosition[2] = (float)position.getDouble(2);
                }
                float[] forceArray = { (float)force.getDouble(0), (float)force.getDouble(1), (float)force.getDouble(2)};

                Node nodeControl = (Node) viroView;
                nodeControl.applyImpulse(forceArray, forcePosition);
            }
        });
    }

    @ReactMethod
    public void applyTorqueImpulse(final int viewTag, final ReadableArray torque) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View viroView = nativeViewHierarchyManager.resolveView(viewTag);
                if (!(viroView instanceof Node)){
                    throw new IllegalViewOperationException("Invalid view returned when applying force: expected a node-type control!");
                }

                if (torque == null || torque.size() != 3){
                    throw new IllegalViewOperationException("Invalid impulse torque params provided!");
                }

                float[] torqueArray = { (float)torque.getDouble(0), (float)torque.getDouble(1), (float)torque.getDouble(2)};
                Node nodeControl = (Node) viroView;
                nodeControl.applyTorqueImpulse(torqueArray);
            }
        });
    }

    @ReactMethod
    public void setVelocity(final int viewTag, final ReadableArray velocity) {
        UIManagerModule uiManager = getReactApplicationContext().getNativeModule(UIManagerModule.class);
        uiManager.addUIBlock(new UIBlock() {
            @Override
            public void execute(NativeViewHierarchyManager nativeViewHierarchyManager) {
                View viroView = nativeViewHierarchyManager.resolveView(viewTag);
                if (!(viroView instanceof Node)){
                    throw new IllegalViewOperationException("Invalid view returned when applying velocity: expected a node-type control!");
                }

                if (velocity == null || velocity.size() != 3){
                    throw new IllegalViewOperationException("Invalid velocity params provided!");
                }

                float[] velocityArray = { (float)velocity.getDouble(0), (float)velocity.getDouble(1), (float)velocity.getDouble(2)};
                Node nodeControl = (Node) viroView;
                nodeControl.setVelocity(velocityArray, false);
            }
        });
    }
}
