
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
  ViroQuad,
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

var setLightValuesFast = true;

var ARFBXVersionTest = createReactClass({
  getInitialState: function() {
    return {
      ambientIntensity : 1000,
      ambientColor : 6500,
      animState: true,
    };
  },
  componentDidMount: function() {
    this._ambientIntensity = 1000;
    this._ambientColor = 6500;
    if (!setLightValuesFast) {
      // Only set the intensity and temp twice a second!
      setInterval(
        ()=>{
          this.setState({
            ambientIntensity : this._ambientIntensity,
            ambientColor : this._ambientColor
          })
        },
        500
      )
    }
  },
  render: function() {
    return (
        <ViroARScene onAmbientLightUpdate={this._onAmbientLightUpdate}>
          <ViroAmbientLight color="#ffffff" intensity={this.state.ambientIntensity} color={this.state.ambientColor} />
          <ViroNode position={[0,0,-2]} onDrag={()=>{}} dragType="FixedToWorld">
            <ViroText position={[1,.33,0]} text={"FBX 2018"}
            style={styles.instructionText} transformBehaviors={["billboard"]}/>
            <Viro3DObject position={[1,.0,-3]} scale={[.04,.04,.4]} type="VRX" source={require('./res/dragao_2018.vrx')} animation={{name:"01", delay:0, loop:true, run:this.state.animState}} resources={[require('./res/dragao_map.jpg')]}  />

            <ViroText position={[-1,.33,0]} text={"FBX 2014"}
            style={styles.instructionText} transformBehaviors={["billboard"]}/>
            <Viro3DObject position={[-1,0,0]} scale={[.2,.2,.2]} onClick={this._changeAnimState} type="VRX" source={require('./res/emoji_kiss_anim/emoji_kiss_anim.vrx')}
              animation={{name:"02", delay:0, loop:true, run:this.state.animState}}
              resources={[require('./res/emoji_kiss_anim/emoji_kiss_diffuse.png'),
                          require('./res/emoji_kiss_anim/emoji_kiss_normal.png'),
                          require('./res/emoji_kiss_anim/emoji_kiss_specular.png')]} />
          </ViroNode>


          <ViroText position={[0, -2, -4]} text={"Start/Stop Anim."}
            style={styles.instructionText} onClick={this._changeAnimState} transformBehaviors={["billboard"]}/>

          {/* Release Menu */}
          <ViroText position={polarToCartesian([6, -30, 0])} text={"Next test"}
            style={styles.instructionText} onClick={this._goToNextTest} transformBehaviors={["billboard"]}/>
          <ViroText position={polarToCartesian([6, -30, -15])} text={"Release Menu"}
            style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
            transformBehaviors={["billboard"]}/>
        </ViroARScene>
    );
  },
  _onAmbientLightUpdate(retDict) {
    if (!setLightValuesFast) {
      this._ambientIntensity = retDict.intensity;
      this._ambientColor = retDict.color;
    } else {
      this.setState({
        ambientIntensity : retDict.intensity,
        ambientColor : retDict.color
      })
    }
  },
  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARPlaneSelectorTest", {scene:require("./ARPlaneSelectorTest")})
  },

  _changeAnimState() {
    this.setState({
      animState: !this.state.animState,
    });
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
  doesntWriteToDepthBuffer: {
    writesToDepthBuffer: false,
  }
});

module.exports = ARFBXVersionTest;
