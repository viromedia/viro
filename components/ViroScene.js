/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import { requireNativeComponent, View } from 'react-native';
import React, { Component } from 'react';
var PropTypes = require('react/lib/ReactPropTypes');


var ViroScene = React.createClass({
  propTypes: {
    ...View.propTypes,
    /**
     * Flag for displaying the reticle. True by default.
     */
    reticleEnabled:PropTypes.bool,

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
    return (
        <VRTScene
            {...this.props}
            canGaze={this.props.onGaze != undefined}
            canTap={this.props.onTap != undefined}
            onTapViro={this._onTap}
            onGazeViro={this._onGaze}/>
    );
  }
});

var VRTScene = requireNativeComponent(
    'VRTScene', ViroScene, {
        nativeOnly: {canTap: true, canGaze: true }
    }
);

module.exports = ViroScene;
