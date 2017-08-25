/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroSpotLight
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
 * Used to render a ViroSpotLight
 */
var ViroSpotLight = React.createClass({
  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    color: ColorPropType,
    intensity: PropTypes.number,
    influenceBitMask: PropTypes.number,
    castsShadow: PropTypes.bool,
    direction: PropTypes.arrayOf(PropTypes.number).isRequired,
    attenuationStartDistance: PropTypes.number,
    attenuationEndDistance: PropTypes.number,
    innerAngle: PropTypes.number,
    outerAngle : PropTypes.number,
  },

  render: function() {
      let nativeProps = Object.assign({}, this.props);
      nativeProps.style=[this.props.style];
      nativeProps.color = this.props.color;

      return (
        <VRTSpotLight
          {...nativeProps}
        />
      );
  }
});

var VRTSpotLight = requireNativeComponent(
  'VRTSpotLight',
  ViroSpotLight
);

module.exports = ViroSpotLight;
