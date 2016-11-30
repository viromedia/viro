/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.viro.renderer.jni.BoxJni;

public class Box extends Control {
    public Box(ReactApplicationContext reactContext) {
        super(reactContext);

        /**
         * TODO
         * Pass in dimension / positional properties of a given ViroBox when creating the native BoxJNI element.
         */
        BoxJni boxGeometry = new BoxJni(2,4,2);
        getNodeJni().setGeometry(boxGeometry);
        getNodeJni().setPosition(0,0,-5);
    }
}
