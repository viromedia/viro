/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroDirectionalLight
 * @flow
 */
'use strict';

import { requireNativeComponent, View, StyleSheet } from 'react-native';
import React, { Component } from 'react';
var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var NativeModules = require('react-native').NativeModules;
var PropTypes = require('react/lib/ReactPropTypes');
var ColorPropType = require('react-native').ColorPropType;


/**
 * Used to render a ViroDirectionalLight
 */
var ViroDirectionalLight = React.createClass({
  mixins: [NativeMethodsMixin],

  propTypes: {
    color: ColorPropType,
    direction: PropTypes.arrayOf(PropTypes.number),
  },

  render: function() {
      return (
        <VRTDirectionalLight
          {...this.props}
          style={[this.props.style]}
        />
      );
  }
});

var VRTDirectionalLight = requireNativeComponent(
  'VRTDirectionalLight',
  ViroDirectionalLight
);

module.exports = ViroDirectionalLight;
