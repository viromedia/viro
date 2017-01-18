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
    public static final String ON_START = "onStartViro";
    public static final String ON_FINISH = "onFinishViro";
    public static final String ON_HOVER = "onHoverViro";
    public static final String ON_CLICK = "onClickViro";
    public static final String ON_TOUCH = "onTouchViro";
}
