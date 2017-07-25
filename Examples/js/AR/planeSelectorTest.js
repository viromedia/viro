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
  ViroMaterials,
  ViroImage,
  ViroARPlaneSelector,
  ViroSurface,
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var testARScene = React.createClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      text : "not tapped",
      visible: true,
      everythingVisible: false
    }
  },
  render: function() {
    return (
        <ViroARScene position={[0,0,0]} reticleEnabled={false} >
          <ViroSurface scale={[.5,.5,.5]} materials={"green"} position={[0, .5, -1]} onClick={this._startRecording}/>
          <ViroSurface scale={[.5,.5,.5]} materials={"red"} position={[.5, .5, -1]} onClick={this._stopRecording}/>
          <ViroARPlaneSelector ref={"ref"}
            maxPlanes={2}
            onPlaneSelected={()=>{console.log("plane was selected")}} >
            <ViroImage
              width={1.15} height={3}
              position={[0, 1.5, 0]}
              source={require("./res/ironman.png")}
              onClick={this._onClick}
              />
          </ViroARPlaneSelector>
        </ViroARScene>
    );
  },
  _startRecording() {
    console.log("kirby begin recording!");
    this.props.sceneNavigator.startVideoRecording("testVid11.mp4", true);
  },
  _stopRecording() {
    this.props.sceneNavigator.stopVideoRecording().then((url)=>{
      console.log("kirby the url was: " + url);
    });
  },
  _onClick() {
    this.refs["ref"].reset();
  },
});

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#FFFFFF',
  },
  welcome: {
    fontSize: 13,
    textAlign: 'center',
    color: '#ffffff',
    margin: 2,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

ViroMaterials.createMaterials({
  blue: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#0000ff"
  },
  black: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#000000"
  },
  red: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#ff0000"
  },
  green: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#00ff00"
  },
});

module.exports = testARScene;
