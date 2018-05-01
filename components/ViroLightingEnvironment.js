/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule Viro360Image
 * @flow
 */
'use strict';

import { requireNativeComponent, View, StyleSheet } from 'react-native';
import React, { Component } from 'react';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
import PropTypes from 'prop-types';
var createReactClass = require('create-react-class');
import { checkMisnamedProps } from './Utilities/ViroProps';

var ViroLightingEnvironment = createReactClass({
  propTypes: {
    ...View.propTypes,

    /**
     * The hdr image file, which is required
     */
    source: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./image.jpg')
      PropTypes.number,
    ]).isRequired,

    /**
     * Callback triggered when we are processing the assets to be
     * used in computing this lighting environment (either downloading / reading from file).
     */
    onLoadStart: PropTypes.func,

    /**
     * Callback triggered when we have finished processing assets to be
     * used in computing this lighting environment. Wether or not assets were
     * processed successfully will be indicated by the parameter "success".
     * For example:
     *
     *   _onLoadEnd(event:Event){
     *      // Indication of asset loading success
     *      event.nativeEvent.success
     *   }
     *
     */
    onLoadEnd: PropTypes.func,

    /**
     * Callback triggered when the hdr image fails to load. Invoked with
     * {nativeEvent: {error}}
     */
    onError: PropTypes.func,

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

  setNativeProps: function(nativeProps) {
    this._component.setNativeProps(nativeProps);
  },

  render: function() {
    checkMisnamedProps("ViroLightingEnvironment", this.props);

    var imgsrc = resolveAssetSource(this.props.source);

    // Create native props object.
    let nativeProps = Object.assign({}, this.props);
    nativeProps.source = imgsrc;
    nativeProps.onErrorViro = this._onError;
    nativeProps.onLoadStartViro = this._onLoadStart;
    nativeProps.onLoadEndViro = this._onLoadEnd;
    nativeProps.ref = component => {this._component = component; };

    return (
      <VRTLightingEnvironment {...nativeProps} />
    );
  }
});

var VRTLightingEnvironment = requireNativeComponent(
  'VRTLightingEnvironment', ViroLightingEnvironment, {
    nativeOnly: {
      onLoadStartViro: true,
      onErrorViro: true,
      onLoadEndViro: true}
  }
);

module.exports = ViroLightingEnvironment;
