/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroParticleEmitter
 * @flow
 */
'use strict';

import { requireNativeComponent, View, findNodeHandle } from 'react-native';
import React from 'react';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
import { checkMisnamedProps } from './Utilities/ViroProps';
var NativeModules = require('react-native').NativeModules;
var createReactClass = require('create-react-class');
import PropTypes from 'prop-types';
var StyleSheet = require('react-native/Libraries/StyleSheet/StyleSheet');

var ViroPropTypes = require('./Styles/ViroPropTypes');
var StyleSheetPropType = require('react-native/Libraries/DeprecatedPropTypes/DeprecatedStyleSheetPropType');
var stylePropType = StyleSheetPropType(ViroPropTypes);
var ColorPropType = require('react-native').ColorPropType;
var processColor = require('react-native').processColor;

var ViroParticleEmitter = createReactClass({
  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    scalePivot: PropTypes.arrayOf(PropTypes.number),
    rotationPivot: PropTypes.arrayOf(PropTypes.number),
    onTransformUpdate: PropTypes.func,
    renderingOrder: PropTypes.number,
    visible: PropTypes.bool,
    viroTag: PropTypes.string,
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    highAccuracyEvents:PropTypes.bool,

    duration: PropTypes.number,
    delay: PropTypes.number,
    loop: PropTypes.bool,
    run: PropTypes.bool,
    fixedToEmitter : PropTypes.bool,

    image: PropTypes.shape({
      source : PropTypes.oneOfType([
        PropTypes.shape({
            uri: PropTypes.string,
        }),
        PropTypes.number
      ]).isRequired,
      height: PropTypes.number,
      width: PropTypes.number,
      bloomThreshold: PropTypes.number,
      blendMode: PropTypes.string,
    }).isRequired,

    spawnBehavior: PropTypes.shape({
      emissionRatePerSecond: PropTypes.arrayOf(PropTypes.number),
      emissionRatePerMeter: PropTypes.arrayOf(PropTypes.number),
      particleLifetime: PropTypes.arrayOf(PropTypes.number),
      maxParticles: PropTypes.number,
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

    particleAppearance: PropTypes.shape({
      opacity: PropTypes.shape({
        initialRange: PropTypes.arrayOf(PropTypes.number),
        factor: PropTypes.oneOf(["Time", "Distance"]),
        interpolation: PropTypes.arrayOf(PropTypes.shape({
          interval: PropTypes.arrayOf(PropTypes.number),
          endValue: PropTypes.number,
        })),
      }),
      scale: PropTypes.shape({
        initialRange: PropTypes.arrayOf(PropTypes.arrayOf(PropTypes.number)),
        factor: PropTypes.oneOf(["Time", "Distance"]),
        interpolation: PropTypes.arrayOf(PropTypes.shape({
          interval: PropTypes.arrayOf(PropTypes.number),
          endValue: PropTypes.arrayOf(PropTypes.number),
        })),
      }),
      // rotation is only about the Z axis
      rotation: PropTypes.shape({
        initialRange: PropTypes.arrayOf(PropTypes.number),
        factor: PropTypes.oneOf(["Time", "Distance"]),
        interpolation: PropTypes.arrayOf(PropTypes.shape({
          interval: PropTypes.arrayOf(PropTypes.number),
          endValue: PropTypes.number,
        })),
      }),
      color: PropTypes.shape({
        initialRange:  PropTypes.arrayOf(ColorPropType),
        factor: PropTypes.oneOf(["Time", "Distance"]),
        interpolation: PropTypes.arrayOf(PropTypes.shape({
          interval: PropTypes.arrayOf(PropTypes.number),
          endValue: ColorPropType,
        })),
      }),
    }),

    particlePhysics: PropTypes.shape({
      velocity: PropTypes.shape({
        initialRange: PropTypes.arrayOf(PropTypes.arrayOf(PropTypes.number)),
      }),
      acceleration: PropTypes.shape({
        initialRange: PropTypes.arrayOf(PropTypes.arrayOf(PropTypes.number)),
      }),

      explosiveImpulse:PropTypes.shape({
        impulse: PropTypes.number,
        position: PropTypes.arrayOf(PropTypes.number),
        decelerationPeriod: PropTypes.number,
      }),
    }),
  },

  getInitialState: function() {
    return {
      propsPositionState:this.props.position,
      nativePositionState:undefined
    }
  },

  async getTransformAsync() {
    return await NativeModules.VRTNodeModule.getNodeTransform(findNodeHandle(this));
  },

  async getBoundingBoxAsync() {
    return await NativeModules.VRTNodeModule.getBoundingBox(findNodeHandle(this));
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

    checkMisnamedProps("ViroParticleEmitter", this.props);

    let image = {...this.props.image}
    if (image.source != undefined) {
      image.source = resolveAssetSource(image.source);
    }

    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
      new Array(this.props.transformBehaviors) : this.props.transformBehaviors;

    let transformDelegate = this.props.onTransformUpdate != undefined ? this._onNativeTransformUpdate : undefined;

    // Create native props object.
    let nativeProps = Object.assign({}, this.props);
    nativeProps.position = this.state.propsPositionState;
    nativeProps.onNativeTransformDelegateViro = transformDelegate;
    nativeProps.hasTransformDelegate = this.props.onTransformUpdate != undefined;
    nativeProps.image = image;
    nativeProps.transformBehaviors = transformBehaviors;

    // For color modifiers, we'll need to processColor for each color value.
    if (this.props.particleAppearance && this.props.particleAppearance.color){
      let colorModifier = this.props.particleAppearance.color;
      if (colorModifier.initialRange.length != 2){
        console.error('The <ViroParticleEmitter> component requires initial value of [min, max] when defining inital rotation property!');
        return;
      }

      let minColorFinal = processColor(colorModifier.initialRange[0]);
      let maxColorFinal = processColor(colorModifier.initialRange[1]);
      let modifierFinal = [];
      let interpolationLength = colorModifier.interpolation != undefined ? colorModifier.interpolation.length : 0;
      for (let i = 0; i < interpolationLength; i ++){
        let processedColor = processColor(colorModifier.interpolation[i].endValue);
        let mod = {
            interval: colorModifier.interpolation[i].interval,
            endValue: processedColor
        };
        modifierFinal.push(mod);
      }

      let newAppearanceColorMod = {
        initialRange: [minColorFinal, maxColorFinal],
        factor:colorModifier.factor,
        interpolation:modifierFinal
      }
      nativeProps.particleAppearance.color = newAppearanceColorMod;
    }

    // For rotation modifiers, convert degrees to radians, then apply the
    // Z rotation (due to billboarding for quad particles)
    if (this.props.particleAppearance && this.props.particleAppearance.rotation){
      let rotMod = this.props.particleAppearance.rotation;
      if (rotMod.initialRange.length != 2){
        console.error('The <ViroParticleEmitter> component requires initial value of [min, max] when defining inital rotation property!');
      }

      let minRotFinal = [0,0,rotMod.initialRange[0] * Math.PI / 180];
      let maxRotFinal = [0,0,rotMod.initialRange[1] * Math.PI / 180];
      let modifierFinal = [];

      let interpolationLength = rotMod.interpolation != undefined ? rotMod.interpolation.length : 0;
      for (var i = 0; i < interpolationLength; i ++){
        let processedRot = [0,0, rotMod.interpolation[i].endValue * Math.PI / 180];
        let mod = {
            interval: rotMod.interpolation[i].interval,
            endValue: processedRot
        };
        modifierFinal.push(mod);
      }

      let newAppearanceRotMod = {
        initialRange: [minRotFinal, maxRotFinal],
        factor:rotMod.factor,
        interpolation:modifierFinal
      }
      nativeProps.particleAppearance.rotation = newAppearanceRotMod;
    }

    nativeProps.ref = component => {this._component = component; };

    return (
      <VRTParticleEmitter {...nativeProps} />
    );
  },

  // Set the propsPositionState on the native control if the
  // nextProps.position state differs from the nativePositionState that
  // reflects this control's current vroNode position.
  statics: {
    getDerivedStateFromProps(nextProps, prevState) {
      if (nextProps.position != prevState.nativePositionState) {
        var newPosition = [nextProps.position[0], nextProps.position[1], nextProps.position[2], Math.random()];
        return {
          propsPositionState:newPosition
        };
      }
      return {};
    }
  },
});

var VRTParticleEmitter = requireNativeComponent(
  'VRTParticleEmitter', ViroParticleEmitter, {
    nativeOnly: {
      onNativeTransformDelegateViro:true,
      hasTransformDelegate:true,
      canHover: true,
      canClick: true,
      canTouch: true,
      canScroll: true,
      canSwipe: true,
      canDrag: true,
      canPinch: true,
      canRotate: true,
      canFuse: true,
      canCollide: true,
      onHoverViro: true,
      onClickViro: true,
      onTouchViro: true,
      onScrollViro: true,
      onSwipeViro: true,
      onDragViro:true,
      onPinchViro:true,
      onRotateViro:true,
      onPlatformUpdateViro: true,
      onFuseViro:true,
      timeToFuse:true,
      physicsBody:true,
      onCollisionViro:true,
      animation:true,
      materials:true,
      dragType:true,
      dragPlane: true,
      ignoreEventHandling: true,
    }
  }
);

module.exports = ViroParticleEmitter;
