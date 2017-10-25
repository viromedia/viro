/**
 * Copyright (c) 2015-present, Viro, Inc.
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
  ViroSceneNavigator,
  ViroScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroImage,
  ViroVideo,
  ViroSkyBox,
  Viro360Video,
  ViroUtils,
  ViroText,

} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;

var createReactClass = require('create-react-class');

var visibleTest = createReactClass({
  getInitialState() {
    return {
      visible1 : true,
      visible2 : true,
      visible3 : true,
      visible4 : true,
      visible5 : true,
      visible6 : true,
      visible7 : true,
    }
  },
  render: function() {
    return (
      <ViroScene position={[0,0,0]} reticleEnabled={false}>
        <ViroSkyBox source={{nx:require("./res/sun_2302.jpg"),
                             px:require("./res/sun_2302.jpg"),
                             ny:require("./res/sun_2302.jpg"),
                             py:require("./res/sun_2302.jpg"),
                             nz:require("./res/sun_2302.jpg"),
                             pz:require("./res/sun_2302.jpg")}} format="RGBA8" />
        <ViroNode visible={this.state.visible1} >                   
          <ViroNode position={[2, -3, 0]} visible={this.state.visible2}>
            <ViroBox materials="wework_title" position={[0, 0, -5]} scale={[2, 4, 2]} transformBehaviors={["billboard"]} />
            <ViroBox materials="box_texture" position={[1, 5, -2]} scale={[3,3,3]} transformBehaviors={["billboard"]} />
          </ViroNode>
        </ViroNode>

        <ViroNode visible={this.state.visible3}>
          <ViroVideo height={4} width={4} position={[-1,0,-3]} visible={this.state.visible4}
            source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}} />
        </ViroNode>

        <ViroNode visible={this.state.visible5}>
          <ViroImage position={[0, 0, -2]} format="RGBA8" mipmap={true} visible={this.state.visible6}
            source={{uri: "https://upload.wikimedia.org/wikipedia/commons/7/74/Earth_poster_large.jpg"}} />
        </ViroNode>

        <ViroText position={polarToCartesian([2, -40, 20])} text={"visible1"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._changeVisiblity("visible1")}/>
        <ViroText position={polarToCartesian([2, -40, 10])} text={"visible2"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._changeVisiblity("visible2")}/>
        <ViroText position={polarToCartesian([2, -40, 0])} text={"visible3"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._changeVisiblity("visible3")}/>
        <ViroText position={polarToCartesian([2, -40, -10])} text={"visible4"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._changeVisiblity("visible4")}/>
        <ViroText position={polarToCartesian([2, -40, -20])} text={"visible5"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._changeVisiblity("visible5")}/>
        <ViroText position={polarToCartesian([2, -40, -30])} text={"visible6"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._changeVisiblity("visible6")}/>

      </ViroScene>
    );
  },
  _changeVisiblity(key) {
    return ()=>{
      let newState = {}
      newState[key] = !this.state[key];
      this.setState(newState);
    }
  },
});

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#FFFFFF',
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    color: '#333333',
    margin: 10,
  },
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
  blue: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#0000ff"
  },
  black: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#000000"
  },
  red: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#ff0000"
  },
  wework_title: {
    shininess: 1.0,
    lightingModel: "Constant",
    diffuseTexture: {"uri": "https://s3-us-west-2.amazonaws.com/viro/guadalupe_360.jpg"},
    diffuseTexture: require("./res/new_menu_screen.jpg"),
    fresnelExponent: .5,
  },
  box_texture: {
    diffuseTexture: require("./res/sun_2302.jpg"),
  }
});


module.exports = visibleTest;
