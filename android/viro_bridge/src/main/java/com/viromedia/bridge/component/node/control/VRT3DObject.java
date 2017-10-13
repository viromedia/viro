/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import android.net.Uri;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.AsyncObject3DListener;
import com.viro.renderer.jni.ExecutableAnimation;
import com.viro.renderer.jni.Material;
import com.viro.renderer.jni.Node;
import com.viro.renderer.jni.Object3D;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.Helper;
import com.viromedia.bridge.utility.ViroLog;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class VRT3DObject extends VRTControl {
    private static final String TAG = ViroLog.getTag(VRT3DObject.class);

    private enum ObjectType {
        OBJ, VRX
    }

    private static class Object3dAnimation extends NodeAnimation {
        private Node mNode;

        public Object3dAnimation(ReactApplicationContext context, VRT3DObject parent) {
            super(context, parent);
            mNode = parent.getNodeJni();
        }

        @Override
        public ExecutableAnimation loadAnimation() {
            Set<String> animationKeys = mNode.getAnimationKeys();
            if (animationKeys.isEmpty()) {
                return super.loadAnimation();
            }

            if (mAnimationName != null) {
                if (animationKeys.contains(mAnimationName)) {
                    return new ExecutableAnimation(mNode, mAnimationName);
                }
                else {
                    return super.loadAnimation();
                }
            }
            else {
                return super.loadAnimation();
            }
        }
    }

    private Uri mSource;
    private List<String> mResources = null;
    private boolean mObjLoaded = false;
    private boolean mSourceChanged = false;
    private ObjectType mType;

    public VRT3DObject(ReactApplicationContext reactContext) {
        super(reactContext);
        mNodeAnimation = new Object3dAnimation(reactContext, this);
        mNodeAnimation.setNode(this);
    }

    @Override
    protected Node createNodeJni() {
        return new Object3D();
    }

    @Override
    public void onTearDown() {
        if (isTornDown()) {
            return;
        }
        super.onTearDown();
    }

    private Object3D getObject3D() {
        return (Object3D) getNodeJni();
    }

    public void setType(String type) {
        try {
            mType = ObjectType.valueOf(type.toUpperCase());
        } catch (IllegalArgumentException e) {
            throw new IllegalArgumentException("String [" + type + "] is not a valid object type.");
        }
    }

    public void setSource(String source) {
        if (source == null || source.trim().isEmpty()) {
            throw new IllegalArgumentException("source is a required prop for Viro3DObject");
        }
        mSource = Helper.parseUri(source, mReactContext);
        mSourceChanged = true;
    }

    public void setAnimation(ReadableMap animation) {
        super.setAnimation(animation);
        updateAnimation();
    }

    public void setResources(List<String> resources) {
        mResources = resources;
    }

    @Override
    public void setMaterials(List<Material> materials) {
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

        Node nodeJni = getNodeJni();
        if (nodeJni != null && !isTornDown()){
            nodeJni.removeAllChildNodes();
        }

        loadDidStart();

        AsyncObject3DListener listener = new AsyncObject3DListener() {
            @Override
            public void onObject3DLoaded(Object3D object, Object3D.Type type) {
                if (isTornDown()) {
                    return;
                }
                //object.transferToNode(getNodeJni());
                mObjLoaded = true;

                if (mMaterials != null) {
                    // set materials on the node after it's finished loading
                    setMaterials(mMaterials);
                }
                updateAnimation();
                loadDidEnd();
            }

            @Override
            public void onObject3DFailed(String error) {
                if (isTornDown()) {
                    return;
                }
                onError(error);
            }
        };

        boolean isVRX = mType == ObjectType.VRX;

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

            // When in release mode, the objects are packaged as resources so we use the
            // resource constructor
            getObject3D().loadModel(mSource.toString(), isVRX ? Object3D.Type.FBX : Object3D.Type.OBJ,
                    listener, resourceMap);
        } else {
            // When in debug mode (not release), the objects are loaded as URLs so we use
            // the URL constructor
            getObject3D().loadModel(mSource, isVRX ? Object3D.Type.FBX : Object3D.Type.OBJ,
                    listener);
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

    private void updateAnimation() {
        /*
         If no animation name was specified, then use the first keyframe animation,
         if available.
        */
        if (mNodeAnimation.getAnimationName() == null || mNodeAnimation.getAnimationName().length() == 0) {
            Set<String> animationKeys = getNodeJni().getAnimationKeys();
            if (!animationKeys.isEmpty()) {
                mNodeAnimation.setAnimationName(animationKeys.iterator().next());
            }
        }

        mNodeAnimation.updateAnimation();
    }
}
