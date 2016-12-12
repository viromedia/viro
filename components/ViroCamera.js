/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroCamera
 * @flow
 */
'use strict';

import { requireNativeComponent, View } from 'react-native';

var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var NativeModules = require('NativeModules');
var PropTypes = require('react/lib/ReactPropTypes');
var React = require('React');
var StyleSheet = require('StyleSheet');

var ViroCamera = React.createClass({
  mixins: [NativeMethodsMixin],

  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
  },

  render: function() {
    return (
      <VRTCamera {...this.props} />
    );
  }
});

var VRTCamera = requireNativeComponent(
  'VRTCamera', ViroCamera
);

module.exports = ViroCamera;
