
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
  ViroARObjectMarker,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var ARImageMarkerTest = createReactClass({
  getInitialState: function() {
    return {
      showObjectMarker : true,
      pauseUpdates : false,
      targetTwo : 0,
    };
  },
  render: function() {

    var target = targetsList[this.state.targetTwo];

    return (
      <ViroARScene>

        {/* Image Marker component*/}

        <ViroText position={polarToCartesian([6, 20, 10])} text={"Object Marker: " + (this.state.showObjectMarker ? "Visible" : "Not Visible")}
          style={styles.instructionText} onClick={this._toggleShowObjectMarker} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 20, 0])} text={"Switch Target, current: " + target}
          style={styles.instructionText} onClick={this._toggleTargetTwo} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 20, -10])} text={"Pause Updates: " + (this.state.pauseUpdates ? "Yes" : "No")}
          style={styles.instructionText} onClick={this._togglePauseUpdates} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 20, -20])} text={"Test Delete Obj Target"}
          style={styles.instructionText} onClick={this._deleteTarget} transformBehaviors={["billboard"]}/>

        <ViroARObjectMarker target={target}
          pauseUpdates={this.state.pauseUpdates}
          onAnchorFound={()=>{console.log("ARImageMarkerTest found marker!")}}
          onAnchorUpdated={()=>{console.log("ARImageMarkerTest updated marker!")}}
          onAnchorRemoved={()=>{console.log("ARImageMarkerTest removed marker!")}} >
          <ViroBox scale={[.1,.1,.1]} position={[0, .05, 0]}/>
        </ViroARObjectMarker>


        {/* Release Menu */}
        <ViroText position={polarToCartesian([6, -30, 0])} text={"Next test"}
          style={styles.instructionText} onClick={this._goToNextTest} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, -30, -15])} text={"Release Menu"}
          style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
          transformBehaviors={["billboard"]}/>

      </ViroARScene>
    );
  },
  _deleteTarget() {
    ViroARTrackingTargets.deleteTarget("to_delete")
  },
  _togglePauseUpdates() {
    this.setState({
      pauseUpdates : !this.state.pauseUpdates
    })
  },
  _toggleShowObjectMarker() {
    this.setState({
      showObjectMarker : !this.state.showObjectMarker
    })
  },
  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARSceneAndNavigatorTest", {scene:require("./ARSceneAndNavigatorTest")})
  },
  _toggleTargetTwo() {
    console.log("[ARImageMarkerTest] toggling target two!");

    this.setState({
      targetTwo : (this.state.targetTwo == targetsList.length - 1) ? 0 : this.state.targetTwo + 1
    })
  }
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

var newTargets = {
  google_home : {
    source : require("./res/ar_targets/google_home.arobject"),
    type : 'Object'
  },
  alcohol_bottle : {
    source : require("./res/ar_targets/alcohol_bottle.arobject"),
    type : 'Object'
  },
  diet_coke : {
    source : require("./res/ar_targets/diet_coke.arobject"),
    type : 'Object'
  },
  to_delete : {
    source : require("./res/ar_targets/google_home.arobject"),
    type : 'Object'
  }
}

ViroARTrackingTargets.createTargets(newTargets);

var targetsList = ['alcohol_bottle', 'google_home', 'diet_coke'];

module.exports = ARImageMarkerTest;
