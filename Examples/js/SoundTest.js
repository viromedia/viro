/**
 * Copyright (c) 2017-present, Viro, Inc.
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
  ViroCamera,
  ViroImage,
  ViroVideo,
  Viro360Image,
  Viro360Video,
  ViroFlexView,
  ViroUtils,
  ViroAnimations,
  ViroAnimatedComponent,
  ViroQuad,
  ViroSkyBox,
  ViroSpatialSound,
  ViroSound,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var stateOne = {
  flex: {flex: 1},
  material: "redColor",
  width: 3,
  showSurface: true,
};

var stateTwo = {
  flex: {flex: 2},
  material: "greenColor",
  width: 3,
  showSurface: false,
}

var soundRoom = {
  size: [15, 2, 3],
  wallMaterial: "CURTAIN_HEAVY",
  floorMaterial: "CURTAIN_HEAVY",
  ceilingMaterial: "CURTAIN_HEAVY"
}

var soundRoom1 = {
  size: [15, 2, 3],
  wallMaterial: "transparent",
  floorMaterial: "transparent",
  ceilingMaterial: "transparent"
}

/*
 This file tests a simple flex view and updating properties within it.
 */
var SoundTest = createReactClass({
  getInitialState: function() {
    return {
      ...stateOne,
      state: 1,
      soundSource: 1,
      position: [15,0,0]
    }
  },
  render: function() {
    return (
      <ViroScene reticleEnabled={true} onClick={this._onClick} soundRoom={this.state.state == 1 ? soundRoom : soundRoom1} >

        <ViroSkyBox color="#ff69b4" />

        <ViroAnimatedComponent animation="testLoopRotate" run={true} loop={true} >
          <ViroNode>
            <ViroSpatialSound source={this._getSource()} loop={true} position={this.state.position} />
          </ViroNode>
        </ViroAnimatedComponent>

        {this._getAdditionalSounds()}

      </ViroScene>
    );
  },
  _onFinishSound() {
    console.log("SoundTest onFinishSound!");
  },
  _onClick(component) {
    if (this.state.state == 1) {
      this.setState({
        ...stateTwo,
        state: 2,
        soundSource: 2,
        position: [5,0,0],
      })
    } else {
      this.setState({
        ...stateOne,
        state: 1,
        soundSource: 1,
        position: [5,0,0],
      })
    }
  },
  _getSurface(component) {
    if (this.state.showSurface) {
      return (<ViroQuad style={{flex:1}} materials={"redColor"} />)
    }
  },
  _getAdditionalSounds(component) {
    return (<ViroSound source={{uri : "http://www.freesound.org/data/previews/146/146887_197046-lq.mp3"}} loop={false} position={this.state.position} onFinish={this._onFinishSound}/>);
  },
  _getSource(component) {
    if (this.state.state == 1) {
      return require("./res/metronome.mp3");
    } else {
      return "cube_sound";
    }
  },
  /*
  <Viro360Image source={{uri: "http://cdn3-www.dogtime.com/assets/uploads/gallery/pembroke-welsh-corgi-dog-breed-pictures/prance-8.jpg"}} rotation={[-30,90,0]} />
  */
});

const styles = StyleSheet.create({
  containerVertical: {
    flexDirection: 'column',
  },
  containerInner: {
    flexDirection: 'row',
    flex: 1,
    padding: .1,
    backgroundColor: "#0000ff",
  },
  containerInnerTwo: {
    flexDirection: 'row',
    flex: 2,
    padding: .1,
    backgroundColor: "#0000ff",
  },
  containerHorizontal: {
    flexDirection: 'row',
    padding: .1,
  },
});

stateOne.innerStyle = styles.containerInner;
stateTwo.innerStyle = styles.containerInnerTwo;

ViroMaterials.createMaterials({
  redColor: {
    diffuseColor: "#ff0000"
  },
  greenColor: {
    diffuseColor: "#00ff00"
  },
});

ViroAnimations.registerAnimations({
  testLoopRotate:{properties:{rotateY:"+=45"}, duration:500},
});

ViroSound.preloadSounds({
  "metronome" : "http://www.kozco.com/tech/32.mp3",
  "cube_sound" : "https://s3-us-west-2.amazonaws.com/viro/cube_sound.wav",
  "g_note" : "http://www.jetcityorange.com/musical-notes/G4-392.0.mp3",
});

module.exports = SoundTest;
