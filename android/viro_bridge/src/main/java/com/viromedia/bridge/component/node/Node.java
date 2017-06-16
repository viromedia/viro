/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component.node;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.JSApplicationCausedNativeException;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableType;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.IllegalViewOperationException;
import com.facebook.react.uimanager.PixelUtil;

import com.facebook.react.uimanager.events.RCTEventEmitter;
import com.viro.renderer.jni.BaseGeometry;
import com.viro.renderer.jni.EventDelegateJni;
import com.viro.renderer.jni.MaterialJni;
import com.viro.renderer.jni.NodeJni;
import com.viromedia.bridge.component.AnimatedComponent;
import com.viromedia.bridge.component.Component;
import com.viromedia.bridge.component.Light;
import com.viromedia.bridge.component.node.control.Image;
import com.viromedia.bridge.component.node.control.Surface;
import com.viromedia.bridge.component.node.control.Text;
import com.viromedia.bridge.component.node.control.VideoSurface;
import com.viromedia.bridge.utility.ComponentEventDelegate;
import com.viromedia.bridge.utility.ViroEvents;
import com.viromedia.bridge.utility.ViroLog;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;

import static com.viromedia.bridge.component.node.NodeManager.s2DUnitPer3DUnit;

/**
 * Node is inherited by any component which is represented by a VRONode in native
 */
public class Node extends Component {
    private static final String TAG = ViroLog.getTag(Node.class);

    // Always place the children of views .01 in front of the parent. This helps with z-fighting
    // and ensures that the child is always in front of the parent for hit detection
    private static final float sZIncrementToAvoidZFighting = (float) 0.01;
    protected final static boolean DEFAULT_CAN_HOVER = false;
    protected final static boolean DEFAULT_CAN_CLICK = false;
    protected final static boolean DEFAULT_CAN_TOUCH = false;
    protected final static boolean DEFAULT_CAN_SCROLL = false;
    protected final static boolean DEFAULT_CAN_SWIPE = false;
    public final static boolean DEFAULT_HIGH_ACCURACY_GAZE = false;
    protected final static boolean DEFAULT_CAN_DRAG = false;
    protected final static boolean DEFAULT_CAN_FUSE = false;
    protected final static float DEFAULT_TIME_TO_FUSE_MILLIS = 1000f;

    private NodeJni mNodeJni;
    protected float[] mPosition;
    protected float[] mRotation;
    protected float[] mScale;
    protected float[] mRotationPivot;
    protected float[] mScalePivot;
    protected float mOpacity;
    protected boolean mVisible;
    protected boolean mHighAccuracyGazeEnabled;
    protected List<MaterialJni> mMaterials;
    private EventDelegateJni mEventDelegateJni;

    // these are used to preserve the old 2D layout values which we'll store before and restore after
    // calling attemptRecalcLayout
    protected int mOldLeft;
    protected int mOldTop;
    protected int mOldRight;
    protected int mOldBottom;

    /**
     * This field is set to true when during a property setting iteration, this Node is one of the
     * highest elements in the scene tree that had its properties set/changed.
     */
    protected boolean mIsTopMostChangedElement = false;

    // True if this node initialized and contains a physics body.
    private boolean hasPhysicsBody = false;

    // Last known set physics properties for this node.
    private ReadableMap mPhysicsMap = null;
    private PhysicsBodyDelegate mPhysicsDelegate;

    public Node(ReactApplicationContext reactContext) {
        this(reactContext.getBaseContext(), null, -1, -1, reactContext);
    }

    public Node(Context context, AttributeSet attrs, int defStyleAttr,
                     int defStyleRes, ReactApplicationContext reactContext) {
        super(context, attrs, defStyleAttr, defStyleRes, reactContext);
        mNodeJni = new NodeJni();

        // Create and attach callbacks.
        mEventDelegateJni = new EventDelegateJni();
        mEventDelegateJni.setEventDelegateCallback(new ComponentEventDelegate(this));
        mNodeJni.setEventDelegateJni(mEventDelegateJni);
    }

    public NodeJni getNodeJni(){
        return mNodeJni;
    }

    @Override
    public void onTearDown() {
        super.onTearDown();
        if (mNodeJni != null){
            clearPhysicsBody();
            mEventDelegateJni.setEventDelegateCallback(null);
            mEventDelegateJni.destroy();
            mNodeJni.destroy();
            mNodeJni = null;
        }
    }

    // TODO: handle children separate from android viewgroup childViews
    @Override
    public void addView(View child, int index) {
        if (!isTornDown()) {
            if (child instanceof Light) {
                ((Light) child).addToNode(mNodeJni);
            } else if (child instanceof Node) {
                final Node childNode = (Node) child;
                mNodeJni.addChildNode(childNode.mNodeJni);
            } else if (child instanceof AnimatedComponent) {
                // We want the AnimatedComponent to be "invisible" to the scene graph so we need to add
                // the children of the AnimatedComponent to the parent of the AnimatedComponent natively.
                // Otherwise the parent of an AnimatedComponent has no idea that it has extra children
                // natively.
                // Note: AnimatedComponents can only have 1 child right now, so the loop is just
                // futureproofing
                AnimatedComponent animatedComponent = (AnimatedComponent) child;
                for (int i = 0; i < animatedComponent.getChildCount(); i++) {
                    if (!(animatedComponent.getChildAt(i) instanceof Node)) {
                        continue;
                    }

                    Node animatedTarget = (Node) animatedComponent.getChildAt(i);
                    if (!containsChild(animatedTarget)) {
                        addNativeChild(animatedTarget);
                    }
                }
            } else {
                // TODO: Throw Error? Red Screen?
            }
        }

        super.addView(child, index);
    }

    @Override
    public void removeViewAt(int index) {
        if (!isTornDown()) {
            View child = getChildAt(index);
            if (child instanceof Light) {
                ((Light) child).removeFromNode(mNodeJni);
            } else if (child instanceof Node) {
                final Node childNode = (Node) child;
                mNodeJni.removeChildNode(childNode.mNodeJni);
            } else if (child instanceof AnimatedComponent) {
                AnimatedComponent animatedComponent = (AnimatedComponent) child;
                for (int i = 0; i < animatedComponent.getChildCount(); i++) {
                    if (!(animatedComponent.getChildAt(i) instanceof Node)) {
                        continue;
                    }

                    Node animatedTarget = (Node) animatedComponent.getChildAt(i);
                    if (containsChild(animatedTarget)) {
                        removeNativeChild(animatedTarget);
                    }
                }
            } else {
                // TODO: Throw Error? Red Screen?
            }
        }

        super.removeViewAt(index);
    }

    /*
     * Determines non-recursively if the child is within the given parent viewgroup.
     */
    public boolean containsChild(Node child) {
        for (int i = 0; i < this.getChildCount(); i ++) {
            View view = getChildAt(i);
            if (view == child) {
                return true;
            }
        }
        return false;
    }

    public void addNativeChild(Node child) {
        if (isTornDown()) {
            return;
        }
        mNodeJni.addChildNode(child.getNodeJni());
    }

    public void removeNativeChild(Node child) {
        if (isTornDown()) {
            return;
        }
        mNodeJni.removeChildNode(child.getNodeJni());
    }

    @Override
    protected void onPropsSet() {
        super.onPropsSet();
        // Because props are set from the bottom of the scene tree up, if props are set on this
        // Node, then we know its the top-most changed element.
        mIsTopMostChangedElement = true;
        setChildrenNotTopMostChangedElement();
    }

    /**
     * This function ensures that the children of this element aren't the top-most changed element.
     */
    protected void setChildrenNotTopMostChangedElement() {
        for (int i = 0; i < getChildCount(); i++) {
            View child = getChildAt(i);
            if (child instanceof Node) {
                ((Node) child).mIsTopMostChangedElement = false;
                ((Node) child).setChildrenNotTopMostChangedElement();
            }
        }
    }

    @Override
    public void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        // Only call recalcLayout if we're the top-most changed element because we don't want to
        // recalc the 3D position/size N times (N = number of changed elements).
        if (mIsTopMostChangedElement) {
            recalcLayout();
        }
    }

    @Override
    public void recalcLayout() {
        // First thing we should do is store our old 2d bounds
        store2DBounds();

        // Nodes should attempt to recalc, but if it fails it should always call super.recalcLayout() to recurse.
        attemptRecalcLayout();
        super.recalcLayout();

        // once we've laid out all the children, we need to restore the 2D bounds
        restore2DBounds();
    }

    protected void attemptRecalcLayout() {
        // Attempt to find a FlexView parent OR FlexView grandparent if the parent is an AnimatedComponent
        Node superView = null;
        if (getParent() != null && getParent() instanceof FlexView) {
            superView = (Node) getParent();
        } else if(getParent() != null && getParent() instanceof AnimatedComponent) {
            if (getParent().getParent() != null && getParent().getParent() instanceof FlexView) {
                superView = (Node) getParent().getParent();
            }
        }

        // if we failed to find an acceptable parent, just return.
        if (superView == null) {
            return;
        }

        float[] position = get2DPosition();
        float[] size = get2DSize();

        float[] parentPosition = superView.get2DPosition();
        float[] parentSize = superView.get2DSize();

        // flip y because in our 3D coordinates, y increases as you go up, rather than downwards on mobile/web.
        float transformedY = parentSize[1] - position[1];

        // transform by subtracting by center of superview
        float transformedX = position[0] - parentPosition[0];
        transformedY = transformedY - parentPosition[1];

        // we need the factor that the CSS algorithm applies to the original width/height.
        float pixelsPerDIP = PixelUtil.toPixelFromDIP(1.0);

        // calculate 3D size/position
        float width3d = size[0] / pixelsPerDIP / s2DUnitPer3DUnit;
        float height3d = size[1] / pixelsPerDIP / s2DUnitPer3DUnit;
        transformedX = transformedX / pixelsPerDIP / s2DUnitPer3DUnit;
        transformedY = transformedY / pixelsPerDIP / s2DUnitPer3DUnit;

        // set position.
        setPosition(new float[]{transformedX, transformedY, sZIncrementToAvoidZFighting});

        // TODO: create Flexible interface
        // set the size (either through scale or width/height if the node supports it).
        if (this instanceof Image) {
            Image image = (Image) this;
            image.setWidth(width3d);
            image.setHeight(height3d);
            image.onPropsSet();
        } else if (this instanceof FlexView) {
            FlexView flexView = (FlexView) this;
            flexView.setWidth(width3d);
            flexView.setHeight(height3d);
        }else if (this instanceof VideoSurface) {
            VideoSurface videoSurface = (VideoSurface)this;
            videoSurface.setWidth(width3d);
            videoSurface.setHeight(height3d);
            videoSurface.onPropsSet();
        } else if (this instanceof Surface) {
            Surface surface = (Surface) this;
            surface.setWidth(width3d);
            surface.setHeight(height3d);
            surface.updateSurface();
        } else if (this instanceof Text) {
            Text text = (Text) this;
            text.setWidth(width3d);
            text.setHeight(height3d);
            text.onPropsSet();
        } else {
            this.setScale(new float[]{width3d, height3d, 1});
        }

        // normalize bounds so that they assume left & top == 0, we shouldn't set the pivots because
        // 1) they're already correct
        // 2) if you set pivots, the view will cease to use its default (center) and use the value you
        //    give it (see View's setPivotX/setPivotY javadocs)
        setLeft(0);
        setTop(0);
        setRight((int) size[0]);
        setBottom((int) size[1]);
    }

    protected void setPosition(float[] position) {
        if (isTornDown()) {
            return;
        }
        if (position.length != 3){
            throw new IllegalArgumentException("Missing a position value: All three " +
                    "[x,y,z] axis value are needed.");
        }

        mPosition = position;
        mNodeJni.setPosition(position);
    }

    protected void setRotation(float[] rotation) {
        if (isTornDown()) {
            return;
        }
        if (rotation.length != 3){
            throw new IllegalArgumentException("Missing a rotation value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mRotation = rotation;
        mNodeJni.setRotation(rotation);
    }

    protected void setScale(float[] scale) {
        if (isTornDown()) {
            return;
        }
        if (scale.length != 3){
            throw new IllegalArgumentException("Missing a scale value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mScale = scale;
        mNodeJni.setScale(scale);
    }

    protected void setRotationPivot(float[] pivot) {
        if (isTornDown()) {
            return;
        }
        if (pivot.length != 3){
            throw new IllegalArgumentException("Missing a pivot value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mRotationPivot = pivot;
        mNodeJni.setRotationPivot(pivot);
    }

    protected void setScalePivot(float[] pivot) {
        if (isTornDown()) {
            return;
        }
        if (pivot.length != 3){
            throw new IllegalArgumentException("Missing a pivot value: All three " +
                    "[x,y,z] axis values are needed.");
        }
        mScalePivot = pivot;
        mNodeJni.setScalePivot(pivot);
    }

    protected void setOpacity(float opacity) {
        if (isTornDown()) {
            return;
        }
        mOpacity = opacity;
        mNodeJni.setOpacity(opacity);
    }

    protected void setVisible(boolean visible) {
        if (isTornDown()) {
            return;
        }
        mVisible = visible;
        mNodeJni.setVisible(visible);
    }

    public void setHighAccuracyGaze(boolean highAccuracyGazeEnabled){
        if (isTornDown()) {
            return;
        }
        mHighAccuracyGazeEnabled = highAccuracyGazeEnabled;
        mNodeJni.setHighAccuracyGaze(highAccuracyGazeEnabled);
    }

    protected void setGeometry(BaseGeometry geometry) {
        if (isTornDown()) {
            return;
        }
        mNodeJni.setGeometry(geometry);
        if (mMaterials != null) {
            mNodeJni.setMaterials(mMaterials);
        }
    }

    protected void setMaterials(List<MaterialJni> materials) {
        if (isTornDown()) {
            return;
        }
        mMaterials = materials;
        mNodeJni.setMaterials(materials);
    }

    protected void setTransformBehaviors(String[] transformBehaviors) {
        if (isTornDown()) {
            return;
        }
        mNodeJni.setTransformBehaviors(transformBehaviors);
    }

    protected float[] get2DPosition() {
        float[] arr = {getLeft() + getPivotX(), getTop() + getPivotY()};
        return arr;
    }

    protected float[] get2DSize() {
        float[] arr = {getRight() - getLeft(), getBottom() - getTop()};
        return arr;
    }

    protected void store2DBounds() {
        mOldLeft = getLeft();
        mOldTop = getTop();
        mOldRight = getRight();
        mOldBottom = getBottom();
    }

    protected void restore2DBounds() {
        setLeft(mOldLeft);
        setTop(mOldTop);
        setRight(mOldRight);
        setBottom(mOldBottom);
    }

    protected void setCanHover(boolean canHover){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_HOVER, canHover);
    }

    protected void setCanClick(boolean canClick){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_CLICK, canClick);
    }

    protected void setCanTouch(boolean canTouch){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_TOUCH, canTouch);
    }

    protected void setCanScroll(boolean canScroll){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_SCROLL, canScroll);
    }

    protected void setCanSwipe(boolean canSwipe){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_SWIPE, canSwipe);
    }

    protected void setCanDrag(boolean canDrag){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_DRAG, canDrag);
    }

    protected void setCanFuse(boolean canFuse){
        mEventDelegateJni.setEventEnabled(EventDelegateJni.EventAction.ON_FUSE, canFuse);
    }

    protected void setTimeToFuse(float durationInMillis){
        mEventDelegateJni.setTimeToFuse(durationInMillis);
    }

    public void setPhysicsBody(ReadableMap map){
        // If un-setting the physicsBody, clear it from the node.
        if (map == null){
            clearPhysicsBody();
            mPhysicsMap = map;
            return;
        }

        // Else update the current physicsBody with the new properties, recreating
        // the body if needed.
        recreatePhysicsBodyIfNeeded(map);
        updatePhysicsBodyProperties(map);
        applyForcesOnBody(map);

        // Finally save a copy of the last known set physics properties.
        mPhysicsMap = map;
    }

    public void setCanCollide(boolean canCollide) {
        if (canCollide && mPhysicsDelegate == null) {
            mPhysicsDelegate = new PhysicsBodyDelegate(this);
        } else if (!canCollide) {
            mPhysicsDelegate = null;
        }

        if (!hasPhysicsBody){
            return;
        }

        if (mPhysicsDelegate != null){
            mNodeJni.setPhysicsDelegate(mPhysicsDelegate);
        } else {
            mNodeJni.setPhysicsDelegate(null);
        }
    }

    public void setViroTag(String tag){
        mNodeJni.setTag(tag);
    }

    private void recreatePhysicsBodyIfNeeded(ReadableMap map){
        float mass = 0;
        if (map.hasKey("mass")){
            mass = (float) map.getDouble("mass");
        }

        // Determine if the physics body type has changed
        String bodyTypeProp = map.getString("type");
        String bodyTypeCurrent = (mPhysicsMap != null && mPhysicsMap.hasKey("type")) ?
                mPhysicsMap.getString("type") : null;
        boolean hasBodyTypeChanged = bodyTypeProp != bodyTypeCurrent;
        if (bodyTypeProp != null){
            hasBodyTypeChanged = !bodyTypeProp.equals(bodyTypeCurrent);
        }

        String bodyTypeError = mNodeJni.checkIsValidBodyType(bodyTypeProp, mass);
        if (bodyTypeError != null){
            throw new JSApplicationCausedNativeException(bodyTypeError);
        }

        // Determine if the physics shape has changed
        ReadableMap shapeTypeProp = map.hasKey("shape") ? map.getMap("shape") : null;
        ReadableMap shapeTypeCurrent = (mPhysicsMap != null && mPhysicsMap.hasKey("shape")) ?
                mPhysicsMap.getMap("shape") : null;
        boolean hasShapeChanged = shapeTypeProp != shapeTypeCurrent;
        if (shapeTypeProp != null){
            hasShapeChanged = !shapeTypeProp.equals(shapeTypeCurrent);
        }

        // Create or update the VROPhysicsBody only if needed
        if (!hasPhysicsBody || hasBodyTypeChanged || hasShapeChanged){
            String propShapeType = null;
            float params[] = {};

            // Recreate a physics shape with the latest properties by grabbing
            // the current shapeType (required in JS if providing a physics shape)
            if (shapeTypeProp != null){
                propShapeType = shapeTypeProp.getString("type");
                if (shapeTypeProp.hasKey("params")) {
                    ReadableArray readableParams = shapeTypeProp.getArray("params");
                    params = new float[readableParams.size()];
                    for (int i = 0; i < readableParams.size(); i++) {
                        params[i] = (float) readableParams.getDouble(i);
                    }
                }

                String error = mNodeJni.checkIsValidShapeType(propShapeType, params);
                if (error != null){
                    throw new JSApplicationCausedNativeException(error);
                }
            }

            if (!hasPhysicsBody || hasBodyTypeChanged){
                clearPhysicsBody();
                createPhysicsBody(bodyTypeProp, mass, propShapeType, params);
            } else {
                mNodeJni.setPhysicsShape(propShapeType, params);
            }
        }
    }

    private void updatePhysicsBodyProperties(ReadableMap map){
        if (map.hasKey("mass")) {
            float mass = (float)map.getDouble("mass");
            String bodyType = map.getString("type");
            String bodyTypeError = mNodeJni.checkIsValidBodyType(bodyType, mass);
            if (bodyTypeError != null){
                throw new JSApplicationCausedNativeException(bodyTypeError);
            }
            mNodeJni.setPhysicsMass(mass);
        }

        if (map.hasKey("inertia")){
            ReadableArray paramsArray = map.getArray("inertia");
            float inertiaArray[] = new float[paramsArray.size()];
            for (int i = 0; i < paramsArray.size(); i ++){
                inertiaArray[i] = (float) paramsArray.getDouble(i);
            }

            if (inertiaArray.length != 3){
                throw new JSApplicationCausedNativeException("Incorrect parameters " +
                        "provided for inertia, expected: [x, y, z]!");
            }

            mNodeJni.setPhysicsInertia(inertiaArray);
        }

        if (map.hasKey("friction")) {
            mNodeJni.setPhysicsFriction((float)map.getDouble("friction"));
        }

        if (map.hasKey("restitution")) {
            mNodeJni.setPhysicsRestitution((float)map.getDouble("restitution"));
        }

        if (map.hasKey("enabled")) {
            mNodeJni.setPhysicsEnabled(map.getBoolean("enabled"));
        }

        if (map.hasKey("useGravity")) {
            String bodyType = map.getString("type");
            if (!bodyType.equalsIgnoreCase("dynamic")){
                ViroLog.warn(TAG,"Attempted to set useGravity for non-dynamic phsyics bodies.");
            } else {
                mNodeJni.setPhsyicsUseGravity(map.getBoolean("useGravity"));
            }
        }

        if (map.hasKey("velocity")) {
            ReadableArray paramsArray = map.getArray("velocity");
            float velocityArray[] = new float[paramsArray.size()];
            for (int i = 0; i < paramsArray.size(); i ++){
                velocityArray[i] = (float) paramsArray.getDouble(i);
            }

            if (velocityArray.length != 3){
                throw new JSApplicationCausedNativeException("Incorrect parameters " +
                        "provided for velocity, expected: [x, y, z]!");
            }

            mNodeJni.setPhysicsVelocity(velocityArray, true);
        } else {
            mNodeJni.setPhysicsVelocity(new float[]{0,0,0}, true);
        }
    }

    private void applyForcesOnBody(ReadableMap map){
        // Determine if the applied torque has changed
        ReadableArray torqueProp = map.hasKey("torque") ? map.getArray("torque") : null;
        boolean currentMapHasTorque = mPhysicsMap != null && mPhysicsMap.hasKey("torque");
        ReadableArray currentTorque = currentMapHasTorque ? mPhysicsMap.getArray("torque") : null;
        boolean hasTorqueChanged = torqueProp != currentTorque;
        if (torqueProp != null){
            hasTorqueChanged = !torqueProp.equals(currentTorque);
        }

        // Determine if the applied force has changed
        ArrayList<ReadableMap> forcesListProp = getForcesFromReactMap(map);
        ArrayList<ReadableMap> forcesListCurrent = getForcesFromReactMap(mPhysicsMap);
        boolean hasForceChanged = !forcesListProp.equals(forcesListCurrent);

        // If nothing has changed, return
        if (!hasForceChanged && !hasTorqueChanged){
            return;
        }

        mNodeJni.clearPhysicsForce();

        // Apply Torque
        if (map.hasKey("torque")){
            ReadableArray paramsArray = map.getArray("torque");
            float torqueArray[] = new float[paramsArray.size()];
            for (int i = 0; i < paramsArray.size(); i ++){
                torqueArray[i] = (float) paramsArray.getDouble(i);
            }

            if (torqueArray.length != 3){
                throw new JSApplicationCausedNativeException("Incorrect parameters " +
                        "provided for torque, expected: [x, y, z]!");
            }

            mNodeJni.applyPhysicsTorque(torqueArray);
        }

        // Apply force
        for (ReadableMap forceMap : forcesListProp){
            float forceArray[];
            if (forceMap.hasKey("power")){
                ReadableArray paramsArray = forceMap.getArray("power");
                forceArray = new float[paramsArray.size()];
                for (int i = 0; i < paramsArray.size(); i ++){
                    forceArray[i] = (float) paramsArray.getDouble(i);
                }

                if (forceArray.length != 3){
                    throw new JSApplicationCausedNativeException("Incorrect parameters " +
                            "provided for force's power, expected: [x, y, z]!");
                }
            } else {
                throw new JSApplicationCausedNativeException("Incorrect parameters: missing" +
                        " power of format [x, y, z] for force!");
            }

            float positionArray[];
            if (forceMap.hasKey("position")){
                ReadableArray paramsArray = forceMap.getArray("position");
                positionArray = new float[paramsArray.size()];
                for (int i = 0; i < paramsArray.size(); i ++){
                    positionArray[i] = (float) paramsArray.getDouble(i);
                }

                if (positionArray.length != 3){
                    throw new JSApplicationCausedNativeException("Incorrect parameters " +
                            "provided for force's position, expected: [x, y, z]!");
                }
            } else {
                // Fallback to a central force if no position is provided
                positionArray = new float[]{0,0,0};
            }

            mNodeJni.applyPhysicsForce(forceArray, positionArray);
        }
    }

    // Parse, check and return a list of valid force props from a given Readable map.
    private ArrayList<ReadableMap> getForcesFromReactMap(ReadableMap map){
        ArrayList<ReadableMap> maps = new ArrayList<ReadableMap>();
        if (map != null && map.hasKey("force")){
            ReadableType type = map.getType("force");
            if (type.equals(ReadableType.Array)){
                ReadableArray reactForceArray = map.getArray("force");
                for (int i =0; i < reactForceArray.size(); i ++){
                    maps.add(reactForceArray.getMap(i));
                }
            } else if (type.equals(ReadableType.Map)){
                maps.add(map.getMap("force"));
            } else if (!type.equals(ReadableType.Null)) {
                throw new IllegalViewOperationException("Invalid force format!");
            }
        }
        return maps;
    }

    private void createPhysicsBody(String bodyType, float mass, String shapeType,
                                   float shapeParams[]){
        mNodeJni.initPhysicsBody(bodyType, mass, shapeType, shapeParams);
        if (mScene != null){
            mScene.addPhysicsBodyToScene(this);
        }

        if (mPhysicsDelegate != null){
            mNodeJni.setPhysicsDelegate(mPhysicsDelegate);
        } else {
            mNodeJni.setPhysicsDelegate(null);
        }

        hasPhysicsBody = true;
    }

    private void clearPhysicsBody(){
        if (mScene != null){
            mScene.removePhysicsBodyFromScene(this);
        }

        mNodeJni.clearPhysicsBody();
        hasPhysicsBody = false;
    }

    @Override
    public void setScene(Scene scene) {
        if (hasPhysicsBody) {
            scene.addPhysicsBodyToScene(this);
        }
        super.setScene(scene);
    }

    public void applyImpulse(float[] force, float[] position){
        if (!hasPhysicsBody){
            ViroLog.error(TAG, "Attempted to set an impulse force on a non-physics node");
            return;
        }
        mNodeJni.applyPhysicsImpulse(force, position);
    }

    public void applyTorqueImpulse(float[] torque){
        if (!hasPhysicsBody){
            ViroLog.error(TAG, "Attempted to set an impulse force on a non-physics node");
            return;
        }
        mNodeJni.applyPhysicsTorqueImpulse(torque);
    }

    public void setVelocity(float[] velocity, boolean isConstant){
        if (!hasPhysicsBody){
            ViroLog.error(TAG, "Attempted to set a velocity on a non-physics node");
            return;
        }

        mNodeJni.setPhysicsVelocity(velocity, isConstant);
    }

    protected class PhysicsBodyDelegate implements NodeJni.PhysicsDelegate{
        private WeakReference<Component> weakComponent;
        public PhysicsBodyDelegate(Component component){
            weakComponent = new WeakReference<Component>(component);
        }

        @Override
        public void onCollided(String collidedTag, float[] collidedPoint, float[] collidedNormal) {
            Component node = weakComponent.get();
            if (node == null){
                return;
            }

            WritableArray points = Arguments.createArray();
            points.pushDouble(collidedPoint[0]);
            points.pushDouble(collidedPoint[1]);
            points.pushDouble(collidedPoint[2]);

            WritableArray normals = Arguments.createArray();
            normals.pushDouble(collidedNormal[0]);
            normals.pushDouble(collidedNormal[1]);
            normals.pushDouble(collidedNormal[2]);

            WritableMap event = Arguments.createMap();
            event.putString("viroTag", collidedTag);
            event.putArray("collidedPoint", points);
            event.putArray("collidedNormal", normals);

            node.getReactContext().getJSModule(RCTEventEmitter.class).receiveEvent(
                    node.getId(),
                    ViroEvents.ON_COLLIDED,
                    event);
        }
    }
}
