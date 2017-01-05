/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroAnimatedComponent
 * @flow
 */
'use strict';

import { requireNativeComponent, View, StyleSheet } from 'react-native';
import React, { Component } from 'react';
var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var NativeModules = require('react-native').NativeModules;
var PropTypes = require('react/lib/ReactPropTypes');
var findNodeHandle = require('react/lib/findNodeHandle');
var ViroAnimatedComponentManager = require('react-native').NativeModules.VRTAnimatedComponentManager;

var RCT_ANIMATED_COMPONENT_REF = 'animatedcomp';
/**
 * Used to render a ViroAnimatedComponent
 */
var ViroAnimatedComponent = React.createClass({
  mixins: [NativeMethodsMixin],

  propTypes: {
    animation: PropTypes.string,
    delay: PropTypes.number,
    loop: PropTypes.bool,
    onFinish: React.PropTypes.func,
    run: PropTypes.bool,
  },

  getWebViewHandle: function(): any {
    return findNodeHandle(this.refs[RCT_ANIMATED_COMPONENT_REF]);
  },

  startAnimation: function() {
    ViroAnimatedComponentManager.startAnimation(this.getWebViewHandle());
  },

  stopAnimation: function() {
    ViroAnimatedComponentManager.stopAnimation(this.getWebViewHandle());
  },

  render: function() {
    return (
      <VROAnimatedComponent
        ref={RCT_ANIMATED_COMPONENT_REF}
        {...this.props}
        style={[this.props.style]}
      />
    );
  }
});


var VROAnimatedComponent = requireNativeComponent(
  'VRTAnimatedComponent',
  ViroAnimatedComponent
);

module.exports = ViroAnimatedComponent;
