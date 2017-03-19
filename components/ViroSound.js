/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroSound
 * @flow
 */
'use strict';

import { requireNativeComponent, View, findNodeHandle, Platform } from 'react-native';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
import React from 'react';

var PropTypes = require('react/lib/ReactPropTypes');
var NativeModules = require('react-native').NativeModules;
var SoundModule = NativeModules.VRTSoundModule;
var RCT_SOUND_REF = 'virosoundcomponent';

var ViroSound = React.createClass({
  statics: {
    preloadSounds: function(soundMap:{[key:string]: string}) {
      SoundModule.preloadSounds(soundMap);
    },
    unloadSounds: function(soundKeys: [string]) {
      SoundModule.unloadSounds(soundKeys);
    }
  },
  propTypes: {
    ...View.propTypes,

    // Source can either be a String referencing a preloaded file, a web uri, or a
    // local js file (using require())
    source: PropTypes.oneOfType([
      PropTypes.string,
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./sound.mp3')
      PropTypes.number,
    ]).isRequired,

    paused: PropTypes.bool,
    loop: PropTypes.bool,
    muted: PropTypes.bool,
    volume: PropTypes.number,
    onFinish: React.PropTypes.func,
    onError: React.PropTypes.func,
  },

  _onFinish: function(event: Event) {
    this.props.onFinish && this.props.onFinish(event);
  },

  _onError: function(event: Event) {
    this.props.onError && this.props.onError(event);
  },

  render: function() {

    var soundSrc = this.props.source;
    if (typeof soundSrc === 'number') {
      soundSrc = resolveAssetSource(soundSrc);
    } else if (typeof soundSrc === 'string') {
      soundSrc = {name: soundSrc};
    }

    let nativeProps = Object.assign({}, this.props);
    nativeProps.ref = RCT_SOUND_REF;
    nativeProps.source = soundSrc;
    nativeProps.onFinishViro = this._onFinish;
    nativeProps.onErrorViro = this._onError;

    return (
      <VRTSound {...nativeProps} />
    );
  },

  getNodeHandle: function(): any {
    return findNodeHandle(this.refs[RCT_SOUND_REF]);
  },

  seekToTime(timeInSeconds) {
    switch (Platform.OS) {
      case 'ios':
        NativeModules.VRTSoundManager.seekToTime(this.getNodeHandle(), timeInSeconds);
        break;
      case 'android':
        NativeModules.UIManager.dispatchViewManagerCommand(
            this.getNodeHandle(),
            NativeModules.UIManager.VRTSound.Commands.seekToTime,
            [ timeInSeconds ]);
        break;
    }
  },
});

var VRTSound = requireNativeComponent(
  'VRTSound', ViroSound, {
    nativeOnly: {
      onFinishViro: true,
      onErrorViro: true,
    }
  }
);

module.exports = ViroSound;
