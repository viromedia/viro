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
var PropTypes = require('react/lib/ReactPropTypes');
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

/**
 * Composite control for a 2D Spinner
 */
var ViroSpinner = React.createClass({
  propTypes: {
    position: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
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
  },

  getDefaultProps() {
    return {
      type: 'dark'
    };
  },

  render: function() {
    // Since transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;

    return (
      <ViroNode position={this.props.position} rotation={this.props.rotation} scale={this.props.scale}
            opacity={this.props.opacity} transformBehaviors={transformBehaviors} visible={this.props.visible}
            onHover={this.props.onHover} onClick={this.props.onClick} onClickState={this.props.onClickState}
            onTouch={this.props.onTouch} onDrag={this.props.onDrag}>

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
