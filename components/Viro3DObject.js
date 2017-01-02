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

import { requireNativeComponent, View, StyleSheet } from 'react-native';
import React, { Component } from 'react';
var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var NativeModules = require('react-native').NativeModules;
var PropTypes = require('react/lib/ReactPropTypes');

import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";

/**
 * Used to render a Viro3DObject
 */
var Viro3DObject = React.createClass({
  mixins: [NativeMethodsMixin],

  propTypes: {
    position: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    opacity: PropTypes.number,

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
    visible: PropTypes.bool,

    /**
     * Callback that is called when user gazes on box.
     */
    onGaze: React.PropTypes.func,

    /**
     * Callback that is called when user taps on box.
     */
    onTap: React.PropTypes.func,

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
  },

  _onGaze: function(event: Event) {
    if (this.props.onGaze) {
      this.props.onGaze(event.nativeEvent.isGazing);
    }
  },

  _onTap: function(event: Event) {
    if (this.props.onTap) {
        this.props.onTap();
    }
  },

  render: function() {
    var modelsrc = resolveAssetSource(this.props.source);
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
    return (
      <VRT3DObject
        {...this.props}
        source={modelsrc}
        materials={materials}
        transformBehaviors={transformBehaviors}
        canGaze={this.props.onGaze != undefined}
        canTap={this.props.onTap != undefined}
        onTapViro={this._onTap}
        onGazeViro={this._onGaze}
      />
    );
  }
});

var VRT3DObject = requireNativeComponent(
  'VRT3DObject', Viro3DObject, {
    nativeOnly: {canTap: true, canGaze: true, onTapViro:true, onGazeViro:true}
  }
);

module.exports = Viro3DObject;
