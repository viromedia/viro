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
import PropTypes from 'prop-types';

/**
 * Used to render a ViroAnimatedComponent
 */
var ViroAnimatedComponent = React.createClass({
  propTypes: {
    ...View.propTypes,
    animation: PropTypes.string,
    delay: PropTypes.number,
    loop: PropTypes.bool,
    onStart: PropTypes.func,
    onFinish: PropTypes.func,
    run: PropTypes.bool,
  },

  _onStart: function(event: Event) {
    this.props.onStart && this.props.onStart();
  },

  _onFinish: function(event: Event) {
    this.props.onFinish && this.props.onFinish();
  },

  setNativeProps: function(nativeProps) {
    this._component.setNativeProps(nativeProps);
  },

  render: function() {
    let nativeProps = Object.assign({}, this.props);
    nativeProps.onAnimationFinishViro = this._onFinish;
    nativeProps.onAnimationStartViro = this._onStart;
    nativeProps.ref = component => {this._component = component; };

    return (
      <VRTAnimatedComponent {...nativeProps} />
    );
  }
});


var VRTAnimatedComponent = requireNativeComponent(
  'VRTAnimatedComponent', ViroAnimatedComponent, {
    nativeOnly: { onAnimationStartViro:true, onAnimationFinishViro:true }
  }
);

module.exports = ViroAnimatedComponent;
