/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import { requireNativeComponent, View } from 'react-native';

var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var NativeModules = require('NativeModules');
var PropTypes = require('react/lib/ReactPropTypes');
var React = require('React');
var StyleSheet = require('StyleSheet');
var ViroVideoManager = require('NativeModules').VideoSurfaceManager;
var resolveAssetSource = require('resolveAssetSource');
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
    return (
      <VRTVideoSurface ref={RCT_VIDEO_REF}
        style={[this.props.style]} {...this.props} source={source} onFinish={this._onFinish}
      />
    );
  },

  seekToTime(timeInSeconds) {
    ViroVideoManager.seekToTime(this.getNodeHandle(), timeInSeconds);
  },

  _onFinish(event: Event) {
      if (!this.props.onFinish) {
        return;
      }

      this.props.onFinish();
    }
});

var VRTVideoSurface = requireNativeComponent(
    'VRTVideoSurface', ViroVideo, {
      nativeOnly: {onFinish: true}
    }

);

module.exports = ViroVideo;
