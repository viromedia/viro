/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.AsyncObjListener;
import com.viro.renderer.jni.MaterialJni;
import com.viro.renderer.jni.ObjectJni;
import com.viromedia.bridge.utility.ViroEvents;

import java.util.List;

public class Object3d extends Control {
    private ObjectJni mNative3dObject;
    private String mSource;
    private boolean mObjLoaded = false;
    public Object3d(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    protected void onTearDown() {
        if (mNative3dObject != null){
            mNative3dObject.destroy();
        }
        super.onTearDown();
    }

    public void setSource(String source) {
        if (source == null || source.trim().isEmpty()) {
            throw new IllegalArgumentException("source is a required prop for Viro3DObject");
        }
        mSource = source;
    }

    @Override
    public void setMaterials(List<MaterialJni> materials) {
        if (mObjLoaded) {
            super.setMaterials(materials);
        }
        mMaterials = materials;
    }

    @Override
    protected void onPropsSet() {
        if (mSource == null) {
            return;
        }
        super.onPropsSet();

        ObjectJni oldObject3d = mNative3dObject;
        loadDidStart();

        mNative3dObject = new ObjectJni(mSource, new AsyncObjListener() {
            @Override
            public void onObjLoaded() {
                mObjLoaded = true;
                if (mMaterials != null) {
                    // set materials on the node after it's finished loading OBJ
                    setMaterials(mMaterials);
                }
                loadDidEnd();
            }
        });
        setGeometry(mNative3dObject);

        if (oldObject3d != null) {
            mNative3dObject.destroy();
        }
    }

    private void loadDidStart() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_LOAD_START,
                null
        );
    }

    private void loadDidEnd() {
        mReactContext.getJSModule(RCTEventEmitter.class).receiveEvent(
                getId(),
                ViroEvents.ON_LOAD_END,
                null
        );
    }

}
