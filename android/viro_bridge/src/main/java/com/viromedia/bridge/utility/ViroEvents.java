/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.utility;

/**
 * This class contains all the event callbacks that we have. If the names conflict
 * with React Native, it gives us an error, but it's apparently fine if our native
 * components share event names.
 */
public class ViroEvents {
    public static final String ON_LOAD_START = "onLoadStartViro";
    public static final String ON_LOAD_END = "onLoadEndViro";
    public static final String ON_ERROR = "onErrorViro";
    public static final String ON_UPDATE_TIME = "onUpdateTimeViro";
    public static final String ON_START = "onStartViro";
    public static final String ON_FINISH = "onFinishViro";
    public static final String ON_ANIMATION_START = "onAnimationStartViro";
    public static final String ON_ANIMATION_FINISH = "onAnimationFinishViro";
    public static final String ON_HOVER = "onHoverViro";
    public static final String ON_CLICK = "onClickViro";
    public static final String ON_TOUCH = "onTouchViro";
    public static final String ON_SWIPE = "onSwipeViro";
    public static final String ON_SCROLL = "onScrollViro";
    public static final String ON_DRAG = "onDragViro";
    public static final String ON_PLATFORM_UPDATE = "onPlatformUpdateViro";
    public static final String ON_CONTROLLER_STATUS = "onControllerStatusViro";
    public static final String ON_EXIT_VIRO = "onExitViro";
    public static final String ON_FUSE = "onFuseViro";
    public static final String ON_PINCH = "onPinchViro";
    public static final String ON_ROTATE = "onRotateViro";
    public static final String ON_BUFFER_START = "onBufferStartViro";
    public static final String ON_BUFFER_END = "onBufferEndViro";
    public static final String ON_COLLIDED = "onCollisionViro";
    public static final String ON_TRANSFORM_DELEGATE = "onNativeTransformDelegateViro";
    public static final String ON_TRACKING_INITIALIZED = "onTrackingInitializedViro";
    public static final String ON_AMBIENT_LIGHT_UPDATE = "onAmbientLightUpdateViro";
    public static final String ON_ANCHOR_FOUND = "onAnchorFoundViro";
    public static final String ON_ANCHOR_UPDATED = "onAnchorUpdatedViro";
    public static final String ON_ANCHOR_REMOVED = "onAnchorRemovedViro";
}
