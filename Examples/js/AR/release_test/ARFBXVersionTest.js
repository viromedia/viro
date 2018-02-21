
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
  View,
  Platform
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

import TimerMixin from 'react-timer-mixin';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var setLightValuesFast = true;

var ARFBXVersionTest = createReactClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      intensity : 1000,
      colorTemp : 6500
    };
  },
  componentDidMount: function() {
    this._intensity = 1000;
    this._colorTemp = 6500;
    if (!setLightValuesFast) {
      // Only set the intensity and temp twice a second!
      this.setInterval(
        ()=>{
          this.setState({
            intensity : this._intensity,
            colorTemp : this._colorTemp
          })
        },
        500
      )
    }
  },
  render: function() {
    return (
        <ViroARScene onAmbientLightUpdate={this._onAmbientLightUpdate}>
          <ViroAmbientLight color="#ffffff" intensity={this.state.intensity} temperature={Platform.OS == 'android' ? 6500 : this.state.colorTemp}/>
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
  _onAmbientLightUpdate(retDict) {
    if (!setLightValuesFast) {
      this._intensity = retDict.intensity;
      this._colorTemp = retDict.colorTemperature;
    } else {
      this.setState({
        intensity : retDict.intensity,
        colorTemp : retDict.colorTemperature
      })
    }
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
