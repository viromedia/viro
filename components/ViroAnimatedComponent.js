/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroAnimatedComponent
 * @flow
 */
'use strict';

import { requireNativeComponent, View, StyleSheet, Platform } from 'react-native';
import React from 'react';
var PropTypes = require('react/lib/ReactPropTypes');

/**
 * Used to render a ViroAnimatedComponent
 */
var ViroAnimatedComponent = React.createClass({
  propTypes: {
    ...View.propTypes,
    animation: PropTypes.string,
    delay: PropTypes.number,
    loop: PropTypes.bool,
    onStart: React.PropTypes.func,
    onFinish: React.PropTypes.func,
    run: PropTypes.bool,
  },

  _onStart: function(event: Event) {
    this.props.onStart && this.props.onStart();
  },

  _onFinish: function(event: Event) {
    this.props.onFinish && this.props.onFinish();
  },

  render: function() {
    let nativeProps = Object.assign({}, this.props);
    nativeProps.onFinishViro = this._onFinish;
    nativeProps.onStartViro = this._onStart;

    return (
      <VRTAnimatedComponent {...nativeProps} />
    );
  }
});


var VRTAnimatedComponent = requireNativeComponent(
  'VRTAnimatedComponent', ViroAnimatedComponent, {
    nativeOnly: { onStartViro:true, onFinishViro:true }
  }
);

module.exports = ViroAnimatedComponent;
