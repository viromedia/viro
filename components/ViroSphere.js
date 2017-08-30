/**
 * Copyright (c) 2016-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroSphere
 */

'use strict';

import React, { Component } from 'react';
import { requireNativeComponent, View, StyleSheet, findNodeHandle } from 'react-native';
var PropTypes = React.PropTypes;
var NativeModules = require('react-native').NativeModules;

/**
 * Used to render a ViroSphere
 */
var ViroSphere = React.createClass({
  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    scalePivot: PropTypes.arrayOf(PropTypes.number),
    rotationPivot: PropTypes.arrayOf(PropTypes.number),
    visible: PropTypes.bool,
    opacity: PropTypes.number,
    widthSegmentCount: PropTypes.number,
    heightSegmentCount: PropTypes.number,
    radius: PropTypes.number,
    facesOutward: PropTypes.bool,
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    lightBitMask : PropTypes.number,
    shadowCastingBitMask : PropTypes.number,
    acceptShadows : PropTypes.bool,
    onTransformUpdate: React.PropTypes.func,
    onHover: React.PropTypes.func,
    onClick: React.PropTypes.func,
    onClickState: React.PropTypes.func,
    onTouch: React.PropTypes.func,
    onScroll: React.PropTypes.func,
    onSwipe: React.PropTypes.func,
    onFuse: PropTypes.oneOfType([
      React.PropTypes.shape({
        callback: React.PropTypes.func.isRequired,
        timeToFuse: PropTypes.number
      }),
      React.PropTypes.func
    ]),

    /**
     * Enables high accuracy gaze collision checks for this object.
     * This can be useful for complex 3D objects where the default
     * checking method of bounding boxes do not provide adequate
     * collision detection coverage.
     *
     * NOTE: Enabling high accuracy gaze collision checks has a high
     * performance cost and should be used sparingly / only when
     * necessary.
     *
     * Flag is set to false by default.
     */
    highAccuracyGaze:PropTypes.bool,
    onDrag: React.PropTypes.func,
    physicsBody: React.PropTypes.shape({
      type: React.PropTypes.oneOf(['dynamic','kinematic','static']).isRequired,
      mass: PropTypes.number,
      restitution: PropTypes.number,
      shape: React.PropTypes.shape({
        type: PropTypes.string.isRequired,
        params: PropTypes.arrayOf(PropTypes.number)
      }),
      friction: PropTypes.number,
      useGravity: PropTypes.bool,
      enabled: PropTypes.bool,
      velocity: PropTypes.arrayOf(PropTypes.number),
      force: PropTypes.oneOfType([
        PropTypes.arrayOf(React.PropTypes.shape({
          power: PropTypes.arrayOf(PropTypes.number),
          position: PropTypes.arrayOf(PropTypes.number)
        })),
        React.PropTypes.shape({
          power: PropTypes.arrayOf(PropTypes.number),
          position: PropTypes.arrayOf(PropTypes.number)
        }),
      ]),
      torque: PropTypes.arrayOf(PropTypes.number)
    }),

    viroTag: PropTypes.string,
    onCollided: React.PropTypes.func,
  },

getInitialState: function() {
  return {
    propsPositionState:this.props.position,
    nativePositionState:undefined
  }
},

  _onHover: function(event: Event) {
    this.props.onHover && this.props.onHover(event.nativeEvent.isHovering, event.nativeEvent.position, event.nativeEvent.source);
  },

  _onClick: function(event: Event) {
    this.props.onClick && this.props.onClick(event.nativeEvent.position, event.nativeEvent.source);
  },

  _onClickState: function(event: Event) {
    this.props.onClickState && this.props.onClickState(event.nativeEvent.clickState, event.nativeEvent.position, event.nativeEvent.source);
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

  applyImpulse: function(force, atPosition) {
    NativeModules.VRTNodeModule.applyImpulse(findNodeHandle(this), force, atPosition);
  },

  applyTorqueImpulse: function(torque) {
    NativeModules.VRTNodeModule.applyTorqueImpulse(findNodeHandle(this), torque);
  },

  setInstantaneousVelocity: function(velocity) {
    NativeModules.VRTNodeModule.setInstantaneousVelocity(findNodeHandle(this), velocity);
  },

  _onCollided: function(event: Event){
    if (this.props.onCollided){
      this.props.onCollided(event.nativeEvent.viroTag, event.nativeEvent.collidedPoint,
                                                           event.nativeEvent.collidedNormal);
    }
  },

  // Called from native on the event a positional change has occured
  // for the underlying control within the renderer.
  _onNativeTransformUpdate: function(event: Event){
    var position =  event.nativeEvent.position;
    this.setState({
      nativePositionState:position
    }, () => {
      if (this.props.onTransformUpdate){
        this.props.onTransformUpdate(position);
      }
    });
  },

  // Set the propsPositionState on the native control if the
  // nextProps.position state differs from the nativePositionState that
  // reflects this control's current vroNode position.
  componentWillReceiveProps(nextProps){
    if(nextProps.position != this.state.nativePositionState){
      var newPosition = [nextProps.position[0], nextProps.position[1], nextProps.position[2], Math.random()];
      this.setState({
        propsPositionState:newPosition
      });
    }
  },


  // Ignore all changes in native position state as it is only required to
  // keep track of the latest position prop set on this control.
  shouldComponentUpdate: function(nextProps, nextState) {
    if (nextState.nativePositionState != this.state.nativePositionState){
      return false;
    }

    return true;
  },

  setNativeProps: function(nativeProps) {
    this._component.setNativeProps(nativeProps);
  },

  render: function() {
    if (this.props.material) {
      console.error('The <ViroSphere> component takes a `materials` property rather than `material`.');
    }
    // Since materials and transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let materials = typeof this.props.materials === 'string' ? new Array(this.props.materials) : this.props.materials;
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;

    let timeToFuse = undefined;
    if (this.props.onFuse != undefined && typeof this.props.onFuse === 'object'){
        timeToFuse = this.props.onFuse.timeToFuse;
    }

    let transformDelegate = this.props.onTransformUpdate != undefined ? this._onNativeTransformUpdate : undefined;

    return (
      <VRTSphere
        {...this.props}
        ref={ component => {this._component = component; }}
        position={this.state.propsPositionState}
        materials={materials}
        transformBehaviors={transformBehaviors}
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
        canCollide={this.props.onCollided != undefined}
        onCollidedViro={this._onCollided}
        onNativeTransformDelegateViro={transformDelegate}
        hasTransformDelegate={this.props.onTransformUpdate != undefined}
        timeToFuse={timeToFuse}
        />
    );
  }
});


var VRTSphere = requireNativeComponent(
  'VRTSphere', ViroSphere , {
    nativeOnly: {
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
            onNativeTransformDelegateViro:true,
            hasTransformDelegate:true,
          }
  }
);


module.exports = ViroSphere;
