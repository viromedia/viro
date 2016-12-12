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

import { requireNativeComponent, View, StyleSheet } from 'react-native';

var ViroSceneNavigator = React.createClass({
  propTypes: {
    ...View.propTypes,
    /**
     * Calling vrModeEnabled allows switching to and from VR mode.
     * When set to false, it transitions back to pre-VR (mono) mode.
     * When set to true, we set thie view into a full VR mode.
     * This is set to true by default.
     */
    vrModeEnabled: PropTypes.bool,
  },
  getDefaultProps: function() {
    return {
      vrModeEnabled: true,
    };
  },
  render: function() {
    return (
        <VRTSceneNavigator {...this.props} style={this.props.style, styles.container}/>
    );
  }
});

var styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
});


var VRTSceneNavigator = requireNativeComponent(
    'VRTSceneNavigator', ViroSceneNavigator
);

module.exports = ViroSceneNavigator;
