/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
'use strict';

import { requireNativeComponent, findNodeHandle, View, StyleSheet, Platform } from 'react-native';
import React, { Component } from 'react';
var NativeModules = require('react-native').NativeModules;
var PropTypes = require('react/lib/ReactPropTypes');
var ViroControllerModule = require('react-native').NativeModules.VRTControllerModule;

var ViroController = React.createClass({

  propTypes: {
    ...View.propTypes,
    onHover: React.PropTypes.func,
    onClick: React.PropTypes.func,
    onClickState: React.PropTypes.func,
    onTouch: React.PropTypes.func,
    onScroll: React.PropTypes.func,
    onSwipe: React.PropTypes.func,
    onControllerStatus: React.PropTypes.func,
    onFuse: React.PropTypes.func,
    reticleVisibility: PropTypes.bool,
    controllerVisibility: PropTypes.bool,
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

  _onControllerStatus: function(event: Event) {
    this.props.onControllerStatus && this.props.onControllerStatus(event.nativeEvent.controllerStatus, event.nativeEvent.source);
  },

  _onFuse: function(event: Event){
    this.props.onFuse && this.props.onFuse(event.nativeEvent.source);
  },

  async getControllerForwardAsync() {
    return await ViroControllerModule.getForwardVectorAsync(findNodeHandle(this));
  },

  render: function() {
      return (
        <VRTController
          {...this.props}
          canClick={this.props.onClick != undefined || this.props.onClickState != undefined}
          canTouch={this.props.onTouch != undefined}
          canScroll={this.props.onScroll != undefined}
          canSwipe={this.props.onSwipe != undefined}
          canGetControllerStatus={this.props.onControllerStatus != undefined}
          canFuse={this.props.onFuse != undefined}
          onClickViro={this._onClickState}
          onTouchViro={this._onTouch}
          onScrollViro={this._onScroll}
          onSwipeViro={this._onSwipe}
          onFuseViro={this._onFuse}
          onControllerStatusViro={this._onControllerStatus}
        />
      );
  }
});

var VRTController = requireNativeComponent(
  'VRTController', ViroController, {
          nativeOnly: {
              canClick: true,
              canTouch: true,
              canScroll: true,
              canSwipe: true,
              canFuse: true,
              canGetControllerStatus: true,
              onClickViro:true,
              onTouchViro:true,
              onScrollViro:true,
              onSwipeViro:true,
              onControllerStatusViro:true,
              onFuseViro:true,
              timeToFuse:true,
            }
      }
);

module.exports = ViroController;
