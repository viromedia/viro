
/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

import {
  ViroARScene,
  ViroARPlane,
  ViroImage,
  ViroARPlaneSelector,
  ViroQuad,
  ViroConstants,
  ViroNode,
  ViroBox,
  ViroOmniLight,
  ViroText,
  ViroUtils,
  ViroARTrackingTargets,
  ViroARImageMarker,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var ARImageTrackingTest = createReactClass({
  getInitialState: function() {
    return {
      target : "variety",
      pauseUpdates : false,
    };
  },

  componentDidMount: function() {
    this.props.arSceneNavigator.viroAppProps.toggleNumberOfTrackedImages();
  },

  render: function() {
    return (
      <ViroARScene>
        <ViroARImageMarker target={"variety"}
            onAnchorFound={()=>{console.log("ARImageMarkerTest found marker!")}}
            onAnchorUpdated={()=>{console.log("ARImageMarkerTest updated marker!")}}
            onAnchorRemoved={()=>{console.log("ARImageMarkerTest removed marker!")}} >
          <ViroBox scale={[.189, .1, 0.245]} position={[0, .05, 0]}/>
        </ViroARImageMarker>

        {/* Release Menu */}
        <ViroText position={polarToCartesian([6, -30, -15])} text={"Release Menu"}
          style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
          transformBehaviors={["billboard"]}/>
      </ViroARScene>
    );
  },
});

var styles = StyleSheet.create({
  instructionText: {
      fontFamily: 'Arial',
      fontSize: 25,
      width: 2,
      color: '#cccccc',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});

ViroARTrackingTargets.createTargets({
  variety : {
    source : require("./res/ar_targets/variety_magazine.jpg"),
    orientation : "Up",
    physicalWidth : 0.189,
    height : .245,
  },
});

module.exports = ARImageTrackingTest;
