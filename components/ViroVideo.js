/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import { requireNativeComponent, View, StyleSheet } from 'react-native';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
import React, { Component } from 'react';
var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var PropTypes = require('react/lib/ReactPropTypes');
var ViroVideoManager = require('react-native').NativeModules.VideoSurfaceManager;
var findNodeHandle = require('react/lib/findNodeHandle');
var RCT_VIDEO_REF = 'virovideocomponent';

var ViroVideo = React.createClass({
  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    opacity: PropTypes.number,
    visible: PropTypes.bool,
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    width: PropTypes.number,
    height: PropTypes.number,
    paused: PropTypes.bool,
    loop: PropTypes.bool,
    muted: PropTypes.bool,
    volume: PropTypes.number,
    source: PropTypes.oneOfType([
        PropTypes.shape({
            uri: PropTypes.string,
        }),
        // Opaque type returned by require('./test_video.mp4')
        PropTypes.number
    ]),

    /**
     * Callback that is called when the video is finished playing. This
     * function isn't called at the end of a video if looping is enabled.
     */
    onFinish: React.PropTypes.func
  },

  getNodeHandle: function(): any {
    return findNodeHandle(this.refs[RCT_VIDEO_REF]);
  },

  _onFinish: function(event: Event) {
    this.props.onFinish && this.props.onFinish(event);
  },

  render: function() {
    if (this.props.src) {
      console.error('The <ViroVideo> component takes a `source` property rather than `src`.');
    }

    if (this.props.material) {
      console.error('The <ViroVideo> component takes a `materials` property rather than `material`.');
    }

    var source = resolveAssetSource(this.props.source);
    // Since materials and transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let materials = typeof this.props.materials === 'string' ? new Array(this.props.materials) : this.props.materials;
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;

    let nativeProps = Object.assign({}, this.props);
    nativeProps.ref = RCT_VIDEO_REF;
    nativeProps.style = [this.props.style];
    nativeProps.source = source;
    nativeProps.materials = materials;
    nativeProps.transformBehaviors = transformBehaviors;
    nativeProps.onFinishViro = this._onFinish;

    return (
      <VRTVideoSurface {...nativeProps} />
    );
  },

  seekToTime(timeInSeconds) {
    ViroVideoManager.seekToTime(this.getNodeHandle(), timeInSeconds);
  },
});

var VRTVideoSurface = requireNativeComponent(
    'VRTVideoSurface', ViroVideo, {
      nativeOnly: {onFinishViro: true}
    }
);

module.exports = ViroVideo;
