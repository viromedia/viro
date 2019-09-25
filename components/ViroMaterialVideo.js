/**
 * Copyright (c) 2018-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import {
  requireNativeComponent,
  View,
  StyleSheet,
  findNodeHandle,
  Platform,
} from 'react-native';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
import React, { Component } from 'react';
var NativeModules = require('react-native').NativeModules;
var createReactClass = require('create-react-class');
import PropTypes from 'prop-types';
import { checkMisnamedProps } from './Utilities/ViroProps';

var ViroMaterialVideo = createReactClass({
  propTypes: {
    ...View.propTypes,

    material:PropTypes.string,
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
  },

  componentWillUnmount() {
      // pause the current video texture on Android since java gc will release when it feels like it.
      if (Platform.OS == 'android') {
        NativeModules.UIManager.dispatchViewManagerCommand(
            findNodeHandle(this),
            NativeModules.UIManager.VRTMaterialVideo.Commands.pause, [ 0 ]);
      }
  },

  _onBufferStart: function(event: Event) {
    this.props.onBufferStart && this.props.onBufferStart(event);
  },

  _onBufferEnd: function(event: Event) {
    this.props.onBufferEnd && this.props.onBufferEnd(event);
  },

  _onFinish: function() {
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


    // Since materials and transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    //let materials = typeof this.props.materials === 'string' ? new Array(this.props.materials) : this.props.materials;

    let nativeProps = Object.assign({}, this.props);
    //nativeProps.materials = materials;
    nativeProps.onBufferStartViro = this._onBufferStart;
    nativeProps.onBufferEndViro = this._onBufferEnd;
    nativeProps.onFinishViro = this._onFinish;
    nativeProps.onErrorViro = this._onError;
    nativeProps.onUpdateTimeViro = this._onUpdateTime;
    nativeProps.ref = component => {this._component = component; };
    return (
      <VRTMaterialVideo {...nativeProps} />
    );
  },

  seekToTime(timeInSeconds) {
    switch (Platform.OS) {
      case 'ios':
        NativeModules.VRTMaterialVideoManager.seekToTime(findNodeHandle(this), timeInSeconds);
        break;
      case 'android':
        NativeModules.UIManager.dispatchViewManagerCommand(
            findNodeHandle(this),
            NativeModules.UIManager.VRTMaterialVideo.Commands.seekToTime,
            [ timeInSeconds ]);
        break;
    }
  },
});

var VRTMaterialVideo = requireNativeComponent(
    'VRTMaterialVideo', ViroMaterialVideo, {
      nativeOnly: {
          onBufferStartViro: true,
          onBufferEndViro: true,
          onUpdateTimeViro: true,
          onFinishViro: true,
          onErrorViro:true,
      }
    }
);

module.exports = ViroMaterialVideo;
