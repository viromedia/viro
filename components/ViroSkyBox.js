/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroSkybox
 * @flow
 */
'use strict';

import { requireNativeComponent, View, StyleSheet, Platform } from 'react-native';
import React, { Component } from 'react';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
var NativeModules = require('react-native').NativeModules;
var createReactClass = require('create-react-class');

import { checkMisnamedProps } from './Utilities/ViroProps';
import PropTypes from 'prop-types';
var CubeMapPropType = require('./Material/CubeMapPropType');
var ColorPropType = require('react-native').ColorPropType;

/**
 * Used to render a skybox as a scene background.
 */
var ViroSkybox = createReactClass({

  propTypes: {
    ...View.propTypes,
    /**
     * The source cube map. Either this or a color must be specified.
     */
    source: CubeMapPropType,
    color: ColorPropType,
    format: PropTypes.oneOf(['RGBA8', 'RGB565']),

    /**
     * Callback triggered when we are processing the assets to be
     * displayed in this 360 Photo (either downloading / reading from file).
     */
    onLoadStart: PropTypes.func,

    /**
     * Callback triggered when we have finished processing assets to be
     * displayed. Wether or not assets were processed successfully and
     * thus displayed will be indicated by the parameter "success".
     * For example:
     *
     *   _onLoadEnd(event:Event){
     *      // Indication of asset loading success
     *      event.nativeEvent.success
     *   }
     *
     */
    onLoadEnd: PropTypes.func,
  },

  _onLoadStart: function(event: Event) {
    this.props.onLoadStart && this.props.onLoadStart(event);
  },

  _onLoadEnd: function(event: Event) {
    this.props.onLoadEnd && this.props.onLoadEnd(event);
  },

  setNativeProps: function(nativeProps) {
    this._component.setNativeProps(nativeProps);
  },

  render: function() {

    checkMisnamedProps("ViroSkyBox", this.props);

    // Create and set the native props.
    var skyboxDict = {};
    let nativeProps = Object.assign({}, this.props);

    if (this.props.source !== undefined) {
      for (var key in this.props.source) {
        var s = resolveAssetSource(this.props.source[key]);
        skyboxDict[key] = s;
      }
      nativeProps.source = skyboxDict;
    }

    nativeProps.onViroSkyBoxLoadStart = this._onLoadStart;
    nativeProps.onViroSkyBoxLoadEnd = this._onLoadEnd;
    nativeProps.color = this.props.color;
    nativeProps.ref = component => {this._component = component; };

    return (
      <VRTSkybox {...nativeProps}/>
    );
  }
});

var VRTSkybox = requireNativeComponent(
  'VRTSkybox', ViroSkybox, {
    nativeOnly: {onViroSkyBoxLoadStart: true, onViroSkyBoxLoadEnd: true}
  }
);

module.exports = ViroSkybox;
