
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
  ViroAmbientLight,
  ViroOmniLight,
  ViroText,
  ViroUtils,
  Viro3DObject,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var ARFBXVersionTest = createReactClass({
  getInitialState: function() {
    return {};
  },
  render: function() {
    return (
        <ViroARScene>
          <ViroAmbientLight color="#ffffff"/>
          <ViroNode position={[0,0,-2]} onDrag={()=>{}} dragType="FixedToWorld">
            <ViroText position={[1,.33,0]} text={"FBX 2018"}
            style={styles.instructionText} transformBehaviors={["billboard"]}/>
            <Viro3DObject position={[1,.0,-3]} scale={[.04,.04,.4]} type="VRX" source={require('./res/dragao_2018.vrx')} animation={{name:"01", delay:0, loop:true, run:true}} resources={[require('./res/dragao_map.jpg')]}  />

            <ViroText position={[-1,.33,0]} text={"FBX 2014"}
            style={styles.instructionText} transformBehaviors={["billboard"]}/>
            <Viro3DObject position={[-1,0,0]} scale={[.2,.2,.2]} type="VRX" source={require('./res/pug_01.vrx')} animation={{name:"01", delay:0, loop:true, run:true}} resources={[require('./res/PUG_bake_otimizado.jpg')]} />
          </ViroNode>



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

module.exports = ARFBXVersionTest;
