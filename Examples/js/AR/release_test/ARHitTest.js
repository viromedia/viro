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
  ViroSurface
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';
let polarToCartesian = ViroUtils.polarToCartesian;


var testARScene = React.createClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      surfaceSize : [0,0,0],
      testWithRay : true,
    }
  },
  render: function() {
    return (
        <ViroARScene ref="arscene">
          <ViroText position={polarToCartesian([2, -10, 0])} text={"Mode: " + (this.state.testWithRay ? "testWithRay" : "testWithPosition")}
            style={styles.baseTextTwo} onClick={this._switchRayTestType} transformBehaviors={["billboard"]}/>
          <ViroARPlane onAnchorUpdated={this._onPlaneUpdate}>
            <ViroSurface materials={"transparent"} scale={this.state.surfaceSize}
             rotation={[-90, 0, 0]} onClick={this._onSurfaceClickUsingPosition}/>
          </ViroARPlane>
          {this._getBox()}

          {/* Release Menu */}
          <ViroText position={polarToCartesian([2, -30, 0])} text={"Next test"}
            style={styles.instructionText} onClick={this._goToNextTest} transformBehaviors={["billboard"]}/>
          <ViroText position={polarToCartesian([2, -30, -15])} text={"Release Menu"}
            style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
            transformBehaviors={["billboard"]}/>
        </ViroARScene>
    );
  },
  /*
          <ViroARPlane onAnchorUpdated={this._onPlaneUpdate}>
            <ViroSurface materials={"transparent"} scale={this.state.surfaceSize}
             rotation={[-90, 0, 0]} onClick={this._onSurfaceClickUsingPosition}/>
          </ViroARPlane>
   */
  _goToNextTest() {
    // do something!
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
      surfaceSize : [updateDict.width, updateDict.height, 1]
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
    diffuseColor: "#00000055"
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

module.exports = testARScene;
