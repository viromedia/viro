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
  ViroUtils,
  ViroPolygon
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var testARScene = createReactClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      useFirstPlane : true,
      updateMap : undefined,
      minValue : 0,
      detectType : 0,
    }
  },
  render: function() {
    return (
    <ViroARScene position={[0,0,0]} reticleEnabled={false} anchorDetectionTypes={this._getDetectType()} displayPointCloud={true}>
      <ViroARPlane
          minHeight={0.3}
          minWidth={0.3}
          alignment={this.state.detectType == 1 ? "Vertical" : "Horizontal"}
          key={"firstPlane"}
          onAnchorFound={this._onAnchorFound}
          onAnchorUpdated={this._onAnchorUpdated}
          onAnchorRemoved={this._onAnchorRemoved} >

        <ViroPolygon
          ref={component=>{this._polySurface = component}}
          rotation={[-90, 0, 0]}
          position={[0,0,0]}
          vertices={[[0,0], [0,0], [0,0]]}
          materials={"blue_plane"}/>

        <ViroSurface
            ref={component=>{this._surface = component}}
            rotation={[-90, 0, 0]}
            position={[0,-0.01,0]}
            scale={[1,1,1]}
            materials={"red_plane"}/>


      </ViroARPlane>

      <ViroText position={polarToCartesian([2, 50, 20])} text={"Detect Type: " + this._getDetectType()}
        style={styles.instructionText} transformBehaviors={["billboard"]}
        onClick={this._changeDetectType}/>

      {/* Release Menu */}
      <ViroText position={polarToCartesian([2, -30, 25])} text={"Release Menu"}
        style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
        transformBehaviors={["billboard"]}/>
    </ViroARScene>);
  },
  _changeDetectType() {
    this.setState({
      detectType : (this.state.detectType == 2) ? 0 : this.state.detectType + 1
    })
  },
  _getDetectType() {
    if (this.state.detectType == 0) {
      return "PlanesHorizontal"
    } else if (this.state.detectType == 1) {
      return "PlanesVertical"
    } else {
      return "None"
    }
  },
  _onAnchorUpdated(updateMap) {
    var polygonPoint = [];
    for (i = 0; i < updateMap.vertices.length; i ++){
      var point = [];
      point.push(updateMap.vertices[i][0]);
      point.push(updateMap.vertices[i][2]);
      polygonPoint.push(point);
    }

//this.setState({polygonPoint:polygonPoint})
    this._polySurface.setNativeProps({"vertices" : polygonPoint});
    this._surface.setNativeProps({"scale" : [updateMap.width, updateMap.height, 1]});

  },
  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARPlaneManualTest", {scene:require("./ARPlaneManualTest")})
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
  blue_plane: {
    lightingModel: "Constant",
    diffuseColor: "#0000ff50"
  },
  red_plane: {
    lightingModel: "Constant",
    diffuseColor: "#ff000050"
  },
});

module.exports = testARScene;
