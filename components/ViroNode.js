/**
 * Copyright (c) 2016-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroNode
 */

'use strict';

import { requireNativeComponent, View, StyleSheet } from 'react-native';
import React, { Component } from 'react';
var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var PropTypes = require('react/lib/ReactPropTypes');

/**
 * Absolute container for Viro Controls
 */
var ViroNode = React.createClass({
  mixins: [NativeMethodsMixin],

  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    visible: PropTypes.bool,
    opacity: PropTypes.number,

    /**
     * Callback that is called when user gazes on the node.
     */
    onGaze: React.PropTypes.func,

    /**
     * Callback that is called when user taps on the node.
     */
    onTap: React.PropTypes.func,
  },

  _onGaze: function(event: Event) {
    this.props.onGaze && this.props.onGaze(event.nativeEvent.isGazing);
  },

  _onTap: function(event: Event) {
    this.props.onTap && this.props.onTap();
  },

  render: function() {
    // Since transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;
    return (
      <VRTViewContainer
        {...this.props}
        transformBehaviors={transformBehaviors}
        canGaze={this.props.onGaze != undefined}
        canTap={this.props.onTap != undefined}
        onTapViro={this._onTap}
        onGazeViro={this._onGaze}/>
    );
  }
});


var VRTViewContainer = requireNativeComponent(
  'VRTViewContainer', ViroNode, {
    nativeOnly: {canTap: true, canGaze: true, materials: [], onTapViro:true, onGazeViro:true}
  }
);


module.exports = ViroNode;
