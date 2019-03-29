/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroAnimationPropTypes
 * @flow
 */

 'use strict';

import React from 'react';
import PropTypes from 'prop-types';

var createStrictShapeTypeChecker = require('react-native/Libraries/DeprecatedPropTypes/deprecatedCreateStrictShapeTypeChecker');
var ColorPropType = require('react-native/Libraries/DeprecatedPropTypes/DeprecatedColorPropType');

var ViroAnimationPropTypes = {
  duration: PropTypes.number.isRequired,
  delay: PropTypes.number,
  easing: PropTypes.string,
  properties: createStrictShapeTypeChecker(
      {
        positionX: PropTypes.oneOfType([
                   PropTypes.number,
                   PropTypes.string]),

        positionY: PropTypes.oneOfType([
                   PropTypes.number,
                   PropTypes.string]),

        positionZ: PropTypes.oneOfType([
                   PropTypes.number,
                   PropTypes.string]),

           scaleX: PropTypes.oneOfType([
                   PropTypes.number,
                   PropTypes.string]),

           scaleY: PropTypes.oneOfType([
                   PropTypes.number,
                   PropTypes.string]),

           scaleZ: PropTypes.oneOfType([
                   PropTypes.number,
                   PropTypes.string]),

          rotateX: PropTypes.oneOfType([
                   PropTypes.number,
                   PropTypes.string]),

          rotateY: PropTypes.oneOfType([
                     PropTypes.number,
                     PropTypes.string]),

          rotateZ: PropTypes.oneOfType([
                   PropTypes.number,
                   PropTypes.string]),

        translateX: PropTypes.oneOfType([
                   PropTypes.number,
                   PropTypes.string]),
        translateY: PropTypes.oneOfType([
                   PropTypes.number,
                   PropTypes.string]),

        translateZ: PropTypes.oneOfType([
                   PropTypes.number,
                   PropTypes.string]),

        opacity:   PropTypes.oneOfType([
                   PropTypes.number,
                   PropTypes.string]),
        color:     ColorPropType,
        material:  PropTypes.string,
      }
    ).isRequired,
};

module.exports = ViroAnimationPropTypes;
