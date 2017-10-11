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
  ViroARPlaneSelector,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroImage,
  ViroVideo,
  ViroSkyBox,
  Viro360Video,
  ViroText,
  ViroSurface
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var testARScene = React.createClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      surfaceSize : [0,0,0],
      center : [0,0,0],
      rotation : [0,0,0],
      scale : [.15,.15,.15],
    }
  },
  render: function() {
    return (
        <ViroARScene ref="arscene" >
          <ViroARPlane onAnchorUpdated={this._onPlaneUpdate}>
            <ViroSurface materials={"transparent"} scale={this.state.surfaceSize}
             rotation={[-90, 0, 0]} onClick={this._onSurfaceClickUsingPosition}/>
          </ViroARPlane>
          {this._getBox()}
        </ViroARScene>
    );
  },
  /*
          <ViroARPlane onAnchorUpdated={this._onPlaneUpdate}>
            <ViroSurface materials={"transparent"} scale={this.state.surfaceSize}
             rotation={[-90, 0, 0]} onClick={this._onSurfaceClickUsingPosition}/>
          </ViroARPlane>
   */
  _getBox() {
    if (this.state.boxLocation != undefined) {
      return (
        <ViroNode position={this.state.boxLocation} >
          <ViroBox ref={(component)=>{this.boxRef = component}} position={[0,.075,0]} onDrag={()=>{}} dragType={"FixedToWorld"}
           scale={this.state.scale} rotation={this.state.rotation} onPinch={this._onPinch} onRotate={this._onRotate}/>
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
  },
  _onRotate(rotateState, rotationFactor, source) {
    if (rotateState == 3) {
      this.setState({
        rotation : [this.state.rotation[0], this.state.rotation[1] - rotationFactor, this.state.rotation[2]]
      });
      return;
    }

    this.boxRef.setNativeProps({rotation:[this.state.rotation[0], this.state.rotation[1] - rotationFactor, this.state.rotation[2]]});
  },

  /*
   Pinch scaling should be relative to its last value *not* the absolute value of the
   scale factor. So while the pinching is ongoing set scale through setNativeProps
   and multiply the state by that factor. At the end of a pinch event, set the state
   to the final value and store it in state.
   */
  _onPinch(pinchState, scaleFactor, source) {
    var newScale = this.state.scale.map((x)=>{return x * scaleFactor})

    if (pinchState == 3) {
      this.setState({
        scale : newScale
      });
      return;
    }

    this.boxRef.setNativeProps({scale:newScale});
  },
});

ViroMaterials.createMaterials({
  transparent: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#00000055"
  },
});

module.exports = testARScene;
