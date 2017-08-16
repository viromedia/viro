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
var PropTypes = require('react/lib/ReactPropTypes');
var StyleSheet = require('react-native/Libraries/StyleSheet/StyleSheet');

var ViroPropTypes = require('./Styles/ViroPropTypes');
var StyleSheetPropType = require('react-native/Libraries/StyleSheet/StyleSheetPropType');
var stylePropType = StyleSheetPropType(ViroPropTypes);
var ColorPropType = require('react-native').ColorPropType;

/**
 * Used to render a ViroSurface
 */
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
    }),
    spawnModifier: PropTypes.shape({
      // TODO: maybe make this oneOfType?
      emissionRatePerSecond: PropTypes.arrayOf(PropTypes.number),
      emissionRatePerMeter: PropTypes.arrayOf(PropTypes.number),
      particleLifetime: PropTypes.number,
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
        size: PropTypes.arrayOf(PropTypes.number),
      }),
    }),
    appearanceModifier: PropTypes.shape({
      stretchFactor: PropTypes.arrayOf(PropTypes.number),
      opacity: PropTypes.shape({
        min: PropTypes.number,
        max: PropTypes.number,
        modifier: PropTypes.arrayOf(PropTypes.shape({
          factor: PropTypes.string,
          finalValue: PropTypes.number,
          interval: PropTypes.arrayOf(PropTypes.number),
        })),
      }),
      scale: PropTypes.shape({
        min: PropTypes.arrayOf(PropTypes.number),
        max: PropTypes.arrayOf(PropTypes.number),
        modifier: PropTypes.arrayOf(PropTypes.shape({
          factor: PropTypes.string,
          finalValue: PropTypes.arrayOf(PropTypes.number),
          interval: PropTypes.arrayOf(PropTypes.number),
        })),
      }),
      // rotation is only about the Z axis
      rotation: PropTypes.shape({
        min: PropTypes.number,
        max: PropTypes.number,
        modifier: PropTypes.arrayOf(PropTypes.shape({
          factor: PropTypes.string,
          finalValue: PropTypes.number,
          interval: PropTypes.arrayOf(PropTypes.number),
        })),
      }),
      color: PropTypes.shape({
        min: ColorPropType,
        max: ColorPropType,
        modifier: PropTypes.arrayOf(PropTypes.shape({
          factor: PropTypes.string,
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
