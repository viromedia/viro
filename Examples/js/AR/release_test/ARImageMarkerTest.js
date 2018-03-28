
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
      showImageMarker : true,
      target : "george",
      pauseUpdates : false,
    };
  },
  render: function() {
    return (
      <ViroARScene>

        {/* Image Marker component*/}
        {this._getImageMarker()}


        <ViroText position={polarToCartesian([6, 0, 10])} text={"Image Marker: " + (this.state.showImageMarker ? "Visible" : "Not Visible")}
          style={styles.instructionText} onClick={this._toggleShowImageMarker} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, 0])} text={"Switch Target, current: " + this.state.target}
          style={styles.instructionText} onClick={this._toggleTarget} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, -10])} text={"Pause Updates: " + (this.state.pauseUpdates ? "Yes" : "No")}
          style={styles.instructionText} onClick={this._flipSaveToCamera} transformBehaviors={["billboard"]}/>

        {/* ViroARScene tests
        <ViroText position={polarToCartesian([6, 30, 10])} text={"initialized? " + (this.state.initialized ? "Yes" : "No")}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 30, 0])} text={this.state.ambientLightText}
          style={styles.instructionText} transformBehaviors={["billboard"]}
          ref={component=>{this._ambientLightText = component}}/>
        <ViroText ref={(component)=>{this.pointCloudPointsText = component}}
          position={polarToCartesian([6, 30, -10])} text={"# of PointCloud points: " + this.state.pointCloudPoints}
          style={styles.instructionText} transformBehaviors={["billboard"]} />
        <ViroText position={polarToCartesian([6, 30, -20])} text={this.state.displayPointCloud ? "Hide Point Cloud" : "Show Point Cloud"}
          style={styles.instructionText} transformBehaviors={["billboard"]}
          onClick={()=>{this.setState({displayPointCloud : !this.state.displayPointCloud})}}/>*/}

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
    this.setState({
      target : (this.state.target == "ben" ? "george" : "ben")
    })
  },
  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARSceneAndNavigatorTest", {scene:require("./ARSceneAndNavigatorTest")})
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
  ben : {
    source : require('./res/ben.jpg'),
    orientation : "Up",
    physicalWidth : 0.157 // real world width in meters
  },
  george : {
    source : require('./res/george.jpg'),
    orientation : "Up",
    physicalWidth : 0.157 // real world width in meters
  }
});

module.exports = ARImageMarkerTest;
