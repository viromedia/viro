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

var createStrictShapeTypeChecker = require('react-native/Libraries/Utilities/createStrictShapeTypeChecker');
var ReactPropTypes = require('react/lib/ReactPropTypes');
var ColorPropType = require('react-native').ColorPropType;

var ViroAnimationPropTypes = {
  duration: ReactPropTypes.number.isRequired,
  delay: ReactPropTypes.number,
  easing: ReactPropTypes.string,
  properties: createStrictShapeTypeChecker(
      {
        positionX: ReactPropTypes.oneOfType([
                   ReactPropTypes.number,
                   ReactPropTypes.string]),

        positionY: ReactPropTypes.oneOfType([
                   ReactPropTypes.number,
                   ReactPropTypes.string]),

        positionZ: ReactPropTypes.oneOfType([
                   ReactPropTypes.number,
                   ReactPropTypes.string]),

           scaleX: ReactPropTypes.oneOfType([
                   ReactPropTypes.number,
                   ReactPropTypes.string]),

           scaleY: ReactPropTypes.oneOfType([
                   ReactPropTypes.number,
                   ReactPropTypes.string]),

           scaleZ: ReactPropTypes.oneOfType([
                   ReactPropTypes.number,
                   ReactPropTypes.string]),

          rotateX: ReactPropTypes.oneOfType([
                   ReactPropTypes.number,
                   ReactPropTypes.string]),

          rotateY: ReactPropTypes.oneOfType([
                     ReactPropTypes.number,
                     ReactPropTypes.string]),

          rotateZ: ReactPropTypes.oneOfType([
                   ReactPropTypes.number,
                   ReactPropTypes.string]),

        translateX: ReactPropTypes.oneOfType([
                   ReactPropTypes.number,
                   ReactPropTypes.string]),
        translateY: ReactPropTypes.oneOfType([
                   ReactPropTypes.number,
                   ReactPropTypes.string]),

        translateZ: ReactPropTypes.oneOfType([
                   ReactPropTypes.number,
                   ReactPropTypes.string]),

        opacity:   ReactPropTypes.oneOfType([
                   ReactPropTypes.number,
                   ReactPropTypes.string]),
        color:     ColorPropType,
      }
    ).isRequired,
};

module.exports = ViroAnimationPropTypes;
