/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import { requireNativeComponent, findNodeHandle, View } from 'react-native';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
import React, { Component } from 'react';
import PropTypes from 'prop-types';
import { checkMisnamedProps } from '../Utilities/ViroProps';

var NativeModules = require('react-native').NativeModules;
var createReactClass = require('create-react-class');
var ViroConstants = require('../ViroConstants');

var ViroARScene = createReactClass({
  propTypes: {
    ...View.propTypes,
    displayPointCloud: PropTypes.oneOfType([
      PropTypes.shape({
        imageSource : PropTypes.oneOfType([
            PropTypes.shape({
                uri: PropTypes.string,
            }),
            PropTypes.number
          ]),
        imageScale: PropTypes.arrayOf(PropTypes.number),
        maxPoints : PropTypes.number,
      }),
      PropTypes.bool,
    ]),
    ignoreEventHandling: PropTypes.bool,
    anchorDetectionTypes: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    dragType: PropTypes.oneOf(["FixedDistance", "FixedDistanceOrigin", "FixedToWorld", "FixedToPlane"]),
    dragPlane: PropTypes.shape({
      planePoint : PropTypes.arrayOf(PropTypes.number),
      planeNormal : PropTypes.arrayOf(PropTypes.number),
      maxDistance : PropTypes.number
    }),
    onHover: PropTypes.func,
    onClick: PropTypes.func,
    onClickState: PropTypes.func,
    onTouch: PropTypes.func,
    onScroll: PropTypes.func,
    onSwipe: PropTypes.func,
    onDrag: PropTypes.func,
    onPinch: PropTypes.func,
    onRotate: PropTypes.func,
    onCameraARHitTest: PropTypes.func,
    onARPointCloudUpdate: PropTypes.func,
    onCameraTransformUpdate: PropTypes.func,
    onFuse: PropTypes.oneOfType([
      PropTypes.shape({
        callback: PropTypes.func.isRequired,
        timeToFuse: PropTypes.number
      }),
      PropTypes.func
    ]),
    onTrackingUpdated: PropTypes.func,
    onPlatformUpdate: PropTypes.func,
    onAmbientLightUpdate: PropTypes.func,
    onAnchorFound: PropTypes.func,
    onAnchorUpdated: PropTypes.func,
    onAnchorRemoved: PropTypes.func,
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

    /**
     * ##### DEPRECATION WARNING - this prop may be removed in future releases #####
     */
    onTrackingInitialized: PropTypes.func,
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

  _onCameraARHitTest: function(event: Event) {
    var hitTestEventObj = {
      hitTestResults: event.nativeEvent.hitTestResults,
      cameraOrientation: {
        position: [event.nativeEvent.cameraOrientation[0], event.nativeEvent.cameraOrientation[1], event.nativeEvent.cameraOrientation[2]],
        rotation: [event.nativeEvent.cameraOrientation[3], event.nativeEvent.cameraOrientation[4], event.nativeEvent.cameraOrientation[5]],
        forward: [event.nativeEvent.cameraOrientation[6], event.nativeEvent.cameraOrientation[7], event.nativeEvent.cameraOrientation[8]],
        up: [event.nativeEvent.cameraOrientation[9], event.nativeEvent.cameraOrientation[10], event.nativeEvent.cameraOrientation[11]]
      }
    };
    this.props.onCameraARHitTest && this.props.onCameraARHitTest(hitTestEventObj);
  },

  _onARPointCloudUpdate: function(event: Event) {
    this.props.onARPointCloudUpdate && this.props.onARPointCloudUpdate(event.nativeEvent.pointCloud);
  },

  _onCameraTransformUpdate: function(event: Event) {
    var cameraTransform = {
      // ** DEPRECATION WARNING ** The cameraTransform key will be deprecated in a future release, 
      cameraTransform: {
        position: [event.nativeEvent.cameraTransform[0], event.nativeEvent.cameraTransform[1], event.nativeEvent.cameraTransform[2]],
        rotation: [event.nativeEvent.cameraTransform[3], event.nativeEvent.cameraTransform[4], event.nativeEvent.cameraTransform[5]],
        forward: [event.nativeEvent.cameraTransform[6], event.nativeEvent.cameraTransform[7], event.nativeEvent.cameraTransform[8]],
        up: [event.nativeEvent.cameraTransform[9], event.nativeEvent.cameraTransform[10], event.nativeEvent.cameraTransform[11]]
      },
      position: [event.nativeEvent.cameraTransform[0], event.nativeEvent.cameraTransform[1], event.nativeEvent.cameraTransform[2]],
      rotation: [event.nativeEvent.cameraTransform[3], event.nativeEvent.cameraTransform[4], event.nativeEvent.cameraTransform[5]],
      forward: [event.nativeEvent.cameraTransform[6], event.nativeEvent.cameraTransform[7], event.nativeEvent.cameraTransform[8]],
      up: [event.nativeEvent.cameraTransform[9], event.nativeEvent.cameraTransform[10], event.nativeEvent.cameraTransform[11]]
    };
    this.props.onCameraTransformUpdate && this.props.onCameraTransformUpdate(cameraTransform);
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

  // TODO VIRO-3172: Remove in favor of deprecating onTrackingInitialized
  componentDidMount :function(){
    this.onTrackingFirstInitialized = false;
  },

  _onTrackingUpdated: function(event: Event){
    if (this.props.onTrackingUpdated){
      this.props.onTrackingUpdated(event.nativeEvent.state, event.nativeEvent.reason);
    }

    // TODO VIRO-3172: Remove in favor of deprecating onTrackingInitialized
    if ((event.nativeEvent.state == ViroConstants.TRACKING_LIMITED ||
         event.nativeEvent.state == ViroConstants.TRACKING_NORMAL) &&
         !this.onTrackingFirstInitialized) {
      this.onTrackingFirstInitialized = true;
      if (this.props.onTrackingInitialized){
        this.props.onTrackingInitialized();
      }
    }
  },

  /**
   * ##### DEPRECATION WARNING - this prop may be removed in future releases #####
   */
  _onTrackingInitialized: function(event: Event) {
    this.props.onTrackingInitialized && this.props.onTrackingInitialized();
  },

  /*
   Gives constant estimates of the ambient light as detected by the camera.

   Returns object w/ "intensity" and "color" keys
   */
  _onAmbientLightUpdate: function(event: Event) {
    this.props.onAmbientLightUpdate && this.props.onAmbientLightUpdate(event.nativeEvent.ambientLightInfo)
  },

  _onAnchorFound: function(event: Event) {
    this.props.onAnchorFound && this.props.onAnchorFound(event.nativeEvent.anchor);
  },

  _onAnchorUpdated: function(event: Event) {
    this.props.onAnchorUpdated && this.props.onAnchorUpdated(event.nativeEvent.anchor);
  },

  _onAnchorRemoved: function(event: Event) {
    this.props.onAnchorRemoved && this.props.onAnchorRemoved(event.nativeEvent.anchor);
  },

  async findCollisionsWithRayAsync(from, to, closest, viroTag) {
    return await NativeModules.VRTSceneModule.findCollisionsWithRayAsync(findNodeHandle(this), from, to, closest, viroTag);
  },

  async findCollisionsWithShapeAsync(from, to, shapeString, shapeParam, viroTag) {
    return await NativeModules.VRTSceneModule.findCollisionsWithShapeAsync(findNodeHandle(this), from, to, shapeString, shapeParam, viroTag);
  },

  async performARHitTestWithRay(ray) {
    return await NativeModules.VRTARSceneModule.performARHitTestWithRay(findNodeHandle(this), ray);
  },

  async performARHitTestWithWorldPoints(origin, destination) {
    return await NativeModules.VRTARSceneModule.performARHitTestWithRay(findNodeHandle(this), origin, destination);
  },

  async performARHitTestWithPosition(position) {
    return await NativeModules.VRTARSceneModule.performARHitTestWithPosition(findNodeHandle(this), position);
  },

  async performARHitTestWithPoint(x, y) {
    return await NativeModules.VRTARSceneModule.performARHitTestWithPoint(findNodeHandle(this), x, y);
  },

  /**
   * ##### DEPRECATION WARNING - this prop may be removed in future releases #####
   */
  async getCameraPositionAsync() {
    console.warn("[Viro] ViroARScene.getCameraPositionAsync has been DEPRECATED. Please use getCameraOrientationAsync instead.");
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

  async getCameraPositionAsync() {
    return await ViroCameraModule.getCameraPosition(findNodeHandle(this));
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
      cameraDidUpdate: function(camera, active) {
        if (active) {
          NativeModules.VRTCameraModule.setSceneCamera(findNodeHandle(this), findNodeHandle(camera));
        }
        else {
          NativeModules.VRTCameraModule.removeSceneCamera(findNodeHandle(this), findNodeHandle(camera));
        }
      }.bind(this),
    };
  },

  render: function() {

    // Uncomment this line to check for misnamed props
    //checkMisnamedProps("ViroARScene", this.props);

    // Since anchorDetectionTypes can be either a string or an array, convert the string to a 1-element array.
    let anchorDetectionTypes = typeof this.props.anchorDetectionTypes === 'string' ?
        new Array(this.props.anchorDetectionTypes) : this.props.anchorDetectionTypes;

    let timeToFuse = undefined;
    if (this.props.onFuse != undefined && typeof this.props.onFuse === 'object'){
        timeToFuse = this.props.onFuse.timeToFuse;
    }

    let displayPointCloud = false;
    let pointCloudImage = undefined;
    let pointCloudScale = undefined;
    let pointCloudMaxPoints = undefined;
    // parse out displayPointCloud prop
    if (this.props.displayPointCloud) {
      displayPointCloud = true;
      pointCloudImage = resolveAssetSource(this.props.displayPointCloud.imageSource);
      pointCloudScale = this.props.displayPointCloud.imageScale;
      pointCloudMaxPoints = this.props.displayPointCloud.maxPoints;
    }

    if (this.props.onTrackingInitialized && !this.onTrackingFirstInitialized){
      console.warn("[Viro] ViroARScene.onTrackingInitialized() has been DEPRECATED. Please use onTrackingUpdated() instead.");
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
        canCameraARHitTest={this.props.onCameraARHitTest != undefined}
        canARPointCloudUpdate={this.props.onARPointCloudUpdate != undefined}
        canCameraTransformUpdate={this.props.onCameraTransformUpdate != undefined}
        onHoverViro={this._onHover}
        onClickViro={this._onClickState}
        onTouchViro={this._onTouch}
        onScrollViro={this._onScroll}
        onSwipeViro={this._onSwipe}
        onDragViro={this._onDrag}
        onPinchViro={this._onPinch}
        onRotateViro={this._onRotate}
        onFuseViro={this._onFuse}
        onCameraARHitTestViro={this._onCameraARHitTest}
        onARPointCloudUpdateViro={this._onARPointCloudUpdate}
        onCameraTransformUpdateViro={this._onCameraTransformUpdate}
        onPlatformUpdateViro={this._onPlatformUpdate}
        onTrackingUpdatedViro={this._onTrackingUpdated}
        onAmbientLightUpdateViro={this._onAmbientLightUpdate}
        onAnchorFoundViro={this._onAnchorFound}
        onAnchorUpdatedViro={this._onAnchorUpdated}
        onAnchorRemovedViro={this._onAnchorRemoved}
        timeToFuse={timeToFuse}
        anchorDetectionTypes={anchorDetectionTypes}
        displayPointCloud={displayPointCloud}
        pointCloudImage={pointCloudImage}
        pointCloudScale={pointCloudScale}
        pointCloudMaxPoints={pointCloudMaxPoints}
        />
    );
  },
});

ViroARScene.childContextTypes = {
  cameraDidMount: PropTypes.func,
  cameraWillUnmount: PropTypes.func,
  cameraDidUpdate: PropTypes.func,
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
          canCameraARHitTest: true,
          canARPointCloudUpdate: true,
          canCameraTransformUpdate: true,
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
          onTrackingUpdatedViro:true,
          onAmbientLightUpdateViro: true,
          onAnchorFoundViro: true,
          onAnchorUpdatedViro: true,
          onAnchorRemovedViro:true,
          onCameraARHitTestViro: true,
          onARPointCloudUpdateViro: true,
          onCameraTransformUpdateViro: true,
          timeToFuse:true,
          pointCloudImage:true,
          pointCloudScale:true,
          pointCloudMaxPoints:true,
      }
  }
);

module.exports = ViroARScene;
