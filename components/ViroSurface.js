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

    onHover: React.PropTypes.func,
    onClick: React.PropTypes.func,
    onClickState: React.PropTypes.func,
    onTouch: React.PropTypes.func,
    onScroll: React.PropTypes.func,
    onSwipe: React.PropTypes.func,
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
    this.props.onTouch && this.props.onTouch(event.nativeEvent.source, event.nativeEvent.touchState, event.nativeEvent.touchPos);
  },

  _onScroll: function(event: Event) {
      this.props.onScroll && this.props.onScroll(event.nativeEvent.source, event.nativeEvent.scrollPos);
  },

  _onSwipe: function(event: Event) {
      this.props.onSwipe && this.props.onSwipe(event.nativeEvent.source, event.nativeEvent.swipeState);
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
    nativeProps.onHoverViro = this._onHover;
    nativeProps.onClickViro = this._onClickState;
    nativeProps.onTouchViro = this._onTouch;
    nativeProps.onScrollViro = this._onScroll;
    nativeProps.onSwipeViro = this._onSwipe;
    nativeProps.canHover = this.props.onHover != undefined;
    nativeProps.canClick = this.props.onClick != undefined || this.props.onClickState != undefined;
    nativeProps.canTouch = this.props.onTouch != undefined;
    nativeProps.canScroll = this.props.onScroll != undefined;
    nativeProps.canSwipe = this.props.onSwipe != undefined;

    return (
      <VRTSurface {...nativeProps}/>
    );
  }
});

var VRTSurface = requireNativeComponent(
  'VRTSurface', ViroSurface, {
    nativeOnly: {
            canHover: true,
            canClick: true,
            canTouch: true,
            canScroll: true,
            canSwipe: true,
            onHoverViro:true,
            onClickViro:true,
            onTouchViro:true,
            onScrollViro:true,
            onSwipeViro:true}
  }
);

module.exports = ViroSurface;
