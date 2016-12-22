/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroImage
 * @flow
 */
'use strict';

import { requireNativeComponent, View, StyleSheet } from 'react-native';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
import React, { Component } from 'react';
var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var NativeModules = require('react-native').NativeModules;
var PropTypes = require('react/lib/ReactPropTypes');

// TODO: add styles
// var ViroPropTypes = require('ViroPropTypes');
// var StyleSheetPropType = require('StyleSheetPropType');
// var stylePropType = StyleSheetPropType(ViroPropTypes);

/**
 * Used to render a ViroImage
 */
var ViroImage = React.createClass({
  mixins: [NativeMethodsMixin],

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
    ]),

    /**
     * The position of the card
     */
    position: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    opacity: PropTypes.number,
    width: PropTypes.number,
    height: PropTypes.number,
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    visible: PropTypes.bool,
    // TODO: add styles
    //style: stylePropType,

    // Required to be local source static image by using require(''./image.jpg').
    // or by specifying local uri.
    // If not set, default is set to viro_blank.png in Resources which is a transparent image
    placeHolderSource:PropTypes.oneOfType([
      // TODO: Tooling to support documenting these directly and having them display in the docs.
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      PropTypes.number,
    ]),

    /**
     * Callback triggered when we are processing the assets to be
     * displayed in this ViroImage (either downloading / reading from file).
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
    var defaultPlaceHolder = require('./Resources/viro_blank.png');
    var imgsrc = resolveAssetSource(this.props.source);
    var placeHoldersrc;
    if (this.props.src) {
      console.error('The <ViroImage> component takes a `source` property rather than `src`.');
    }

    if (this.props.material) {
      console.error('The <ViroImage> component takes a `materials` property rather than `material`.');
    }

    if (this.props.placeHolderSource) {
      placeHoldersrc = resolveAssetSource(this.props.placeHolderSource);
    } else {
      placeHoldersrc = resolveAssetSource(defaultPlaceHolder);
    }

    // Since materials and transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let materials = typeof this.props.materials === 'string' ? new Array(this.props.materials) : this.props.materials;
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;

    // Create native props object.
    let nativeProps = Object.assign({}, this.props);
    nativeProps.materials = materials;
    nativeProps.source = imgsrc;
    nativeProps.placeHolderSource = placeHoldersrc;
    nativeProps.transformBehaviors = transformBehaviors;
    nativeProps.onLoadStartViro = this._onLoadStart;
    nativeProps.onLoadEndViro = this._onLoadEnd;
    nativeProps.style = [this.props.style];
    return (
      <VRTImage {...nativeProps} />
    );
  }
});

var VRTImage = requireNativeComponent(
  'VRTImage', ViroImage, {
    nativeOnly: {onTap: true, onGaze: true, onLoadStartViro: true, onLoadEndViro: true}
  }
);

module.exports = ViroImage;
