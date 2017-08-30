/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroQuadEmitter
 * @flow
 */
'use strict';

import { requireNativeComponent, View, findNodeHandle } from 'react-native';
import React from 'react';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
var NativeModules = require('react-native').NativeModules;
var PropTypes = React.PropTypes;
var StyleSheet = require('react-native/Libraries/StyleSheet/StyleSheet');

var ViroPropTypes = require('./Styles/ViroPropTypes');
var StyleSheetPropType = require('react-native/Libraries/StyleSheet/StyleSheetPropType');
var stylePropType = StyleSheetPropType(ViroPropTypes);
var ColorPropType = require('react-native').ColorPropType;
var processColor = require('react-native').processColor;

var ViroQuadEmitter = React.createClass({
  // TODO: make certain props required.
  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    scalePivot: PropTypes.arrayOf(PropTypes.number),
    rotationPivot: PropTypes.arrayOf(PropTypes.number),
    onTransformUpdate: React.PropTypes.func,
    duration: PropTypes.number,
    delay: PropTypes.number,
    loop: PropTypes.bool,
    run: PropTypes.bool,
    visible: PropTypes.bool,
    fixedToEmitter : PropTypes.bool,
    quad: PropTypes.shape({
      source : PropTypes.oneOfType([
        PropTypes.shape({
            uri: PropTypes.string,
        }),
        PropTypes.number // opaque type returned by require('./res/source')
      ]),
      height: PropTypes.number,
      width: PropTypes.number,
    }).isRequired,
    spawnModifier: PropTypes.shape({
      // TODO: maybe make this oneOfType?
      emissionRatePerSecond: PropTypes.arrayOf(PropTypes.number),
      emissionRatePerMeter: PropTypes.arrayOf(PropTypes.number),
      particleLifetime: PropTypes.arrayOf(PropTypes.number),
      maxParticles: PropTypes.number,
      despawnDistance: PropTypes.number,
      emissionBurst: PropTypes.arrayOf(PropTypes.oneOfType([
        PropTypes.shape({
          time: PropTypes.number,
          min: PropTypes.number,
          max: PropTypes.number,
          cycles: PropTypes.number,
          cooldownPeriod: PropTypes.number,
        }),
        PropTypes.shape({
          distance: PropTypes.number,
          min: PropTypes.number,
          max: PropTypes.number,
          cycles: PropTypes.number,
          cooldownDistance: PropTypes.number,
        }),
      ])),
      spawnVolume: PropTypes.shape({
        shape: PropTypes.string,
        params: PropTypes.arrayOf(PropTypes.number),
        spawnOnSurface:PropTypes.bool
      }),
    }),
    appearanceModifier: PropTypes.shape({
      stretchFactor: PropTypes.arrayOf(PropTypes.number),
      opacity: PropTypes.shape({
        min: PropTypes.number,
        max: PropTypes.number,
        factor: PropTypes.string,
        modifier: PropTypes.arrayOf(PropTypes.shape({
          finalValue: PropTypes.number,
          interval: PropTypes.arrayOf(PropTypes.number),
        })),
      }),
      scale: PropTypes.shape({
        min: PropTypes.arrayOf(PropTypes.number),
        max: PropTypes.arrayOf(PropTypes.number),
        factor: PropTypes.string,
        modifier: PropTypes.arrayOf(PropTypes.shape({
          finalValue: PropTypes.arrayOf(PropTypes.number),
          interval: PropTypes.arrayOf(PropTypes.number),
        })),
      }),
      // rotation is only about the Z axis
      rotation: PropTypes.shape({
        min: PropTypes.number,
        max: PropTypes.number,
        factor: PropTypes.string,
        modifier: PropTypes.arrayOf(PropTypes.shape({
          finalValue: PropTypes.number,
          interval: PropTypes.arrayOf(PropTypes.number),
        })),
      }),
      color: PropTypes.shape({
        min: ColorPropType,
        max: ColorPropType,
        factor: PropTypes.string,
        modifier: PropTypes.arrayOf(PropTypes.shape({
          finalValue: ColorPropType,
          interval: PropTypes.arrayOf(PropTypes.number),
        })),
      }),
    }),
    physicsModifier: PropTypes.shape({
      velocity: PropTypes.shape({
        min: PropTypes.arrayOf(PropTypes.number),
        max: PropTypes.arrayOf(PropTypes.number),
      }),
      acceleration: PropTypes.shape({
        min: PropTypes.arrayOf(PropTypes.number),
        max: PropTypes.arrayOf(PropTypes.number),
      }),
      initialExplosiveImpulse:PropTypes.shape({
        impulse: PropTypes.number,
        position: PropTypes.arrayOf(PropTypes.number),
      }),
    }),
  },

  getInitialState: function() {
    return {
      propsPositionState:this.props.position,
      nativePositionState:undefined
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

    let quad = {...this.props.quad}
    if (quad.source != undefined) {
      quad.source = resolveAssetSource(quad.source);
    }

    let transformDelegate = this.props.onTransformUpdate != undefined ? this._onNativeTransformUpdate : undefined;

    // Create native props object.
    let nativeProps = Object.assign({}, this.props);
    nativeProps.position = this.state.propsPositionState;
    nativeProps.onNativeTransformDelegateViro = transformDelegate;
    nativeProps.hasTransformDelegate = this.props.onTransformUpdate != undefined;
    nativeProps.quad = quad;

    // For color modifiers, we'll need to processColor for each color value.
    if (this.props.appearanceModifier && this.props.appearanceModifier.color){
      let colorModifier = this.props.appearanceModifier.color;
      let minColorFinal = processColor(colorModifier.min);
      let maxColorFinal = processColor(colorModifier.max);
      let modifierFinal = [];
      for (let i = 0; i < colorModifier.modifier.length; i ++){
        let processedColor = processColor(colorModifier.modifier[i].finalValue);
        let mod = {
            interval: colorModifier.modifier[i].interval,
            finalValue: processedColor
        };
        modifierFinal.push(mod);
      }

      let newAppearanceColorMod = {
        min: minColorFinal,
        max: maxColorFinal,
        factor:colorModifier.factor,
        modifier:modifierFinal
      }
      nativeProps.appearanceModifier.color = newAppearanceColorMod;
    }

    // For rotation modifiers, convert degrees to radians, then apply the
    // Z rotation (due to billboarding for quad particles)
    if (this.props.appearanceModifier && this.props.appearanceModifier.rotation){
      let rotMod = this.props.appearanceModifier.rotation;
      let minRotFinal = [0,0,rotMod.min * Math.PI / 180];
      let maxRotFinal = [0,0,rotMod.max * Math.PI / 180];
      let modifierFinal = [];
      for (var i = 0; i < rotMod.modifier.length; i ++){
        let processedRot = [0,0, rotMod.modifier[i].finalValue * Math.PI / 180];
        let mod = {
            interval: rotMod.modifier[i].interval,
            finalValue: processedRot
        };
        modifierFinal.push(mod);
      }

      let newAppearanceRotMod = {
        min: minRotFinal,
        max: maxRotFinal,
        factor:rotMod.factor,
        modifier:modifierFinal
      }
      nativeProps.appearanceModifier.rotation = newAppearanceRotMod;
    }

    nativeProps.ref = component => {this._component = component; };

    return (
      <VRTQuadEmitter {...nativeProps} />
    );
  }
});

var VRTQuadEmitter = requireNativeComponent(
  'VRTQuadEmitter', ViroQuadEmitter, {
    nativeOnly: {
      onNativeTransformDelegateViro:true,
      hasTransformDelegate:true
    }
  }
);

module.exports = ViroQuadEmitter;
