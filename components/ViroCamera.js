/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroCamera
 * @flow
 */
'use strict';

import { requireNativeComponent, View, StyleSheet } from 'react-native';
import React, { Component } from 'react';
var PropTypes = require('react/lib/ReactPropTypes');

var ViroCamera = React.createClass({
  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    active: PropTypes.bool.isRequired,
  },

  componentDidMount() {
    this.context.cameraDidMount(this);
  },

  componentWillUnmount() {
    this.context.cameraWillUnmount(this);
  },

  componentWillReceiveProps(nextProps) {
    this.context.cameraWillReceiveProps(this, nextProps);
  },

  render: function() {
    return (
      <VRTCamera
        {...this.props}
      />
    );
  }
});

ViroCamera.contextTypes = {
  cameraDidMount: React.PropTypes.func,
  cameraWillUnmount: React.PropTypes.func,
  cameraWillReceiveProps: React.PropTypes.func,
};

var VRTCamera = requireNativeComponent(
  'VRTCamera',
  ViroCamera, {
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
                onHoverViro:true,
                onClickViro:true,
                onTouchViro:true,
                onScrollViro:true,
                onSwipeViro:true,
                onDragViro:true,
                transformBehaviors:true,
      }
});

module.exports = ViroCamera;
