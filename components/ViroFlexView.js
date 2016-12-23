/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroFlexView
 * @flow
 */
'use strict';

import {
  requireNativeComponent,
  View,
} from 'react-native';

import React from 'react';

var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var NativeModules = require('react-native/Libraries/BatchedBridge/NativeModules');
var PropTypes = require('react/lib/ReactPropTypes');
var StyleSheet = require('react-native/Libraries/StyleSheet/StyleSheet');
var ViroPropTypes = require('./Styles/ViroPropTypes');
var ColorPropType = require('react-native/Libraries/StyleSheet/ColorPropType');

var StyleSheetPropType = require('react-native/Libraries/StyleSheet/StyleSheetPropType');
var ViroFlexViewPropTypes =  Object.assign(Object.create(ViroPropTypes), {
  backgroundColor: ColorPropType,
});
var stylePropType = StyleSheetPropType(ViroFlexViewPropTypes);


/**
 * Used to render a ViroFlexView
 */
var ViroFlexView = React.createClass({
  mixins: [NativeMethodsMixin],

  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    opacity: PropTypes.number,
    width: PropTypes.number,
    height: PropTypes.number,
    style: stylePropType,
    backgroundColor: ColorPropType,
    visible: PropTypes.bool,
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    /**
     * Callback that is called when user gazes on box.
     */
    onGaze: React.PropTypes.func,

    /**
     * Callback that is called when user taps on box.
     */
    onTap: React.PropTypes.func,
  },

  _onGaze: function(event: Event) {
    this.props.onGaze && this.props.onGaze(event.nativeEvent.isGazing);
  },

  render: function() {
    let onGaze = this.props.onGaze ? this._onGaze : undefined;
    // Since materials and transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let materials = typeof this.props.materials === 'string' ? new Array(this.props.materials) : this.props.materials;
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;
    if (this.props.material) {
      console.error('The <ViroFlexView> component takes a `materials` property rather than `material`.');
    }

    let nativeProps = Object.assign({}, this.props);
    nativeProps.materials = materials;
    nativeProps.transformBehaviors = transformBehaviors;

    return (
      <VROFlexView {...nativeProps} />
    );
  }
});


var VROFlexView = requireNativeComponent(
  'VRTFlexView', ViroFlexView, {
    nativeOnly: {onTap: true, onGaze: true}
  }
);


module.exports = ViroFlexView;
