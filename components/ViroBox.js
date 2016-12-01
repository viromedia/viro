/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
var NativeModules = require('NativeModules');
var PropTypes = require('react/lib/ReactPropTypes');
var React = require('React');

import { requireNativeComponent, View } from 'react-native';

var ViroBox = React.createClass({
  propTypes: {
  },
  render: function() {
    return (
        <VRTBox {...this.props} />
    );
  }
});

var VRTBox = requireNativeComponent(
    'VRTBox', ViroBox
);

module.exports = ViroBox;
