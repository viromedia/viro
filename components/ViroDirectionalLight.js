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

import { requireNativeComponent, View, StyleSheet, Platform } from 'react-native';
import React, { Component } from 'react';
var NativeModules = require('react-native').NativeModules;
var createReactClass = require('create-react-class');
import PropTypes from 'prop-types';
var ColorPropType = require('react-native').ColorPropType;
import { checkMisnamedProps } from './Utilities/ViroProps';

/**
 * Used to render a ViroDirectionalLight
 */
var ViroDirectionalLight = createReactClass({
  propTypes: {
    ...View.propTypes,
    color: ColorPropType,
    intensity: PropTypes.number,
    temperature: PropTypes.number,
    direction: PropTypes.arrayOf(PropTypes.number).isRequired,
    influenceBitMask: PropTypes.number,

    // Shadow Properties
    castsShadow: PropTypes.bool,
    shadowOpacity: PropTypes.number,
    shadowOrthographicSize: PropTypes.number,
    shadowOrthographicPosition: PropTypes.arrayOf(PropTypes.number),
    shadowMapSize: PropTypes.number,
    shadowBias: PropTypes.number,
    shadowNearZ: PropTypes.number,
    shadowFarZ: PropTypes.number,
  },

  setNativeProps: function(nativeProps) {
    this._component.setNativeProps(nativeProps);
  },

  render: function() {
    // Uncomment this line to check for misnamed props
    //checkMisnamedProps("ViroDirectionalLight", this.props);

    let nativeProps = Object.assign({}, this.props);
    nativeProps.style=[this.props.style];
    nativeProps.color = this.props.color;
    nativeProps.ref = component => {this._component = component; };

    return (
      <VRTDirectionalLight {...nativeProps} />
    );
  }
});

var VRTDirectionalLight = requireNativeComponent(
  'VRTDirectionalLight',
  ViroDirectionalLight
);

module.exports = ViroDirectionalLight;
