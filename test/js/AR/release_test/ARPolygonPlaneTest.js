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
  ViroText,
  ViroUtils,
  ViroBox,
  ViroPolygon
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var testARScene = createReactClass({
  getInitialState: function() {
    return {
      polygonOpacity : 0,
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
          opacity={this.state.polygonOpacity}
          ref={component=>{this._polySurface = component}}
          rotation={[-90, 0, 0]}
          position={[0,0,0]}
          vertices={[[0,0], [1, 0], [0,1]]}
          materials={"blue_plane"}/>

        <ViroQuad
            ref={component=>{this._surface = component}}
            rotation={[-90, 0, 0]}
            position={[0,-0.0001,0]}
            scale={[1,1,1]}
            materials={"red_plane"}/>

      <ViroBox
      ref={component=>{this._box = component}}

      position={[0,-0.0001,0]}
      scale={[0.05,0.05,0.05]}
          />

      </ViroARPlane>

      <ViroText position={polarToCartesian([6, 50, 20])} text={"Detect Type: " + this._getDetectType()}
        style={styles.instructionText} transformBehaviors={["billboard"]}
        onClick={this._changeDetectType}/>

      {/* Release Menu */}
      <ViroText position={polarToCartesian([6, -30, 25])} text={"Release Menu"}
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

    if (!this.state.polygonOpacity){
      this.setState({
        polygonOpacity : 1,
      });
    }

    this.debugSurfacesAndPlanes(updateMap);

    var centeroffset = [updateMap.center[0], updateMap.center[1] - 0.001, updateMap.center[2]];
    this._polySurface.setNativeProps({"position" :centeroffset});
    this._polySurface.setNativeProps({"vertices" : polygonPoint});

    this._surface.setNativeProps({"position" : updateMap.center});
    this._surface.setNativeProps({"width" : updateMap.width});
    this._surface.setNativeProps({"height" : updateMap.height});
  },

  debugSurfacesAndPlanes(updateMap){

        var minX = 999999;
        var minY = 999999;
        var maxX = -999999;
        var maxY = -999999;
        for (i = 0; i < updateMap.vertices.length; i ++){
          var x = updateMap.vertices[i][0];
          var y = updateMap.vertices[i][2];

          if (x < minX){
            minX = x;
          }
          if (x > maxX){
            maxX = x;
          }

          if (y < minY){
            minY = y;
          }
          if (y > maxY){
            maxY = y;
          }
        }

        var mapMinX = updateMap.center[0] - (updateMap.width/2);
        var mapMinY = updateMap.center[2] - (updateMap.height/2);

        var mapMaxX = updateMap.center[0] +(updateMap.width/2);
        var mapMaxY = updateMap.center[2] + (updateMap.height/2);
        // console.log("Viro Surface width/height : " + mapMinX + " , " + mapMinY + " || " + mapMaxX + " , " + mapMaxY  );
        // console.log("Viro  Polygon width/height : " + (maxX - minX) + " , " + (maxY - minY) + "," + (mapMaxX - mapMinX) + " , " + (mapMaxY - mapMinY)  );

        console.log("Viro center is : " + updateMap.center[0]+ ", " + updateMap.center[1] +", " + updateMap.center[2]);
        console.log("Viro position is : " + updateMap.position[0] +", " + updateMap.position[1] +", " + updateMap.position[2]);
  },

  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARPlaneManualTest", {scene:require("./ARPlaneManualTest")})
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

ViroMaterials.createMaterials({
  blue_plane: {
    lightingModel: "Constant",
    diffuseColor: "#0000ff50",
    cullMode:'None'
  },
  red_plane: {
    lightingModel: "Constant",
    diffuseColor: "#ff000050"
  },
});

module.exports = testARScene;
