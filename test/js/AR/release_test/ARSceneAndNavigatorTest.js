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
import { Platform } from 'react-native';
import { PermissionsAndroid } from 'react-native';

import {
  ViroARScene,
  ViroARPlane,
  ViroMaterials,
  ViroImage,
  ViroARPlaneSelector,
  ViroQuad,
  ViroConstants,
  ViroVideo,
  ViroText,
  ViroUtils
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

let pointCloudDict = {
  imageSource: require("../../res/blue_star.png"),
  imageScale : [.1, .1, .1],
  maxPoints : 50
};

var projectPoint = [1, 1, -4];
var unprojectPoint = [200, 500, .1];

var testARScene = createReactClass({
  getInitialState: function() {
    return {
      isRecording : false,
      saveToCamera : false,
      videoSuccess: false,
      initialized : false,
      ambientLightText: "Ambient Light",
      displayPointCloud : pointCloudDict,
      pointCloudPoints : 0,
      worldOriginChanged : false,
      trackingState :"",
      projectedPoint:[0,0,0],
      unprojectedPoint:[0,0,0],
    }
  },
  componentDidMount: function() {
    this._ambientIntensity = 0;
    this._ambientColor = "#FFFFFF";
    setInterval(
      ()=>{
        this.setState({
          ambientLightText : "Intensity: " + this._ambientIntensity + "\nColor: " + this._ambientColor
        })
      },
      500
    )
  },

  _onTrackingUpdated(state, reason) {
    var strState = "NONE";
    if (state == ViroConstants.TRACKING_NORMAL){
      strState="Normal";
      this.setState({initialized : true});
    } else if (state == ViroConstants.TRACKING_LIMITED){
      strState="Limited";
      this.setState({initialized : false})
    }

    var strReason = "NONE";
    if (reason == ViroConstants.TRACKING_REASON_EXCESSIVE_MOTION){
      strReason="Excessive Motion";
    } else if (reason == ViroConstants.TRACKING_REASON_INSUFFICIENT_FEATURES){
      strReason="Insufficent Features";
    }

    var trackingTag = "Tracking state: " + strState + "\nReason: " + strReason;
    this.setState({
      trackingState : trackingTag,
    });
  },

  render: function() {
    return (
      <ViroARScene
        onAmbientLightUpdate={this._onAmbientLightUpdate}
        displayPointCloud={this.state.displayPointCloud}
        onTrackingUpdated={this._onTrackingUpdated}
        onARPointCloudUpdate={this._onARPointCloudUpdate} >

        {/* ViroARSceneNavigator tests */}
        <ViroText position={polarToCartesian([6, 0, 40])} text={"Unproject point(200, 500, .1)==>(" +this.state.unprojectedPoint[0] + "," + this.state.unprojectedPoint[1] + "," + this.state.unprojectedPoint[2] + ")"}
          style={styles.instructionText} onClick={this._unprojectPoint} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, 30])} text={"Projected point(1, 1, -4)==>(" +this.state.projectedPoint[0] + "," + this.state.projectedPoint[1] + "," + this.state.projectedPoint[2] + ")"}
          style={styles.instructionText} onClick={this._projectPoint} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, 20])} text={this.state.trackingState}
          style={styles.instructionText} onClick={this._startStopRecording} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, 10])} text={"Recording? " + (this.state.isRecording ? "Yes" : "No")}
          style={styles.instructionText} onClick={this._startStopRecording} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, 0])} text={"Take Screenshot"}
          style={styles.instructionText} onClick={this._takeScreenshot} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, -10])} text={"saveToCamera? " + (this.state.saveToCamera ? "Yes" : "No")}
          style={styles.instructionText} onClick={this._flipSaveToCamera} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, -20])} text={"World Origin State: " + (this.state.worldOriginChanged ? "[0,0,1]" : "[0,0,0]")}
          style={styles.instructionText} onClick={this._changeWorldOrigin} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, -30])} text={"Evict Image From Cache (Android)"}
          style={styles.instructionText} onClick={this._evictFromCache} transformBehaviors={["billboard"]}/>

        {this._getVideo()}
        {this._getScreenshot()}

        {/* ViroARScene tests */}
        <ViroText position={polarToCartesian([6, 30, 10])} text={"Initialized? " + (this.state.initialized ? "Yes" : "No")}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 30, 0])} text={this.state.ambientLightText}
          style={styles.ambientLightText} transformBehaviors={["billboard"]}
          ref={component=>{this._ambientLightText = component}}/>
        <ViroText ref={(component)=>{this.pointCloudPointsText = component}}
          position={polarToCartesian([6, 30, -20])} text={"# of PointCloud points: " + this.state.pointCloudPoints}
          style={styles.instructionText} transformBehaviors={["billboard"]} />
        <ViroText position={polarToCartesian([6, 30, -30])} text={this.state.displayPointCloud ? "Hide Point Cloud" : "Show Point Cloud"}
          style={styles.instructionText} transformBehaviors={["billboard"]}
          onClick={()=>{this.setState({displayPointCloud : !this.state.displayPointCloud})}}/>
        <ViroText position={polarToCartesian([6, 30, -40])} text={"Toggle Autofocus/Quality"}
          style={styles.instructionText} transformBehaviors={["billboard"]}
          onClick={()=>{this.props.arSceneNavigator.viroAppProps.toggleAutofocus();
                        this.props.arSceneNavigator.viroAppProps.toggleVideoQuality()}}/>


        {/* Release Menu */}
        <ViroText position={polarToCartesian([6, -30, 0])} text={"Next test"}
          style={styles.instructionText} onClick={this._goToNextTest} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, -30, -15])} text={"Release Menu"}
          style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
          transformBehaviors={["billboard"]}/>
      </ViroARScene>
    );
  },
  _evictFromCache() {
    if (this.state.screenshot != undefined) {
      ViroImage.evictFromCache({uri : "file://" + this.state.screenshot});
    }
  },

  _projectPoint() {
    this.props.arSceneNavigator.project(projectPoint).then((rectDict)=> {
      this.setState({
          projectedPoint: rectDict["screenPosition"],
      });
    });
  },

  _unprojectPoint() {
    this.props.arSceneNavigator.unproject(unprojectPoint).then((rectDict)=> {
        this.setState({
            unprojectedPoint: rectDict["position"],
        });
    });
  },

  _changeWorldOrigin() {
    this.props.arSceneNavigator.setWorldOrigin({
      position : this.state.worldOriginChanged ? [0, 0, 1] : [0, 0, -1]
    });

    this.setState({
      worldOriginChanged : !this.state.worldOriginChanged
    })
  },
  _onARPointCloudUpdate(pointCloud) {
    this.setState({
      pointCloudPoints : pointCloud.points.length
    })
  },
  _getVideo() {
    if (this.state.videoSuccess) {
      return (
        <ViroVideo position={[0,-.6,-1.5]}
        scale={[.5, .5, 1]}
        onClick={this._resetVideo}
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
      return (
        <ViroImage
        position={[1,-.6,-1.5]}
        scale={[.5, .5, 1]}
        onError={this._onError}
        source={{"uri" : "file://" + this.state.screenshot}}
        transformBehaviors={["billboard"]} />)
    }
  },

  _onError(error){
      console.log("Screenshot image load error: " + error);
  },

  _onAmbientLightUpdate(retDict) {
    // setting native props is too insane/quick...
    // maybe if we cast it to an int it's easier to see, for now just use timer
    this._ambientIntensity = retDict.intensity;
    this._ambientColor = retDict.color;
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
      fontSize: 25,
      color: '#AAAAAA',
      width: 2,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
  ambientLightText: {
      fontFamily: 'Arial',
      fontSize: 25,
      color: '#AAAAAA',
      width: 3.5,
      height: 3,
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
