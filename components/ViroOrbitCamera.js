/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroOrbitCamera
 * @flow
 */
'use strict';

import { requireNativeComponent, View } from 'react-native';

var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var NativeModules = require('NativeModules');
var PropTypes = require('react/lib/ReactPropTypes');
var React = require('React');
var StyleSheet = require('StyleSheet');

var ViroOrbitCamera = React.createClass({
  mixins: [NativeMethodsMixin],

  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    focalPoint: PropTypes.arrayOf(PropTypes.number),
  },

  render: function() {
    return (
      <VRTOrbitCamera {...this.props} />
    );
  }
});

var VRTOrbitCamera = requireNativeComponent(
  'VRTOrbitCamera', ViroOrbitCamera
);

module.exports = ViroOrbitCamera;
