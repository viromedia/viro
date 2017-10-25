
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
          <ViroNode position={[0,0,-1]} onDrag={()=>{}} dragType="FixedToWorld">
            <ViroBox position={[0,.13,0]} scale={[.2,.2,.2]} materials="blueBox"/>
            <ViroImage rotation={[-90,0,0]} scale={[.3,.3,.3]} position={[0,.01,0]}
                source={require('../res/dark_circle_shadow.png')} materials="doesntWriteToDepthBuffer"/>
          </ViroNode>

          {/* Display the planes using this, tap on scene to bring them back if you
              accidentally tap on them.*/}
          <ViroARPlaneSelector ref={"planeSelector"}/>

          {/* Release Menu */}
          <ViroText position={polarToCartesian([2, -30, 0])} text={"Next test"}
            style={styles.instructionText} onClick={this._goToNextTest} transformBehaviors={["billboard"]}/>
          <ViroText position={polarToCartesian([2, -30, -15])} text={"Release Menu"}
            style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
            transformBehaviors={["billboard"]}/>
        </ViroARScene>
    );
  },
  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARPlaneSelectorTest", {scene:require("./ARPlaneSelectorTest")})    
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
  blueBox: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#0000cc"
  },
  doesntWriteToDepthBuffer: {
    writesToDepthBuffer: false,
  }
});

module.exports = ARDragTest;
