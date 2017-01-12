/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroSurface
 * @flow
 */
'use strict';

import { requireNativeComponent, View } from 'react-native';
import React from 'react';

var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var NativeModules = require('react-native').NativeModules;
var PropTypes = require('react/lib/ReactPropTypes');
var StyleSheet = require('react-native/Libraries/StyleSheet/StyleSheet');

var ViroPropTypes = require('./Styles/ViroPropTypes');
var StyleSheetPropType = require('react-native/Libraries/StyleSheet/StyleSheetPropType');
var stylePropType = StyleSheetPropType(ViroPropTypes);

/**
 * Used to render a ViroSurface
 */
var ViroSurface = React.createClass({
  mixins: [NativeMethodsMixin],

  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    opacity: PropTypes.number,
    width: PropTypes.number,
    height: PropTypes.number,
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    visible: PropTypes.bool,
    style: stylePropType,

    /**
     * Callback that is called when user gazes on the ViroSurface.
     */
    onGaze: React.PropTypes.func,

    /**
     * Callback that is called when user taps on the ViroSurface.
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

    if (this.props.material) {
      console.error('The <ViroSurface> component takes a `materials` property rather than `material`.');
    }

    // Since materials and transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let materials = typeof this.props.materials === 'string' ? new Array(this.props.materials) : this.props.materials;
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;

    // Create native props object.
    let nativeProps = Object.assign({}, this.props);
    nativeProps.materials = materials;
    nativeProps.transformBehaviors = transformBehaviors;
    nativeProps.style = [this.props.style];
    nativeProps.onTapViro = this._onTap;
    nativeProps.onGazeViro = this._onGaze;
    nativeProps.canGaze = this.props.onGaze != undefined;
    nativeProps.canTap = this.props.onTap != undefined;

    return (
      <VRTSurface {...nativeProps}/>
    );
  }
});

var VRTSurface = requireNativeComponent(
  'VRTSurface', ViroSurface, {
    nativeOnly: {canTap: true, canGaze: true, onTapViro:true, onGazeViro:true}
  }
);

module.exports = ViroSurface;
