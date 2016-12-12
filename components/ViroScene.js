/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import { requireNativeComponent, View } from 'react-native';

var NativeModules = require('NativeModules');
var PropTypes = require('react/lib/ReactPropTypes');
var React = require('React');

var ViroScene = React.createClass({
  propTypes: {
    ...View.propTypes,
    /**
     * Flag for displaying the reticle. True by default.
     */
    reticleEnabled:PropTypes.bool,
  },
  render: function() {
    return (
        <VRTScene {...this.props} />
    );
  }
});

var VRTScene = requireNativeComponent(
    'VRTScene', ViroScene
);

module.exports = ViroScene;