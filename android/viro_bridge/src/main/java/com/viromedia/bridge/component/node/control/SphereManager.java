/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node.control;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;
import com.viromedia.bridge.component.node.Node;
import com.viromedia.bridge.component.node.NodeManager;

/**
 * SphereManager for building a {@link Sphere}
 * corresponding to the ViroSphere.js control.
 */
public class SphereManager extends NodeManager<Sphere> {

    public SphereManager(ReactApplicationContext context){
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSphere";
    }

    @Override
    protected Sphere createViewInstance(ThemedReactContext reactContext) {
        return new Sphere(getContext());
    }

    @ReactProp(name = "facesOutward", defaultBoolean = Sphere.DEFAULT_FACES_OUTWARD)
    public void setFacesOutward(Sphere sphere, boolean facesOutward) {
        sphere.setFacesOutward(facesOutward);
    }

    @ReactProp(name = "widthSegmentCount", defaultInt = Sphere.DEFAULT_WIDTH_SEGMENT)
    public void setWidthSegmentCount(Sphere sphere, int widthSegmentCount) {
        sphere.setWidthSegmentCount(widthSegmentCount);
    }

    @ReactProp(name = "heightSegmentCount", defaultInt = Sphere.DEFAULT_HEIGHT_SEGMENT)
    public void setHeightSegmentCount(Sphere sphere, int heightSegmentCount) {
        sphere.setHeightSegmentCount(heightSegmentCount);
    }

    @ReactProp(name = "radius", defaultFloat = Sphere.DEFAULT_RADIUS)
    public void setRadius(Sphere sphere, float radius) {
        sphere.setRadius(radius);
    }

    @ReactProp(name = "highAccuracyGaze", defaultBoolean = Node.DEFAULT_HIGH_ACCURACY_GAZE)
    public void setHighAccuracyGaze(Sphere sphere, boolean highAccuracyGaze) {
        sphere.setHighAccuracyGaze(highAccuracyGaze);
    }
}