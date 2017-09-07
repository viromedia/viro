/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule Viro3DObject
 * @flow
 */
'use strict';

import { requireNativeComponent, View, StyleSheet, findNodeHandle } from 'react-native';
import React, { Component } from 'react';
var NativeModules = require('react-native').NativeModules;
var PropTypes = React.PropTypes;

import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";

/**
 * Used to render a Viro3DObject
 */
var Viro3DObject = React.createClass({
  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    scalePivot: PropTypes.arrayOf(PropTypes.number),
    rotationPivot: PropTypes.arrayOf(PropTypes.number),
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    type: PropTypes.oneOf(['OBJ', 'VRX']).isRequired,
    opacity: PropTypes.number,
    ignoreEventHandling: PropTypes.bool,
    lightBitMask : PropTypes.number,
    shadowCastingBitMask : PropTypes.number,
    onTransformUpdate: React.PropTypes.func,
    /*
     * The model file, which is required
     */
    source: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./spaceship.obj')
      PropTypes.number,
    ]).isRequired,
    resources: PropTypes.arrayOf(
      PropTypes.oneOfType([
        PropTypes.shape({
          uri: PropTypes.string,
        }),
        PropTypes.number,
      ])
    ),
    animation: React.PropTypes.shape({
      name: PropTypes.string,
      delay: PropTypes.number,
      loop: PropTypes.bool,
      onStart: React.PropTypes.func,
      onFinish: React.PropTypes.func,
      run: PropTypes.bool,
    }),
    visible: PropTypes.bool,

    onHover: React.PropTypes.func,
    onClick: React.PropTypes.func,
    onClickState: React.PropTypes.func,
    onTouch: React.PropTypes.func,
    onScroll: React.PropTypes.func,
    onSwipe: React.PropTypes.func,
    onLoadStart: React.PropTypes.func,
    onLoadEnd: React.PropTypes.func,
    onError: React.PropTypes.func,
    onDrag: React.PropTypes.func,
    onPinch: React.PropTypes.func,
    onRotate: React.PropTypes.func,
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

  _onLoadStart: function(event: Event) {
      this.props.onLoadStart && this.props.onLoadStart(event);
  },

  _onLoadEnd: function(event: Event) {
      this.props.onLoadEnd && this.props.onLoadEnd(event);
  },

  _onError: function(event: Event) {
    this.props.onError && this.props.onError(event);
  },

  _onPinch: function(event: Event) {
    this.props.onPinch && this.props.onPinch(event.nativeEvent.pinchState, event.nativeEvent.scaleFactor, event.nativeEvent.source);
  },

  _onRotate: function(event: Event) {
    this.props.onRotate && this.props.onRotate(event.nativeEvent.rotateState, event.nativeEvent.rotationFactor, event.nativeEvent.source);
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

  _onAnimationStart: function(event: Event) {
    this.props.animation && this.props.animation.onStart && this.props.animation.onStart();
  },

  _onAnimationFinish: function(event: Event) {
    this.props.animation && this.props.animation.onFinish && this.props.animation.onFinish();
  },

  setNativeProps: function(nativeProps) {
     this._viro3dobj.setNativeProps(nativeProps);

  },

  applyImpulse: function(force, position) {
    NativeModules.VRTNodeModule.applyImpulse(findNodeHandle(this), force, position);
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

  async getTransformAsync() {
    return await NativeModules.VRTNodeModule.getNodeTransform(findNodeHandle(this));
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

  render: function() {
    var modelsrc = resolveAssetSource(this.props.source);
    var resources = null;
    if (this.props.resources != undefined) {
      resources = this.props.resources.map(function(resource) {
        return resolveAssetSource(resource)
      });
    }
    let onGaze = this.props.onGaze ? this._onGaze : undefined;
    // Since materials and transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let materials = typeof this.props.materials === 'string' ? new Array(this.props.materials) : this.props.materials;
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;
    if (this.props.src) {
      console.error('The <Viro3DObject> component takes a `source` property rather than `src`.');
    }

    if (this.props.material) {
      console.error('The <Viro3DObject> component takes a `materials` property rather than `material`.');
    }

    let timeToFuse = undefined;
    if (this.props.onFuse != undefined && typeof this.props.onFuse === 'object'){
        timeToFuse = this.props.onFuse.timeToFuse;
    }

    // Always autogenerate a compound shape for 3DObjects if no shape is defined.
    let newPhysicsBody = undefined;
    if (this.props.physicsBody){
      let newPhysicsShape = undefined;
      if (this.props.physicsBody.shape == undefined){
        newPhysicsShape = {type:'AutoCompound'}
      } else {
        newPhysicsShape = this.props.physicsBody.shape;
      }

    newPhysicsBody = {
        type: this.props.physicsBody.type,
        mass: this.props.physicsBody.mass,
        restitution: this.props.physicsBody.restitution,
        friction: this.props.physicsBody.friction,
        useGravity: this.props.physicsBody.useGravity,
        enabled: this.props.physicsBody.enabled,
        velocity: this.props.physicsBody.velocity,
        force: this.props.physicsBody.force,
        torque: this.props.physicsBody.torque,
        shape: newPhysicsShape
      };
    }

    let transformDelegate = this.props.onTransformUpdate != undefined ? this._onNativeTransformUpdate : undefined;

    return (
      <VRT3DObject
        {...this.props}
        ref={ component => { this._viro3dobj = component; }}
        position={this.state.propsPositionState}
        onNativeTransformDelegateViro={transformDelegate}
        hasTransformDelegate={this.props.onTransformUpdate != undefined}
        physicsBody={newPhysicsBody}
        source={modelsrc}
        resources={resources}
        materials={materials}
        transformBehaviors={transformBehaviors}
        canHover={this.props.onHover != undefined}
        canClick={this.props.onClick != undefined || this.props.onClickState != undefined}
        canTouch={this.props.onTouch != undefined}
        canScroll={this.props.onScroll != undefined}
        canSwipe={this.props.onSwipe != undefined}
        canDrag={this.props.onDrag != undefined}
        canFuse={this.props.onFuse != undefined}
        canPinch={this.props.onPinch != undefined}
        canRotate={this.props.onRotate != undefined}
        onHoverViro={this._onHover}
        onClickViro={this._onClickState}
        onTouchViro={this._onTouch}
        onScrollViro={this._onScroll}
        onSwipeViro={this._onSwipe}
        onDragViro={this._onDrag}
        onFuseViro={this._onFuse}
        onPinchViro={this._onPinch}
        onRotateViro={this._onRotate}
        onLoadStartViro={this._onLoadStart}
        onLoadEndViro={this._onLoadEnd}
        onErrorViro={this._onError}
        onAnimationStartViro={this._onAnimationStart}
        onAnimationFinishViro={this._onAnimationFinish}
        timeToFuse={timeToFuse}
        canCollide={this.props.onCollided != undefined}
        onCollidedViro={this._onCollided}
      />
    );
  }
});

var VRT3DObject = requireNativeComponent(
  'VRT3DObject', Viro3DObject, {
    nativeOnly: {
            canHover: true,
            canClick: true,
            canTouch: true,
            canScroll: true,
            canSwipe: true,
            canDrag: true,
            canFuse: true,
            canPinch: true,
            canRotate: true,
            onHoverViro:true,
            onClickViro:true,
            onTouchViro:true,
            onScrollViro:true,
            onPinchViro:true,
            onRotateViro:true,
            onSwipeViro:true,
            onDragViro:true,
            onLoadStartViro:true,
            onLoadEndViro:true,
            onErrorViro:true,
            onFuseViro:true,
            timeToFuse:true,
            canCollide:true,
            onCollidedViro:true,
            onNativeTransformDelegateViro:true,
            hasTransformDelegate:true,
            onAnimationStartViro:true,
            onAnimationFinishViro:true,
          }
  }
);

module.exports = Viro3DObject;
