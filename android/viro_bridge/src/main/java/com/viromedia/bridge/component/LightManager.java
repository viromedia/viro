/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.component;

import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.ViewGroupManager;


public abstract class LightManager extends ViewGroupManager<Light> {

    @Override
    protected Light createViewInstance(ThemedReactContext reactContext) {
        //TODO: properly create a light object
        return new Light(reactContext);
    }
}
