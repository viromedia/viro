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

import {
  requireNativeComponent,
  View,
  StyleSheet,
  findNodeHandle,
  Platform
} from 'react-native';
import React, { Component } from 'react';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";

import { checkMisnamedProps } from './Utilities/ViroProps';
import PropTypes from 'prop-types';
var NativeModules = require('react-native').NativeModules;
var createReactClass = require('create-react-class');

/**
 * Used to render a 360 video on the background sphere.
 */
var Viro360Video = createReactClass({ 
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

    rotation: PropTypes.arrayOf(PropTypes.number),
    paused: PropTypes.bool,
    loop: PropTypes.bool,
    muted: PropTypes.bool,
    volume: PropTypes.number,

    /**
     * Callback invoked when the underlying video component begins buffering. Called at
     * least once at the beginning of playback/video creation.
     */
    onBufferStart: PropTypes.func,

    /**
     * Callback invoked when the underlying video component has finished buffering.
     */
    onBufferEnd: PropTypes.func,

    /**
     * Callback that is called when the video is finished playing. This
     * function isn't called at the end of a video if looping is enabled.
     */
    onFinish: PropTypes.func,

    /**
      * Callback that is called when the current playback position has changed.
      * This is called in the form:
      *     onUpdateTime(currentPlaybackTimeInSeconds, totalPlayBackDurationInSeconds);
      */
    onUpdateTime: PropTypes.func,

    /**
     * Callback triggered when the video fails to load. Invoked with
     * {nativeEvent: {error}}
     */
    onError: PropTypes.func,
    stereoMode:PropTypes.oneOf(['LeftRight', 'RightLeft', 'TopBottom', 'BottomTop', 'None']),
  },

  _onBufferStart: function(event: Event) {
    this.props.onBufferStart && this.props.onBufferStart(event);
  },

  _onBufferEnd: function(event: Event) {
    this.props.onBufferEnd && this.props.onBufferEnd(event);
  },

  _onFinish() {
    this.props.onFinish && this.props.onFinish();
  },

  _onError: function(event: Event) {
    this.props.onError && this.props.onError(event);
  },

  _onUpdateTime: function(event: Event) {
    this.props.onUpdateTime && this.props.onUpdateTime(event.nativeEvent.currentTime, event.nativeEvent.totalTime);
  },

  setNativeProps: function(nativeProps) {
    this._component.setNativeProps(nativeProps);
  },

  render: function() {

    checkMisnamedProps("Viro360Video", this.props);

    var vidsrc = resolveAssetSource(this.props.source);

    let nativeProps = Object.assign({}, this.props);
    nativeProps.source = vidsrc;
    nativeProps.onBufferStartViro = this._onBufferStart;
    nativeProps.onBufferEndViro = this._onBufferEnd;
    nativeProps.onErrorViro = this._onError;
    nativeProps.onFinishViro = this._onFinish;
    nativeProps.onUpdateTimeViro = this._onUpdateTime;
    nativeProps.ref = component => {this._component = component; };
    return (
      <VRO360Video {...nativeProps} />
    );
  },

  seekToTime(timeInSeconds) {
    switch (Platform.OS) {
      case 'ios':
        NativeModules.VRT360VideoManager.seekToTime(findNodeHandle(this), timeInSeconds);
        break;
      case 'android':
        NativeModules.UIManager.dispatchViewManagerCommand(
            findNodeHandle(this),
            NativeModules.UIManager.VRT360Video.Commands.seekToTime,
            [ timeInSeconds ]);
        break;
    }
  },
});

var VRO360Video = requireNativeComponent(
  'VRT360Video', Viro360Video, {
    nativeOnly: {
      onBufferStartViro: true,
      onBufferEndViro: true,
      onUpdateTimeViro: true,
      onErrorViro:true,
      onFinishViro: true}
  }
);

module.exports = Viro360Video;
