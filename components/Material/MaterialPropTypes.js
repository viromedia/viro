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

var ReactPropTypes = require('react/lib/ReactPropTypes');
var CubeMapPropType = require('./CubeMapPropType');

var MaterialPropTypes = {
  shininess: ReactPropTypes.number,
  fresnelExponent: ReactPropTypes.number,
  lightingModel: ReactPropTypes.oneOf(['Phong', 'Blinn', 'Lambert', 'Constant']),
  writesToDepthBuffer: ReactPropTypes.bool,
  readsFromDepthBuffer: ReactPropTypes.bool,
  transparencyMode: ReactPropTypes.oneOf(['AOne', 'RGBZero']),
  cullMode: ReactPropTypes.oneOf(['None', 'Back', 'Front']),
  diffuseTexture: ReactPropTypes.any,
  specularTexture: ReactPropTypes.any,
  normalTexture: ReactPropTypes.any,
  reflectiveTexture: CubeMapPropType,
  emissionTexture: ReactPropTypes.any,
  transparentTexture: ReactPropTypes.any,
  multiplyTexture: ReactPropTypes.any,
  ambientOcclusionTexture: ReactPropTypes.any,
  selfIlluminationTexture: ReactPropTypes.any,
  diffuseColor: ReactPropTypes.string,
  specularColor: ReactPropTypes.string,
  normalColor: ReactPropTypes.string,
  reflectiveColor: ReactPropTypes.string,
  emissionColor: ReactPropTypes.string,
  transparentColor: ReactPropTypes.string,
  multiplyColor: ReactPropTypes.string,
  ambientOcclusionColor: ReactPropTypes.string,
  selfIlluminationColor: ReactPropTypes.string,
};

module.exports = MaterialPropTypes;
