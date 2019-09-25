/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
'use strict';

/**
 * Pull in all imports required for this control.
 */
import React, { Component } from 'react';
import {StyleSheet} from 'react-native';
import PropTypes from 'prop-types';

import {
  ViroScene,
  ViroLight,
  Viro360Image,
  ViroImageCard,
  ViroAnimations,
  ViroAnimatedComponent,
  ViroNode,
  ViroSpinner,
  ViroText,
  ViroSphere,
  ViroUtils,
} from 'react-viro';
let polarToCartesian = ViroUtils.polarToCartesian;

/**
 * Render a simple custom control that groups together a ViroSpinner and text.
 */
export default class LoadingSpinner extends Component {
  constructor() {
    super();
  }

  render() {
    var spinnerPosition = polarToCartesian([0, 0, 0]);
    var textPosition = polarToCartesian([1, -25, -40]);
    return (
      <ViroNode {...this.props}>
                {/* NOTE: Additional layer of ViroNode is placed to get around a temporary billboarding bug */}
                <ViroNode position={polarToCartesian([0, 0, 0])} transformBehaviors={["billboard"]}>
                  <ViroSpinner position={spinnerPosition} scale={[.7,.7,.1]} spinnerType='dark' />
                  <ViroText style={styles.spinnerTextStyle} position={textPosition} text="Loading ...." />
                </ViroNode>
      </ViroNode>
    );
  }
}

/**
 * Declare the spinner's text font family, size and color in terms of
 * a spinnerTextStyle.
 */
var styles = StyleSheet.create({
  spinnerTextStyle: {
    fontFamily: 'HelveticaNeue-Medium',
    fontSize: 70,
    color: '#FFFFFF',
  },
});

module.exports = LoadingSpinner;
