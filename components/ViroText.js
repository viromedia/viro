/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroText
 * @flow
 */
'use strict';

import { requireNativeComponent, View} from 'react-native';
import React, { Component } from 'react';
var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var PropTypes = require('react/lib/ReactPropTypes');
var StyleSheet = require('react-native/Libraries/StyleSheet/StyleSheet');
var ColorPropType = require('react-native').ColorPropType;
var StyleSheetPropType = require('react-native/Libraries/StyleSheet/StyleSheetPropType');
var TextStylePropTypes = require('./Styles/ViroTextPropTypes');

var stylePropType = StyleSheetPropType(TextStylePropTypes);

/**
 * Used to render a ViroText
 */
var ViroText = React.createClass({
  mixins: [NativeMethodsMixin],

  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    text: PropTypes.string,
    color: ColorPropType,
    width: PropTypes.number,
    height: PropTypes.number,
    maxLines: PropTypes.number,
    textClipMode: PropTypes.oneOf(['none, cliptobounds']),
    textLineBreakMode: PropTypes.oneOf(['wordwrap','charwrap','justify','none']),
    visible: PropTypes.bool,
    style: stylePropType,
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    /**
     * Callback that is called when user taps on the text control
     */
    onTap: React.PropTypes.func,
    /**
     * Callback that is called when user gazes on a text control
     */
    onGaze: React.PropTypes.func,
  },

  _onGaze: function(event: Event) {
    this.props.onGaze && this.props.onGaze(event.nativeEvent.isGazing);
  },

  _onTap: function(event: Event) {
    this.props.onTap && this.props.onTap();
  },

  render: function() {
    let onGaze = this.props.onGaze ? this._onGaze : undefined;
    // Since materials and transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;
    return (
      <VRTText
        {...this.props}
        style={[this.props.style]}
        canGaze={this.props.onGaze != undefined}
        canTap={this.props.onTap != undefined}
        onTapViro={this._onTap}
        onGazeViro={this._onGaze}
        transformBehaviors={transformBehaviors}
      />
    );
  }
});

var VRTText = requireNativeComponent(
  'VRTText',
  ViroText, {
    nativeOnly: {canTap: true, canGaze: true, onTapViro:true, onGazeViro:true, scale:[1,1,1], materials:[]}
});

module.exports = ViroText;
