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
  ViroQuad,
  ViroConstants,
  ViroVideo,
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var createReactClass = require('create-react-class');

var testARScene = createReactClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      text : "not tapped",
      visible: true,
      everythingVisible: false,
      success: false,
      video : "",
    }
  },
  render: function() {
    return (
        <ViroARScene position={[0,0,0]} reticleEnabled={false} >
          <ViroQuad scale={[.5,.5,.5]} visible={!this.state.success} materials={"green"} position={[0, .5, -1]} onClick={this._startRecording}/>
          <ViroQuad scale={[.5,.5,.5]} visible={!this.state.success} materials={"red"} position={[.5, .5, -1]} onClick={this._stopRecording}/>
          <ViroARPlaneSelector ref={"planeSelector"}
            maxPlanes={2}
            onPlaneSelected={()=>{console.log("plane was selected")}} >
            <ViroImage
              width={1.15} height={3}
              position={[0, 1.5, 0]}
              source={require("./res/ironman.png")}
              onClick={this._onClick}
              />
          </ViroARPlaneSelector>
          {this._getVideo()}
        </ViroARScene>
    );
  },
  _startRecording() {
    console.log("kirby ViroConstants " + ViroConstants.RECORD_ERROR_NONE)
    console.log("[JS] begin recording!");
    this.props.arSceneNavigator.startVideoRecording("testVid11", true,
       (errorCode)=>{console.log("[JS] onError callback errorCode: " + errorCode)});
  },
  _stopRecording() {
    this.props.arSceneNavigator.stopVideoRecording().then((retDict)=>{
      console.log("[JS] success? " + retDict.success);
      console.log("[JS] the url was: " + retDict.url);
      if (!retDict.success) {
        if (retDict.errorCode == ViroConstants.RECORD_ERROR_NO_PERMISSION) {
          console.log("[JS] had no permissions!");
        }
      }
      this.setState({
        success : retDict.success,
        video : retDict.url,
      });
    });
  },
  _onClick() {
    this.refs["planeSelector"].reset();
  },
  _getVideo() {
    if (this.state.success) {
      return (<ViroVideo position={[0,0,-.9]} onClick={()=>{this.setState({success : false})}}
        source={{"uri" : "file://" + this.state.video}}/>)
    }
  }
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
