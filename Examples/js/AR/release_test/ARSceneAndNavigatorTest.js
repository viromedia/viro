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
  ViroConstants,
  ViroVideo,
  ViroText,
  ViroUtils
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

let pointCloudDict = {
  imageSource: require("../../res/blue_star.png"),
  imageScale : [.1, .1, .1],
  maxPoints : 50
};

var testARScene = createReactClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      isRecording : false,
      saveToCamera : false,
      videoSuccess: false,
      initialized : false,
      ambientLightText: "Ambient Light",
      displayPointCloud : pointCloudDict,
    }
  },
  componentDidMount: function() {
    this._intensity = 0;
    this._colorTemp = 0;
    this.setInterval(
      ()=>{
        this.setState({
          ambientLightText : "Intensity: " + this._intensity + ", ColorTemp: " + this._colorTemp
        })
      },
      500
    )
  },
  render: function() {
    return (
      <ViroARScene
        onTrackingInitialized={()=>{this.setState({initialized : true})}}
        onAmbientLightUpdate={this._onAmbientLightUpdate}
        displayPointCloud={this.state.displayPointCloud} >

        {/* ViroARSceneNavigator tests */}
        <ViroText position={polarToCartesian([2, 0, 10])} text={"Recording? " + (this.state.isRecording ? "Yes" : "No")}
          style={styles.instructionText} onClick={this._startStopRecording} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, 0, 0])} text={"Take Screenshot"}
          style={styles.instructionText} onClick={this._takeScreenshot} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, 0, -10])} text={"saveToCamera? " + (this.state.saveToCamera ? "Yes" : "No")}
          style={styles.instructionText} onClick={this._flipSaveToCamera} transformBehaviors={["billboard"]}/>
        {this._getVideo()}
        {this._getScreenshot()}

        {/* ViroARScene tests */}
        <ViroText position={polarToCartesian([2, 30, 10])} text={"initialized? " + (this.state.initialized ? "Yes" : "No")}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, 30, 0])} text={this.state.ambientLightText}
          style={styles.instructionText} transformBehaviors={["billboard"]}
          ref={component=>{this._ambientLightText = component}}/>
        <ViroText position={polarToCartesian([2, 30, -10])} text={this.state.displayPointCloud ? "Hide Point Cloud" : "Show Point Cloud"}
          style={styles.instructionText} transformBehaviors={["billboard"]}
          onClick={()=>{this.setState({displayPointCloud : !this.state.displayPointCloud})}}/>

        {/* Release Menu */}
        <ViroText position={polarToCartesian([2, -30, 0])} text={"Next test"}
          style={styles.instructionText} onClick={this._goToNextTest} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, -30, -15])} text={"Release Menu"}
          style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
          transformBehaviors={["billboard"]}/>
      </ViroARScene>
    );
  },
  _getVideo() {
    if (this.state.videoSuccess) {
      return (<ViroVideo position={[0,-.6,-1.5]}
        scale={[.5, .5, 1]}
        onClick={()=>{this._resetVideo}}
        source={{"uri" : "file://" + this.state.video}}
        ref={component=>{this._video = component}}
        transformBehaviors={["billboard"]}/>)
    }
  },
  _resetVideo() {
    if (this.state.videoSuccess) {
      this._video.seekToTime(0);
    }
  },
  _getScreenshot() {
    if (this.state.screenshotSuccess) {
      return (<ViroImage position={[1,-.6,-1.5]}
        scale={[.5, .5, 1]}
        source={{"uri" : "file://" + this.state.screenshot}}
        transformBehaviors={["billboard"]} />)
    }
  },
  _onAmbientLightUpdate(retDict) {
    // setting native props is too insane/quick...
    // maybe if we cast it to an int it's easier to see, for now just use timer
    //this._ambientLightText.setNativeProps({"text" : "Intensity: " + retDict.intensity + ", ColorTemp: " + retDict.colorTemperature})
    this._intensity = retDict.intensity;
    this._colorTemp = retDict.colorTemperature;
  },
  _startStopRecording() {
    this.setState({
      isRecording : !this.state.isRecording,
    }, ()=>{
      if (this.state.isRecording) {
        this._startRecording();
      } else {
        this._stopRecording();
      }
    });
  },
  _flipSaveToCamera() {
    this.setState({
      saveToCamera : !this.state.saveToCamera,
    })
  },
  _takeScreenshot() {
    // set screenshotSuccess to false before firing off a screenshot request.
    this.setState({
      screenshotSuccess : false,
    }, ()=>{
      this.props.arSceneNavigator.takeScreenshot("testScreenshot", this.state.saveToCamera)
        .then((retDict)=>{
          if (!retDict.success) {
            if (retDict.errorCode == ViroConstants.RECORD_ERROR_NO_PERMISSION) {
              console.log("[JS] screenshot error, had no permissions!");
            } else {
              console.log("[JS] other screenshot error: " + retDict.errorCode);
            }
          }
          this.setState({
            screenshotSuccess : retDict.success,
            screenshot : retDict.url,
          })
        });
    });
  },
  _startRecording() {
    // set videoSuccess to false before starting (no inception allowed here!)
    this.setState({
      videoSuccess : false,
    }, ()=>{
      this.props.arSceneNavigator.startVideoRecording("testVideo", this.state.saveToCamera,
         (errorCode)=>{console.log("[JS] onError callback errorCode: " + errorCode)});
    });
  },
  _stopRecording() {
    this.props.arSceneNavigator.stopVideoRecording().then((retDict)=>{
      if (!retDict.success) {
        if (retDict.errorCode == ViroConstants.RECORD_ERROR_NO_PERMISSION) {
          console.log("[JS] recording error, had no permissions!");
        } else {
          console.log("[JS] other recording error: " + retDict.errorCode);
        }
      }
      this.setState({
        videoSuccess : retDict.success,
        video : retDict.url,
      });
    });
  },
  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARDragTest", {scene:require("./ARDragTest")})
  },
});

var styles = StyleSheet.create({
  instructionText: {
      fontFamily: 'Arial',
      fontSize: 10,
      color: '#cccccc',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});

ViroMaterials.createMaterials({
  blue: {
    lightingModel: "Constant",
    diffuseColor: "#0000ff50"
  },
  red: {
    lightingModel: "Constant",
    diffuseColor: "#ff000050"
  },
});

module.exports = testARScene;
