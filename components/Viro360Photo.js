/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule Viro360Photo
 * @flow
 */
'use strict';

import { requireNativeComponent, View } from 'react-native';

var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var NativeModules = require('NativeModules');
var PropTypes = require('react/lib/ReactPropTypes');
var React = require('React');
var StyleSheet = require('StyleSheet');
var resolveAssetSource = require('resolveAssetSource');

/**
 * Used to render a 360 photos in a sphere.
 */
var Viro360Photo = React.createClass({
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
    ]).isRequired,
    rotation: PropTypes.arrayOf(PropTypes.number),

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
    var imgsrc = resolveAssetSource(this.props.source);
    if (this.props.src) {
      console.error('The <Viro360Photo> component takes a `source` property rather than `src`.');
    }

    if (this.props.onGaze) {
      console.error('The <Viro360Photo> component does not take on an `onGaze` property. Pass the `onGaze` prop to <ViroScene> instead.');
    }

    if (this.props.onTap) {
      console.error('The <Viro360Photo> component does not take on an `onTap` property. Pass the `onTap` prop to <ViroScene> instead.');
    }

    // Create native props object.
    let nativeProps = Object.assign({}, this.props);
    nativeProps.source = imgsrc;
    nativeProps.onViro360PhotoLoadStart = this._onLoadStart;
    nativeProps.onViro360PhotoLoadEnd = this._onLoadEnd;

    return (
      <VRO360Photo {...nativeProps} />
    );
  }
});


var VRO360Photo = requireNativeComponent(
  'VRO360Photo', Viro360Photo
);

module.exports = Viro360Photo;
