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
  ViroScene,
  ViroARScene,
  ViroARPlane,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroImage,
  ViroVideo,
  ViroUtils,
  ViroSkyBox,
  Viro360Video,
  ViroText,
  ViroQuad
} from 'react-viro';


var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;


var testARScene = createReactClass({
  getInitialState: function() {
    return {
      surfaceSize : [0,0,0],
      center : [0,0,0],
      testWithRay : true,
    }
  },
  render: function() {
    return (
        <ViroARScene ref="arscene" onClick={this.state.testWithRay ? this._onSurfaceClickCameraForward : this._onSurfaceClickUsingPosition} >
          <ViroText position={polarToCartesian([6, -10, 0])} text={"Mode: " + (this.state.testWithRay ? "testWithRay" : "testWithPosition")}
            style={styles.baseTextTwo} onClick={this._switchRayTestType} transformBehaviors={["billboard"]}/>
          <ViroARPlane onAnchorUpdated={this._onPlaneUpdate}>
            <ViroQuad materials={"transparent"} position={this.state.center} scale={this.state.surfaceSize}
             rotation={[-90, 0, 0]} />
          </ViroARPlane>
          {this._getBox()}

          {/* Release Menu */}
          <ViroText position={polarToCartesian([6, -30, 0])} text={"Next test"}
            style={styles.instructionText} onClick={this._goToNextTest} transformBehaviors={["billboard"]}/>
          <ViroText position={polarToCartesian([6, -30, -15])} text={"Release Menu"}
            style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
            transformBehaviors={["billboard"]}/>
        </ViroARScene>
    );
  },
  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARScene/Navigator", {scene:require("./ARSceneAndNavigatorTest")})
  },
  _switchRayTestType() {
    this.setState({
      testWithRay : !this.state.testWithRay
    })
  },
  _getBox() {
    if (this.state.boxLocation != undefined) {
      return (
        <ViroNode position={this.state.boxLocation} >
          <ViroBox position={[0,.075,0]} scale={[.15,.15,.15]} />
        </ViroNode>
      )
    }
  },
  _onPlaneUpdate(updateDict) {
    this.setState({
      surfaceSize : [updateDict.width, updateDict.height, 1],
      center : updateDict.center,
    })
  },
  _onSurfaceClickWithPoint() {
    this.refs["arscene"].performARHitTestWithPoint(320, 568).then((results)=>{
      if (results.length > 0) {
        for (var i = 0; i < results.length; i++) {
          let result = results[i];
          if (result.type == "ExistingPlaneUsingExtent") {
            this.setState({
              boxLocation : result.transform.position
            });
            return;
          }
        }
      }
    })
  },
  _onSurfaceClickCameraForward() {
    this.refs["arscene"].getCameraOrientationAsync().then((orientation) => {
      this.refs["arscene"].performARHitTestWithRay(orientation.forward).then((results)=>{
        if (results.length > 0) {
          for (var i = 0; i < results.length; i++) {
            let result = results[i];
            if (result.type == "ExistingPlaneUsingExtent") {
              this.setState({
                boxLocation : result.transform.position
              });
              return;
            }
          }
        }
      })
    });
  },
  _onSurfaceClickUsingPosition(position) {
    this.refs["arscene"].performARHitTestWithPosition(position).then((results)=>{
      if (results.length > 0) {
        for (var i = 0; i < results.length; i++) {
          let result = results[i];
          if (result.type == "ExistingPlaneUsingExtent") {
            this.setState({
              boxLocation : result.transform.position
            });
            return;
          }
        }
      }
    })
  }
});

ViroMaterials.createMaterials({
  transparent: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#0000cc55"
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

module.exports = testARScene;
