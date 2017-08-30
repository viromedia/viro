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
var PropTypes = React.PropTypes;

/**
 * Used to render a 360 image in a sphere.
 */
var Viro360Image = React.createClass({

  propTypes: {
    ...View.propTypes,

    /**
     * The image file, which is required
     */
    source: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./image.jpg')
      PropTypes.number,
    ]).isRequired,
    rotation: PropTypes.arrayOf(PropTypes.number),
    format: PropTypes.oneOf(['RGBA8', 'RGBA4', 'RGB565']),
    stereoMode:PropTypes.oneOf(['leftRight', 'rightLeft', 'topBottom', 'bottomTop', 'none']),
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

    /**
     * Callback triggered when the image fails to load. Invoked with
     * {nativeEvent: {error}}
     */
    onError: React.PropTypes.func,

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
    var imgsrc = resolveAssetSource(this.props.source);
    if (this.props.src) {
      console.error('The <Viro360Image> component takes a `source` property rather than `src`.');
    }

    if (this.props.onGaze) {
      console.error('The <Viro360Image> component does not take on an `onGaze` property. Pass the `onGaze` prop to <ViroScene> instead.');
    }

    if (this.props.onTap) {
      console.error('The <Viro360Image> component does not take on an `onTap` property. Pass the `onTap` prop to <ViroScene> instead.');
    }

    // Create native props object.
    let nativeProps = Object.assign({}, this.props);
    nativeProps.source = imgsrc;
    nativeProps.onErrorViro = this._onError;
    nativeProps.onLoadStartViro = this._onLoadStart;
    nativeProps.onLoadEndViro = this._onLoadEnd;
    nativeProps.ref = component => {this._component = component; };


    return (
      <VRT360Image {...nativeProps} />
    );
  }
});

var VRT360Image = requireNativeComponent(
  'VRT360Image', Viro360Image, {
    nativeOnly: {
      onLoadStartViro: true,
      onErrorViro: true,
      onLoadEndViro: true}
  }
);

module.exports = Viro360Image;
