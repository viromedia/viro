/**
 * Copyright (c) 2016-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule CubeMapPropType
 * @flow
 */
'use strict';

import React from 'react';
var ReactPropTypes = React.PropTypes;

// Reflective textures are cube maps(nx, px, ny, py, nz, pz), which is
// left(negative x), right(positive x), down(neg y), up(pos y), forward(neg z), backward(pos z)

var CubeMapPropType = ReactPropTypes.shape(
    {
      // Opaque type returned by require('./image.jpg')
      nx: ReactPropTypes.oneOfType([
        ReactPropTypes.shape({
          uri: ReactPropTypes.string,
        }),
        ReactPropTypes.number,
      ]).isRequired,
      px: ReactPropTypes.oneOfType([
        ReactPropTypes.shape({
          uri: ReactPropTypes.string,
        }),
        ReactPropTypes.number,
      ]).isRequired,
      ny: ReactPropTypes.oneOfType([
        ReactPropTypes.shape({
          uri: ReactPropTypes.string,
        }),
        ReactPropTypes.number,
      ]).isRequired,
      py: ReactPropTypes.oneOfType([
        ReactPropTypes.shape({
          uri: ReactPropTypes.string,
        }),
        ReactPropTypes.number,
      ]).isRequired,
      nz: ReactPropTypes.oneOfType([
        ReactPropTypes.shape({
          uri: ReactPropTypes.string,
        }),
        ReactPropTypes.number,
      ]).isRequired,
      pz: ReactPropTypes.oneOfType([
        ReactPropTypes.shape({
          uri: ReactPropTypes.string,
        }),
        ReactPropTypes.number,
      ]).isRequired,
    }
);

module.exports = CubeMapPropType;
