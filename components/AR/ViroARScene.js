/**
 * Copyright (c) 2017-present, Viro Media, Inc.
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

var ViroARScene = React.createClass({
  propTypes: {
    ...View.propTypes,
    ignoreEventHandling: PropTypes.bool,
    onHover: React.PropTypes.func,
    onClick: React.PropTypes.func,
    onClickState: React.PropTypes.func,
    onTouch: React.PropTypes.func,
    onScroll: React.PropTypes.func,
    onSwipe: React.PropTypes.func,
    onDrag: React.PropTypes.func,
    onPinch: React.PropTypes.func,
    onRotate: React.PropTypes.func,
    onFuse: PropTypes.oneOfType([
      React.PropTypes.shape({
        callback: React.PropTypes.func.isRequired,
        timeToFuse: PropTypes.number
      }),
      React.PropTypes.func
    ]),
    onTrackingInitialized: React.PropTypes.func,
    onPlatformUpdate: React.PropTypes.func,
    onAmbientLightUpdate: React.PropTypes.func,
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
    postProcessEffects: PropTypes.arrayOf(PropTypes.string),
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

  _onPinch: function(event: Event) {
    this.props.onPinch && this.props.onPinch(event.nativeEvent.pinchState, event.nativeEvent.scaleFactor, event.nativeEvent.source);
  },

  _onRotate: function(event: Event) {
    this.props.onRotate && this.props.onRotate(event.nativeEvent.rotateState, event.nativeEvent.rotationFactor, event.nativeEvent.source);
  },

  _onDrag: function(event: Event) {
      this.props.onDrag && this.props.onDrag(event.nativeEvent.dragToPos, event.nativeEvent.source);
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
    this.props.onPlatformUpdate && this.props.onPlatformUpdate(event.nativeEvent.platformInfoViro);
  },

  _onTrackingInitialized: function(event: Event) {
    this.props.onTrackingInitialized && this.props.onTrackingInitialized();
  },

  _onAmbientLightUpdate: function(event: Event) {
    this.props.onAmbientLightUpdate && this.props.onAmbientLightUpdate(event.nativeEvent.ambientLightInfo)
  },

  async findCollisionsWithRayAsync(from, to, closest, viroTag) {
    return await NativeModules.VRTSceneModule.findCollisionsWithRayAsync(findNodeHandle(this), from, to, closest, viroTag);
  },

  async findCollisionsWithShapeAsync(from, to, shapeString, shapeParam, viroTag) {
    return await NativeModules.VRTSceneModule.findCollisionsWithShapeAsync(findNodeHandle(this), from, to, shapeString, shapeParam, viroTag);
  },

  async getCameraPositionAsync() {
    return await ViroCameraModule.getCameraPosition(findNodeHandle(this));
  },

  async performARHitTestWithRay(ray) {
    return await NativeModules.VRTARSceneModule.performARHitTestWithRay(findNodeHandle(this), ray);
  },

  async performARHitTestWithPosition(position) {
    return await NativeModules.VRTARSceneModule.performARHitTestWithPosition(findNodeHandle(this), position);
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

  async getCameraPositionAsync() {
    return await ViroCameraModule.getCameraPosition(findNodeHandle(this));
  },

  getChildContext: function() {
    return {
      cameraDidMount: function(camera) {
        if (camera.props.active) {
          ViroCameraModule.setSceneCamera(findNodeHandle(this), findNodeHandle(camera));
        }
      }.bind(this),
      cameraWillUnmount: function(camera) {
        if (camera.props.active) {
          ViroCameraModule.removeSceneCamera(findNodeHandle(this), findNodeHandle(camera));
        }
      }.bind(this),
      cameraWillReceiveProps: function(camera, nextProps) {
        if (nextProps.active) {
          ViroCameraModule.setSceneCamera(findNodeHandle(this), findNodeHandle(camera));
        }
        else {
          ViroCameraModule.removeSceneCamera(findNodeHandle(this), findNodeHandle(camera));
        }
      }.bind(this),
    };
  },

  render: function() {
    let timeToFuse = undefined;
    if (this.props.onFuse != undefined && typeof this.props.onFuse === 'object'){
        timeToFuse = this.props.onFuse.timeToFuse;
    }

    return (
      <VRTARScene
        {...this.props}
        canHover={this.props.onHover != undefined}
        canClick={this.props.onClick != undefined || this.props.onClickState != undefined}
        canTouch={this.props.onTouch != undefined}
        canScroll={this.props.onScroll != undefined}
        canSwipe={this.props.onSwipe != undefined}
        canDrag={this.props.onDrag != undefined}
        canPinch={this.props.onPinch != undefined}
        canRotate={this.props.onRotate != undefined}
        canFuse={this.props.onFuse != undefined}
        onHoverViro={this._onHover}
        onClickViro={this._onClickState}
        onTouchViro={this._onTouch}
        onScrollViro={this._onScroll}
        onSwipeViro={this._onSwipe}
        onDragViro={this._onDrag}
        onPinchViro={this._onPinch}
        onRotateViro={this._onRotate}
        onFuseViro={this._onFuse}
        onPlatformUpdateViro={this._onPlatformUpdate}
        onTrackingInitializedViro={this._onTrackingInitialized}
        onAmbientLightUpdateViro={this._onAmbientLightUpdate}
        timeToFuse={timeToFuse}
        />
    );
  },
});

ViroARScene.childContextTypes = {
  cameraDidMount: React.PropTypes.func,
  cameraWillUnmount: React.PropTypes.func,
  cameraWillReceiveProps: React.PropTypes.func,
};

var VRTARScene = requireNativeComponent(
  'VRTARScene', ViroARScene, {
      nativeOnly: {
          canHover: true,
          canClick: true,
          canTouch: true,
          canScroll: true,
          canSwipe: true,
          canDrag: true,
          canPinch: true,
          canRotate: true,
          canFuse: true,
          onHoverViro: true,
          onClickViro: true,
          onTouchViro: true,
          onScrollViro: true,
          onSwipeViro: true,
          onDragViro:true,
          onPinchViro:true,
          onRotateViro:true,
          onFuseViro:true,
          onPlatformUpdateViro: true,
          onTrackingInitializedViro: true,
          onAmbientLightUpdateViro: true,
          timeToFuse:true,
      }
  }
);

module.exports = ViroARScene;
