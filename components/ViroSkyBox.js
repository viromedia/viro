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

import { requireNativeComponent, View, StyleSheet } from 'react-native';
import React, { Component } from 'react';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";

var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var NativeModules = require('react-native').NativeModules;
var PropTypes = require('react/lib/ReactPropTypes');
var CubeMapPropType = require('./Material/CubeMapPropType');

/**
 * Used to render a skybox as a scene background.
 */
var ViroSkybox = React.createClass({
  mixins: [NativeMethodsMixin],

  propTypes: {
    ...View.propTypes,
    /**
     * The source cube map. Either this or a color must be specified.
     */
    source: CubeMapPropType,

    /**
     * TODO VIRO-621 Add support for color prop type
     */
    color: PropTypes.string,

    /**
     * Callback triggered when we are processing the assets to be
     * displayed in this 360 Photo (either downloading / reading from file).
     */
    onLoadStart: React.PropTypes.func,

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
    onLoadEnd: React.PropTypes.func,
  },

  _onLoadStart: function(event: Event) {
    this.props.onLoadStart && this.props.onLoadStart(event);
  },

  _onLoadEnd: function(event: Event) {
    this.props.onLoadEnd && this.props.onLoadEnd(event);
  },

  render: function() {
    if (this.props.src) {
      console.error('The <ViroSkybox> component takes a `source` property rather than `src`.');
    }

    if (this.props.onGaze) {
      console.error('The <Viro360Photo> component does not take on an `onGaze` property. Pass the `onGaze` prop to <ViroScene> instead.');
    }

    if (this.props.onTap) {
      console.error('The <Viro360Photo> component does not take on an `onTap` property. Pass the `onTap` prop to <ViroScene> instead.');
    }


    if (this.props.source !== undefined) {
      var skyboxDict = {};

      for (var key in this.props.source) {
        var s = resolveAssetSource(this.props.source[key]);
        skyboxDict[key] = s;
      }

      // Create native props object.
      let nativeProps = Object.assign({}, this.props);
      nativeProps.source = skyboxDict;
      nativeProps.onViroSkyBoxLoadStart = this._onLoadStart;
      nativeProps.onViroSkyBoxLoadEnd = this._onLoadEnd;

      return (
        <VRTSkybox {...nativeProps}/>
      );
    }
    else {
      return (
        <VRTSkybox {...this.props} />
      )
    }
  }
});

var VRTSkybox = requireNativeComponent(
  'VRTSkybox', ViroSkybox
);

module.exports = ViroSkybox;
