/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroOmniLight
 * @flow
 */
'use strict';

import { requireNativeComponent, View, StyleSheet, Platform } from 'react-native';
import React, { Component } from 'react';
import normalizeColor from "react-native/Libraries/StyleSheet/normalizeColor"
var NativeModules = require('react-native').NativeModules;
var PropTypes = require('react/lib/ReactPropTypes');
var ColorPropType = require('react-native').ColorPropType;

/**
 * Used to render a ViroOmniLight
 */
var ViroOmniLight = React.createClass({
  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    color: ColorPropType,
    intensity: PropTypes.number,
    attenuationStartDistance: PropTypes.number,
    attenuationEndDistance: PropTypes.number,
  },

  render: function() {
      let nativeProps = Object.assign({}, this.props);
      nativeProps.style=[this.props.style];
      nativeProps.color=this.props.color;

      return (
        <VRTOmniLight
          {...nativeProps}
        />
      );
  }
});

var VRTOmniLight = requireNativeComponent(
  'VRTOmniLight',
  ViroOmniLight
);

module.exports = ViroOmniLight;
