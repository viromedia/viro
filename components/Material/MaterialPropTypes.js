/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule MaterialPropTypes
 * @flow
 */
'use strict';

import React from 'react';
import PropTypes from 'prop-types';

var CubeMapPropType = require('./CubeMapPropType');
var ColorPropType = require('react-native').ColorPropType;

var MaterialPropTypes = {
  shininess: PropTypes.number,
  fresnelExponent: PropTypes.number,
  lightingModel: PropTypes.oneOf(['Phong', 'Blinn', 'Lambert', 'Constant', 'PBR']),
  writesToDepthBuffer: PropTypes.bool,
  readsFromDepthBuffer: PropTypes.bool,
  cullMode: PropTypes.oneOf(['None', 'Back', 'Front']),
  diffuseTexture: PropTypes.any,
  diffuseIntensity: PropTypes.number,
  specularTexture: PropTypes.any,
  normalTexture: PropTypes.any,
  reflectiveTexture: CubeMapPropType,
  diffuseColor: ColorPropType,
  wrapS: PropTypes.oneOf(['Clamp', 'Repeat', 'Mirror']),
  wrapT: PropTypes.oneOf(['Clamp', 'Repeat', 'Mirror']),
  minificationFilter: PropTypes.oneOf(['Nearest', 'Linear']),
  magnificationFilter: PropTypes.oneOf(['Nearest', 'Linear']),
  mipFilter: PropTypes.oneOf(['Nearest', 'Linear']),
  bloomThreshold: PropTypes.number,
  roughness: PropTypes.number,
  roughnessTexture: PropTypes.any,
  metalness: PropTypes.number,
  metalnessTexture: PropTypes.any,
  ambientOcclusionTexture: PropTypes.any,
};

module.exports = MaterialPropTypes;
