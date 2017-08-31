/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroSpinner
 */
'user strict';

import React, { Component } from 'react';
var PropTypes = React.PropTypes;
var ViroMaterials = require('./Material/ViroMaterials');
var ViroAnimations = require('./Animation/ViroAnimations');
var ViroNode = require('./ViroNode');
var ViroAnimatedComponent = require('./ViroAnimatedComponent');
var ViroImage = require('./ViroImage');

// Setup spinner assets
var ViroSpinner_1 = require('./Resources/viro_spinner_1.png');
var ViroSpinner_1a = require('./Resources/viro_spinner_1a.png');
var ViroSpinner_1_w = require('./Resources/viro_spinner_1_w.png');
var ViroSpinner_1a_w = require('./Resources/viro_spinner_1a_w.png');
var RCT_SPINNER_REF = 'virospinnercomponent';
/**
 * Composite control for a 2D Spinner
 */
var ViroSpinner = React.createClass({
  propTypes: {
    position: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    scalePivot: PropTypes.arrayOf(PropTypes.number),
    rotationPivot: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    opacity: PropTypes.number,
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    onTransformUpdate: React.PropTypes.func,
    visible: PropTypes.bool,

    /**
     * Spinner visual type for either a light or dark theme.
     * This defaults to dark.
     */
    type: React.PropTypes.oneOf(['dark','light']),

    onHover: React.PropTypes.func,
    onClick: React.PropTypes.func,
    onClickState: React.PropTypes.func,
    onTouch: React.PropTypes.func,
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
    physicsBody: React.PropTypes.shape({
      type: React.PropTypes.oneOf(['dynamic','kinematic','static']).isRequired,
      mass: PropTypes.number,
      restitution: PropTypes.number,
      shape: React.PropTypes.shape({
        type: PropTypes.string.isRequired,
        params: PropTypes.arrayOf(PropTypes.number)
      }).isRequired,
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

  getDefaultProps() {
    return {
      type: 'dark'
    };
  },

  applyImpulse: function(force, position) {
    this.refs[RCT_SPINNER_REF].applyImpulse(force, position);
  },

  applyTorqueImpulse: function(torque) {
    this.refs[RCT_SPINNER_REF].applyTorqueImpulse(torque);
  },

  setInstantaneousVelocity: function(velocity) {
    this.refs[RCT_SPINNER_REF].setInstantaneousVelocity(findNodeHandle(this), velocity);
  },

  async getTransformAsync() {
    return await this.refs[RCT_SPINNER_REF].getTransformAsync();
  },

  render: function() {
    // Since transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;

    return (
      <ViroNode position={this.props.position} rotation={this.props.rotation} scale={this.props.scale}
            rotationPivot={this.props.rotationPivot} scalePivot={this.props.scalePivot} physicsBody={this.props.physicsBody}
            opacity={this.props.opacity} transformBehaviors={transformBehaviors} visible={this.props.visible}
            onHover={this.props.onHover} onClick={this.props.onClick} onClickState={this.props.onClickState}
            onTouch={this.props.onTouch} onDrag={this.props.onDrag} onPinch={this.props.onPinch}
            onRotate={this.props.onRotate} onFuse={this.props.onFuse}
            onTransformUpdate={this.props.onTransformUpdate} ref={RCT_SPINNER_REF}>

        <ViroAnimatedComponent animation="_ViroSpinner_clockwiseZ" run={true} loop={true} >
          <ViroImage source={this._getImage1()} materials={this.props.materials} />
        </ViroAnimatedComponent>

        {/* Set the posititon of this one to be .01 forward of the other view to help w/ z-fighting*/}
        <ViroAnimatedComponent animation="_ViroSpinner_counterClockwiseZ" run={true} loop={true} >
          <ViroImage position={[0, 0, .01]} source={this._getImage1a()} materials={this.props.materials} />
        </ViroAnimatedComponent>
      </ViroNode>
    );
  },

  _getImage1() {
    return this.props.type == 'light' ? ViroSpinner_1 : ViroSpinner_1_w;
  },

  _getImage1a() {
    return this.props.type == 'light' ? ViroSpinner_1a : ViroSpinner_1a_w;
  },
});

ViroAnimations.registerAnimations({
  _ViroSpinner_counterClockwiseZ: {
    properties: {
      rotateZ: "+=90"
    },
    duration: 250, //.25 seconds
  },
  _ViroSpinner_clockwiseZ: {
    properties: {
      rotateZ: "-=90"
    },
    duration: 250, //.25 seconds
  },
});

module.exports = ViroSpinner;
