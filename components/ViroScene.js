/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import { requireNativeComponent, findNodeHandle, View } from 'react-native';
import React, { Component } from 'react';
var PropTypes = React.PropTypes;
var NativeModules = require('react-native').NativeModules;

var ViroScene = React.createClass({
  propTypes: {
    ...View.propTypes,

    onHover: React.PropTypes.func,
    onClick: React.PropTypes.func,
    onClickState: React.PropTypes.func,
    onTouch: React.PropTypes.func,
    onScroll: React.PropTypes.func,
    onSwipe: React.PropTypes.func,
    onFuse: PropTypes.oneOfType([
      React.PropTypes.shape({
        callback: React.PropTypes.func.isRequired,
        timeToFuse: PropTypes.number
      }),
      React.PropTypes.func
    ]),
    onPlatformUpdate: React.PropTypes.func,
    /**
     * Describes the acoustic properties of the room around the user
     */
    soundRoom: PropTypes.shape({
      // The x, y and z dimensions of the room
      size: PropTypes.arrayOf(PropTypes.number).isRequired,
      wallMaterial: PropTypes.string,
      ceilingMaterial: PropTypes.string,
      floorMaterial: PropTypes.string,
    }),
    physicsWorld: PropTypes.shape({
      gravity: PropTypes.arrayOf(PropTypes.number).isRequired,
      drawBounds: PropTypes.bool,
    }),
  },

  _onHover: function(event: Event) {
    this.props.onHover && this.props.onHover(event.nativeEvent.isHovering, event.nativeEvent.position, event.nativeEvent.source);
  },

  _onClick: function(event: Event) {
    this.props.onClick && this.props.onClick(event.nativeEvent.position, event.nativeEvent.source);
  },

  _onClickState: function(event: Event) {
    this.props.onClickState && this.props.onClickState(event.nativeEvent.clickState, event.nativeEvent.position, event.nativeEvent.source);
    let CLICKED = 3; // Value representation of Clicked ClickState within EventDelegateJni.
    if (event.nativeEvent.clickState == CLICKED){
        this._onClick(event)
    }
  },

  _onTouch: function(event: Event) {
    this.props.onTouch && this.props.onTouch(event.nativeEvent.touchState, event.nativeEvent.touchPos, event.nativeEvent.source);
  },

  _onScroll: function(event: Event) {
    this.props.onScroll && this.props.onScroll(event.nativeEvent.scrollPos, event.nativeEvent.source);
  },

  _onSwipe: function(event: Event) {
    this.props.onSwipe && this.props.onSwipe(event.nativeEvent.swipeState, event.nativeEvent.source);
  },

  _onFuse: function(event: Event){
    if (this.props.onFuse){
      if (typeof this.props.onFuse === 'function'){
        this.props.onFuse(event.nativeEvent.source);
      } else if (this.props.onFuse != undefined && this.props.onFuse.callback != undefined){
        this.props.onFuse.callback(event.nativeEvent.source);
      }
    }
  },

  _onPlatformUpdate: function(event: Event) {
    /**
     * ##### DEPRECATION WARNING - 'vrPlatform' is deprecated in favor of 'platform'! Support
     * for 'vrPlatform' may be removed in the future.
     */
    event.nativeEvent.platformInfoViro.vrPlatform = event.nativeEvent.platformInfoViro.platform
    this.props.onPlatformUpdate && this.props.onPlatformUpdate(event.nativeEvent.platformInfoViro);
  },

  async findCollisionsWithRayAsync(from, to, closest, viroTag) {
    return await NativeModules.VRTSceneModule.findCollisionsWithRayAsync(findNodeHandle(this), from, to, closest, viroTag);
  },

  async findCollisionsWithShapeAsync(from, to, shapeString, shapeParam, viroTag) {
    return await NativeModules.VRTSceneModule.findCollisionsWithShapeAsync(findNodeHandle(this), from, to, shapeString, shapeParam, viroTag);
  },

  /**
   * ##### DEPRECATION WARNING - this prop may be removed in future releases #####
   */
  async getCameraPositionAsync() {
    console.warn("[Viro] ViroScene.getCameraPositionAsync has been DEPRECATED. Please use getCameraOrientationAsync instead.");
    var orientation = await NativeModules.VRTCameraModule.getCameraOrientation(findNodeHandle(this));
    position = [orientation[0], orientation[1], orientation[2]];
    return position;
  },

  async getCameraOrientationAsync(){
    var orientation = await NativeModules.VRTCameraModule.getCameraOrientation(findNodeHandle(this));
    return {
      position: [orientation[0], orientation[1], orientation[2]],
      rotation: [orientation[3], orientation[4], orientation[5]],
      forward: [orientation[6], orientation[7], orientation[8]],
      up: [orientation[9], orientation[10], orientation[11]],
    }
  },

  getChildContext: function() {
    return {
      cameraDidMount: function(camera) {
        if (camera.props.active) {
          NativeModules.VRTCameraModule.setSceneCamera(findNodeHandle(this), findNodeHandle(camera));
        }
      }.bind(this),
      cameraWillUnmount: function(camera) {
        if (camera.props.active) {
          NativeModules.VRTCameraModule.removeSceneCamera(findNodeHandle(this), findNodeHandle(camera));
        }
      }.bind(this),
      cameraWillReceiveProps: function(camera, nextProps) {
        if (nextProps.active) {
          NativeModules.VRTCameraModule.setSceneCamera(findNodeHandle(this), findNodeHandle(camera));
        }
        else {
          NativeModules.VRTCameraModule.removeSceneCamera(findNodeHandle(this), findNodeHandle(camera));
        }
      }.bind(this),
    };
  },

  setNativeProps: function(nativeProps) {
    this._component.setNativeProps(nativeProps);
  },

  render: function() {
    let timeToFuse = undefined;
    if (this.props.onFuse != undefined && typeof this.props.onFuse === 'object'){
        timeToFuse = this.props.onFuse.timeToFuse;
    }

    return (
      <VRTScene
        {...this.props}
        ref={ component => {this._component = component; }}
        canHover={this.props.onHover != undefined}
        canClick={this.props.onClick != undefined || this.props.onClickState != undefined}
        canTouch={this.props.onTouch != undefined}
        canScroll={this.props.onScroll != undefined}
        canSwipe={this.props.onSwipe != undefined}
        canFuse={this.props.onFuse != undefined}
        onHoverViro={this._onHover}
        onClickViro={this._onClickState}
        onTouchViro={this._onTouch}
        onScrollViro={this._onScroll}
        onSwipeViro={this._onSwipe}
        onFuseViro={this._onFuse}
        onPlatformUpdateViro={this._onPlatformUpdate}
        timeToFuse={timeToFuse}
        />
    );
  },
});

ViroScene.childContextTypes = {
  cameraDidMount: React.PropTypes.func,
  cameraWillUnmount: React.PropTypes.func,
  cameraWillReceiveProps: React.PropTypes.func,
};

var VRTScene = requireNativeComponent(
    'VRTScene', ViroScene, {
        nativeOnly: {
          canHover: true,
          canClick: true,
          canTouch: true,
          canScroll: true,
          canSwipe: true,
          canDrag: true,
          canFuse: true,
          canCollide: true,
          onHoverViro: true,
          onClickViro: true,
          onTouchViro: true,
          onScrollViro: true,
          onSwipeViro: true,
          onDragViro:true,
          onPlatformUpdateViro: true,
          onFuseViro:true,
          timeToFuse:true,
          physicsBody:true,
          onCollidedViro:true,
          postProcessEffects:true,
        }
    }
);

module.exports = ViroScene;
