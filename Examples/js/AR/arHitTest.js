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
  ViroSkyBox,
  Viro360Video,
  ViroText,
  ViroQuad
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var createReactClass = require('create-react-class');

var testARScene = createReactClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      surfaceSize : [0,0,0],
      center : [0,0,0],
    }
  },
  render: function() {
    return (
        <ViroARScene ref="arscene">
          <ViroARPlane onAnchorUpdated={this._onPlaneUpdate}>
            <ViroQuad materials={"transparent"} position={this.state.center}
             scale={this.state.surfaceSize} rotation={[-90, 0, 0]}
             onClick={this._onSurfaceClickUsingPosition}/>
          </ViroARPlane>
          {this._getBox()}
        </ViroARScene>
    );
  },
  /*
          <ViroARPlane onAnchorUpdated={this._onPlaneUpdate}>
            <ViroQuad materials={"transparent"} scale={this.state.surfaceSize}
             rotation={[-90, 0, 0]} onClick={this._onSurfaceClickUsingPosition}/>
          </ViroARPlane>
   */
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
    console.log(position);
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

module.exports = testARScene;
