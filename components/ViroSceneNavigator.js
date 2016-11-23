/**
 * Copyright (c) 2015-present, Viro, Inc.
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
  },
  render: function() {
    return (
        <VRTSceneNavigator {...this.props} style=style={this.props.style, styles.container}/>
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


