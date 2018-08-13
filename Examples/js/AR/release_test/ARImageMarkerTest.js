
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

var ARImageMarkerTest = createReactClass({
  getInitialState: function() {
    return {
      showImageMarker : false,
      target : "george",
      pauseUpdates : false,
      targetTwo : 0,
    };
  },
  render: function() {

    var target = targetsList[this.state.targetTwo];

    return (
      <ViroARScene>

        {/* Image Marker component*/}
        {this._getImageMarker()}

        <ViroText position={polarToCartesian([6, 20, 10])} text={"Image Marker: " + (this.state.showImageMarker ? "Visible" : "Not Visible")}
          style={styles.instructionText} onClick={this._toggleShowImageMarker} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 20, 0])} text={"Switch Target, current: " + this.state.target}
          style={styles.instructionText} onClick={this._toggleTarget} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 20, -10])} text={"Pause Updates: " + (this.state.pauseUpdates ? "Yes" : "No")}
          style={styles.instructionText} onClick={this._flipSaveToCamera} transformBehaviors={["billboard"]}/>


        <ViroText position={polarToCartesian([6, -7, 10])} text={"Image Marker #2"}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, -7, 0])} text={"Switch Target, current: " + target}
          style={styles.instructionText} onClick={this._toggleTargetTwo} transformBehaviors={["billboard"]}/>

        <ViroARImageMarker target={target}
            onAnchorFound={()=>{console.log("ARImageMarkerTest found marker!")}}
            onAnchorUpdated={()=>{console.log("ARImageMarkerTest updated marker!")}}
            onAnchorRemoved={()=>{console.log("ARImageMarkerTest removed marker!")}} >
          <ViroBox scale={[newTargets[target].physicalWidth, .1, newTargets[target].height]} position={[0, .05, 0]}/>
        </ViroARImageMarker>

        {/* Release Menu */}
        <ViroText position={polarToCartesian([6, -30, 0])} text={"Next test"}
          style={styles.instructionText} onClick={this._goToNextTest} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, -30, -15])} text={"Release Menu"}
          style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
          transformBehaviors={["billboard"]}/>

      </ViroARScene>
    );
  },
  _getImageMarker() {
    if (this.state.showImageMarker) {
      return (
        <ViroARImageMarker target={this.state.target}
            onAnchorFound={()=>{console.log("ARImageMarkerTest found marker!")}}
            onAnchorUpdated={()=>{console.log("ARImageMarkerTest updated marker!")}}
            onAnchorRemoved={()=>{console.log("ARImageMarkerTest removed marker!")}}
            pauseUpdates={this.state.pauseUpdates}>
          <ViroBox scale={[0.157, .07, 0.066294]} position={[0, .035, 0]} />
        </ViroARImageMarker>
      );
    }
  },
  _toggleShowImageMarker() {
    this.setState({
      showImageMarker : !this.state.showImageMarker
    })
  },
  _toggleTarget() {
    console.log("[ARImageMarkerTest] toggling target one!");
    this.setState({
      target : (this.state.target == "ben" ? "george" : "ben")
    })
  },
  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARObjectMarkerTest", {scene:require("./ARObjectMarkerTest")})
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

ViroARTrackingTargets.createTargets({
  ben : {
    source : require('./res/ben.jpg'),
    orientation : "Up",
    physicalWidth : 0.157 // real world width in meters
  },
  george : {
    source : require('./res/george.jpg'),
    orientation : "Up",
    physicalWidth : 0.157 // real world width in meters
  },
});

var newTargets = {
  vanityfair : {
    source : require("./res/ar_targets/vanityfair_jlaw.jpg"),
    orientation : "Up",
    physicalWidth : 0.21,
    height : .28
  },
  variety : {
    source : require("./res/ar_targets/variety_magazine.jpg"),
    orientation : "Up",
    physicalWidth : 0.189,
    height : .245,
  },
  blackpanther : {
    source : require("./res/ar_targets/poster_computer.jpg"),
    orientation : "Up",
    physicalWidth : 0.189,
    height : .28,
  },
  kitkat : {
    source : require("./res/ar_targets/kitkat.jpg"),
    orientation : "Up", 
    physicalWidth : 0.087,
    height : .06
  },
  nytimes : {
    source : require("./res/ar_targets/nytimes_magazine.jpg"),
    orientation : "Up",
    physicalWidth : 0.215,
    height: .26,
  },
  painting : {
    source : require("./res/ar_targets/painting_wall.jpg"),
    orientation : "Up",
    physicalWidth : 0.22,
    height : .165
  }
}

ViroARTrackingTargets.createTargets(newTargets);

var targetsList = ['vanityfair', 'variety', 'blackpanther', 'kitkat', 'nytimes', 'painting'];

module.exports = ARImageMarkerTest;
