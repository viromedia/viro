/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule Viro360Video
 * @flow
 */
'use strict';

import { requireNativeComponent, View, StyleSheet, findNodeHandle} from 'react-native';
import React, { Component } from 'react';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
var PropTypes = require('react/lib/ReactPropTypes');
var Viro360VideoManager = require('react-native').NativeModules.Video360Manager;

var RCT_360_VIDEO_REF = 'viro360videocomponent';

/**
 * Used to render a 360 video on the background sphere.
 */
var Viro360Video = React.createClass({
  propTypes: {
    ...View.propTypes,

    /**
     * The video uri to play
     */
    source: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./test_video.mp4')
      PropTypes.number,
    ]).isRequired,

    paused: PropTypes.bool,
    loop: PropTypes.bool,
    muted: PropTypes.bool,
    volume: PropTypes.number,

    /**
     * Callback that is called when the video is finished playing. This
     * function isn't called at the end of a video if looping is enabled.
     */
    onFinish: React.PropTypes.func,

    /**
      * Callback that is called when the current playback position has changed.
      * This is called in the form:
      *     onUpdateTime(currentPlaybackTimeInSeconds, totalPlayBackDurationInSeconds);
      */
    onUpdateTime: React.PropTypes.func,
  },

  getNodeHandle: function(): any {
    return findNodeHandle(this.refs[RCT_360_VIDEO_REF]);
  },

  _onFinish() {
    this.props.onFinish && this.props.onFinish();
  },

  _onUpdateTime: function(event: Event) {
    this.props.onUpdateTime && this.props.onUpdateTime(event.nativeEvent.currentTime, event.nativeEvent.totalTime);
  },

  render: function() {
    if (this.props.src) {
      console.error('The <Viro360Video> component takes a `source` property rather than `src`.');
    }

    var vidsrc = resolveAssetSource(this.props.source);

    let nativeProps = Object.assign({}, this.props);
    nativeProps.ref = RCT_360_VIDEO_REF;
    nativeProps.source = vidsrc;
    nativeProps.onFinishViro = this._onFinish;
    nativeProps.onUpdateTimeViro = this._onUpdateTime;
    return (
      <VRO360Video {...nativeProps} />
    );
  },

  seekToTime(timeInSeconds) {
    Viro360VideoManager.seekToTime(this.getNodeHandle(), timeInSeconds);
  },
});

var VRO360Video = requireNativeComponent(
  'VRT360Video', Viro360Video, {
    nativeOnly: {onUpdateTimeViro: true, onFinishViro: true}
  }
);

module.exports = Viro360Video;
