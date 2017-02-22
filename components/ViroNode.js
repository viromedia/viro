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
var PropTypes = require('react/lib/ReactPropTypes');

/**
 * Absolute container for Viro Controls
 */
var ViroNode = React.createClass({
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
    // Since transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;
    return (
      <VRTViewContainer
        {...this.props}
        transformBehaviors={transformBehaviors}
        canHover={this.props.onHover != undefined}
        canClick={this.props.onClick != undefined || this.props.onClickState != undefined}
        canTouch={this.props.onTouch != undefined}
        canScroll={this.props.onScroll != undefined}
        canSwipe={this.props.onSwipe != undefined}
        onHoverViro={this._onHover}
        onClickViro={this._onClickState}
        onTouchViro={this._onTouch}
        onScrollViro={this._onScroll}
        onSwipeViro={this._onSwipe}
        />
    );
  }
});


var VRTViewContainer = requireNativeComponent(
  'VRTViewContainer', ViroNode, {
    nativeOnly: {
            materials: [],
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


module.exports = ViroNode;
