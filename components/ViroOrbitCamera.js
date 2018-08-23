/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroOrbitCamera
 * @flow
 */
'use strict';

import { requireNativeComponent, View, StyleSheet } from 'react-native';
import React, { Component } from 'react';
import PropTypes from 'prop-types';
var createReactClass = require('create-react-class');
import { checkMisnamedProps } from './Utilities/ViroProps';

var ViroOrbitCamera = createReactClass({
  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    focalPoint: PropTypes.arrayOf(PropTypes.number),
    active: PropTypes.bool.isRequired,
    animation: PropTypes.shape({
      name: PropTypes.string,
      delay: PropTypes.number,
      loop: PropTypes.bool,
      onStart: PropTypes.func,
      onFinish: PropTypes.func,
      run: PropTypes.bool,
      interruptible: PropTypes.bool,
    }),
    fieldOfView: PropTypes.number,

  },

  componentDidMount() {
    this.context.cameraDidMount(this);
  },

  componentWillUnmount() {
    this.context.cameraWillUnmount(this);
  },

  componentDidUpdate(prevProps, prevState) {
    if(prevProps.active != this.props.active) {
      this.context.cameraDidUpdate(this, this.props.active);
    }
  },

  setNativeProps: function(nativeProps) {
    this._component.setNativeProps(nativeProps);
  },

  render: function() {
    // Uncomment this line to check for misnamed props
    //checkMisnamedProps("ViroOrbitCamera", this.props);

    return (
      <VRTOrbitCamera
        ref={ component => {this._component = component; }}
        {...this.props}
      />
    );
  },
});

ViroOrbitCamera.contextTypes = {
  cameraDidMount: PropTypes.func,
  cameraWillUnmount: PropTypes.func,
  cameraDidUpdate: PropTypes.func,
};

var VRTOrbitCamera = requireNativeComponent(
  'VRTOrbitCamera',
  ViroOrbitCamera, {
    nativeOnly: {
      scale:[1,1,1],
      materials:[],
      visible: true,
      canHover: true,
      canClick: true,
      canTouch: true,
      canScroll: true,
      canSwipe: true,
      canDrag: true,
      canPinch: true,
      canRotate: true,
      onPinchViro: true,
      onRotateViro: true,
      onHoverViro:true,
      onClickViro:true,
      onTouchViro:true,
      onScrollViro:true,
      onSwipeViro:true,
      onDragViro:true,
      transformBehaviors:true,
      canFuse: true,
      onFuseViro:true,
      timeToFuse: true,
      viroTag: true,
      scalePivot: true,
      rotationPivot: true,
      canCollide:true,
      onCollisionViro:true,
      onNativeTransformDelegateViro:true,
      hasTransformDelegate:true,
      physicsBody:true,
      dragType: true,
      dragPlane:true,
      animation:true,
      ignoreEventHandling: true,
      renderingOrder:true,
    }
});

module.exports = ViroOrbitCamera;
