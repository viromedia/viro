/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroARPlane
 */

'use strict';

import { requireNativeComponent, View, StyleSheet, findNodeHandle } from 'react-native';
import React, { Component } from 'react';
var PropTypes = require('react/lib/ReactPropTypes');
var NativeModules = require('react-native').NativeModules;

/**
 * Container for Viro Components anchored to a detected plane.
 */
var ViroARPlane = React.createClass({
  propTypes: {
    ...View.propTypes,
    minHeight: PropTypes.number,
    minWidth: PropTypes.number,
    visible: PropTypes.bool,
    opacity: PropTypes.number,

    onHover: React.PropTypes.func,
    onClick: React.PropTypes.func,
    onClickState: React.PropTypes.func,
    onTouch: React.PropTypes.func,
    onScroll: React.PropTypes.func,
    onSwipe: React.PropTypes.func,
    onDrag: React.PropTypes.func,
    onFuse: PropTypes.oneOfType([
      React.PropTypes.shape({
        callback: React.PropTypes.func.isRequired,
        timeToFuse: PropTypes.number
      }),
      React.PropTypes.func
    ]),
    onCollided: React.PropTypes.func,
    viroTag: PropTypes.string,
    onComponentFound: React.PropTypes.func,
    onComponentUpdated: React.PropTypes.func,
    onComponentRemoved: React.PropTypes.func,
  },

  _onHover: function(event: Event) {
    this.props.onHover && this.props.onHover(event.nativeEvent.isHovering, event.nativeEvent.source);
  },

  _onClick: function(event: Event) {
    this.props.onClick && this.props.onClick(event.nativeEvent.source);
  },

  _onClickState: function(event: Event) {
    this.props.onClickState && this.props.onClickState(event.nativeEvent.clickState, event.nativeEvent.source);
    let CLICKED = 3; // Value representation of Clicked ClickState within EventDelegateJni.
    if (event.nativeEvent.clickState == CLICKED){
        this._onClick(event)
    }
  },

  _onTouch: function(event: Event) {
    this.props.onTouch && this.props.onTouch(event.nativeEvent.touchState, event.nativeEvent.touchPos, event.nativeEvent.source);
  },

  _onScroll: function(event: Event) {
      this.props.onScroll && this.props.onScroll(event.nativeEvent.scrollPos, event.nativeEvent.source);
  },

  _onSwipe: function(event: Event) {
      this.props.onSwipe && this.props.onSwipe(event.nativeEvent.swipeState, event.nativeEvent.source);
  },

  _onDrag: function(event: Event) {
      this.props.onDrag
        && this.props.onDrag(event.nativeEvent.dragToPos, event.nativeEvent.source);
  },

  _onFuse: function(event: Event){
    if (this.props.onFuse){
      if (typeof this.props.onFuse === 'function'){
        this.props.onFuse(event.nativeEvent.source);
      } else if (this.props.onFuse != undefined && this.props.onFuse.callback != undefined){
        this.props.onFuse.callback(event.nativeEvent.source);
      }
    }
  },

  _onCollided: function(event: Event){
    if (this.props.onCollided){
      this.props.onCollided(event.nativeEvent.viroTag, event.nativeEvent.collidedPoint,
                                                           event.nativeEvent.collidedNormal);
    }
  },

  _onComponentFound: function(event: Event) {
    if (this.props.onComponentFound) {
      this.props.onComponentFound(event.nativeEvent.componentFoundMap);
    }
  },

  _onComponentUpdated: function(event: Event) {
    if (this.props.onComponentUpdated) {
      this.props.onComponentUpdated(event.nativeEvent.componentUpdatedMap);
    }
  },

  _onComponentRemoved: function(event: Event) {
    if (this.props.onComponentRemoved) {
      this.props.onComponentRemoved();
    }
  },

  render: function() {

    let timeToFuse = undefined;
    if (this.props.onFuse != undefined && typeof this.props.onFuse === 'object'){
        timeToFuse = this.props.onFuse.timeToFuse;
    }

    return (
      <VRTARPlane
        {...this.props}
        canHover={this.props.onHover != undefined}
        canClick={this.props.onClick != undefined || this.props.onClickState != undefined}
        canTouch={this.props.onTouch != undefined}
        canScroll={this.props.onScroll != undefined}
        canSwipe={this.props.onSwipe != undefined}
        canDrag={this.props.onDrag != undefined}
        canFuse={this.props.onFuse != undefined}
        onHoverViro={this._onHover}
        onClickViro={this._onClickState}
        onTouchViro={this._onTouch}
        onScrollViro={this._onScroll}
        onSwipeViro={this._onSwipe}
        onDragViro={this._onDrag}
        onFuseViro={this._onFuse}
        timeToFuse={timeToFuse}
        canCollide={this.props.onCollided != undefined}
        onCollidedViro={this._onCollided}
        onComponentFoundViro={this._onComponentFound}
        onComponentUpdatedViro={this._onComponentUpdated}
        onComponentRemovedViro={this._onComponentRemoved}
        />
    );
  }
});


var VRTARPlane = requireNativeComponent(
  'VRTARPlane', ViroARPlane, {
    nativeOnly: {
      materials: [],
      canHover: true,
      canClick: true,
      canTouch: true,
      canScroll: true,
      canSwipe: true,
      canDrag: true,
      canFuse: true,
      onHoverViro:true,
      onClickViro:true,
      onTouchViro:true,
      onScrollViro:true,
      onSwipeViro:true,
      onDragViro:true,
      onFuseViro:true,
      timeToFuse:true,
      canCollide:true,
      onCollidedViro:true,
      onComponentFoundViro:true,
      onComponentUpdatedViro:true,
      onComponentRemovedViro:true,
    }
  }
);


module.exports = ViroARPlane;
