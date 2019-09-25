
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
  ViroAmbientLight,
  ViroARPlane,
  ViroMaterials,
  ViroImage,
  ViroARPlaneSelector,
  ViroQuad,
  ViroConstants,
  ViroNode,
  ViroBox,
  ViroOmniLight,
  ViroText,
  ViroUtils,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var ARDragTest = createReactClass({
  getInitialState: function() {
    return {};
  },
  render: function() {
    return (
        <ViroARScene onClick={()=>{this.refs["planeSelector"].reset()}} >
          <ViroAmbientLight color="#ffffff" />
          <ViroNode position={[0,0,-1]} onDrag={()=>{}} dragType="FixedToWorld">
            <ViroBox position={[0,.13,0]} scale={[.2,.2,.2]} materials="blueBox"
                onRotate={(state, factor)=>{console.log("[DragTest] rotation with factor: " + factor)}}/>
            <ViroImage rotation={[-90,0,0]} scale={[.3,.3,.3]} position={[0,.01,0]}
                source={require('../res/dark_circle_shadow.png')} materials="doesntWriteToDepthBuffer"/>
          </ViroNode>

          <ViroNode position={[0,0,-1]} onDrag={()=>{}}  dragType="FixedToPlane" dragPlane={{planePoint:[0,-.5,0], planeNormal:[0,1,0], maxDistance:3 }}>
            <ViroBox position={[0,.13,0]} scale={[.2,.2,.2]} materials="redBox" />
          </ViroNode>

          <ViroText position={polarToCartesian([1, 0, 10])} text={"Blue = FixedToWorld, Red = FixedToPlane"}
            style={styles.instructionText} transformBehaviors={["billboard"]}/>

          {/* Display the planes using this, tap on scene to bring them back if you
              accidentally tap on them.*/}
          <ViroARPlaneSelector ref={"planeSelector"}/>

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
    this.props.arSceneNavigator.replace("ARFBXVersionTest", {scene:require("./ARFBXVersionTest")})
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
  blueBox: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#0000cc"
  },
  redBox: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#cc0000"
  },
  doesntWriteToDepthBuffer: {
    writesToDepthBuffer: false,
  }
});

module.exports = ARDragTest;
