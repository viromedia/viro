/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.net.Uri;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.AsyncObjListener;
import com.viro.renderer.jni.MaterialJni;
import com.viro.renderer.jni.ObjectJni;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.Helper;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Object3d extends Control {

    private ObjectJni mNative3dObject;
    private Uri mSource;
    private List<String> mResources = null;
    private boolean mObjLoaded = false;
    private boolean mSourceChanged = false;

    public Object3d(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    public void onTearDown() {
        if (isTornDown()) {
            return;
        }
        if (mNative3dObject != null){
            mNative3dObject.destroy();
            mNative3dObject = null;
        }
        super.onTearDown();
    }

    public void setSource(String source) {
        if (source == null || source.trim().isEmpty()) {
            throw new IllegalArgumentException("source is a required prop for Viro3DObject");
        }
        mSource = Helper.parseUri(source, mReactContext);
        mSourceChanged = true;
    }

    public void setResources(List<String> resources) {
        mResources = resources;
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
        if (mSource == null || !mSourceChanged) {
            return;
        }

        super.onPropsSet();
        ObjectJni oldObject3d = mNative3dObject;
        loadDidStart();

        AsyncObjListener listener = new AsyncObjListener() {
            @Override
            public void onObjLoaded() {
                if (isTornDown()) {
                    return;
                }
                mObjLoaded = true;
                if (mMaterials != null) {
                    // set materials on the node after it's finished loading OBJ
                    setMaterials(mMaterials);
                }

                loadDidEnd();
            }

            @Override
            public void onObjFailed(String error) {
                if (isTornDown()) {
                    return;
                }
                onError(error);
            }
        };

        // if the source is from resources, then pass in the resources it depends on (if any)
        if (mSource.getScheme().equals("res")) {
            Map<String, String> resourceMap = null;
            if (mResources != null) {
                resourceMap = new HashMap<>();
                for (String resource : mResources) {
                    Uri uri = Helper.parseUri(resource, getContext());
                    resourceMap.put(resource, uri.toString());
                }
            }
            mNative3dObject = new ObjectJni(mSource, listener, resourceMap);
        } else {
            mNative3dObject = new ObjectJni(mSource, listener);
        }
        setGeometry(mNative3dObject);

        if (oldObject3d != null) {
            oldObject3d.destroy();
        }
        mSourceChanged = false;
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
