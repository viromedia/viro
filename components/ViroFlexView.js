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
    onHover: React.PropTypes.func,
    onClick: React.PropTypes.func,
    onClickState: React.PropTypes.func,
    onTouch: React.PropTypes.func,
  },

  _onHover: function(event: Event) {
    this.props.onHover && this.props.onHover(event.nativeEvent.source, event.nativeEvent.isHovering);
  },

  _onClick: function(event: Event) {
    this.props.onClick && this.props.onClick(event.nativeEvent.source);
  },

  _onClickState: function(event: Event) {
    this.props.onClickState && this.props.onClickState(event.nativeEvent.source, event.nativeEvent.clickState);
    let CLICKED = 3; // Value representation of Clicked ClickState within EventDelegateJni.
    if (event.nativeEvent.clickState == CLICKED){
        this._onClick(event)
    }
  },

  _onTouch: function(event: Event) {
    this.props.onTouch && this.props.onTouch(event.nativeEvent.source, event.nativeEvent.touchState);
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
    nativeProps.onHoverViro = this._onHover;
    nativeProps.onClickViro = this._onClickState;
    nativeProps.onTouchViro = this._onTouch;
    nativeProps.canHover = this.props.onHover != undefined;
    nativeProps.canClick = this.props.onClick != undefined || this.props.onClickState != undefined;
    nativeProps.canTouch = this.props.onTouch != undefined;
    return (
      <VROFlexView {...nativeProps} />
    );
  }
});


var VROFlexView = requireNativeComponent(
  'VRTFlexView', ViroFlexView, {
    nativeOnly: {
            canHover: true,
            canClick: true,
            canTouch: true,
            onHoverViro:true,
            onClickViro:true,
            onTouchViro:true}
  }
);


module.exports = ViroFlexView;
