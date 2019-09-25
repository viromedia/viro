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
var FlexViewTest = createReactClass({
  getInitialState: function() {
    return {
      ...stateOne,
      state: 1,
      soundSource: 1,
      position: [5,0,0]
    }
  },
  render: function() {
    return (
      <ViroScene reticleEnabled={true} onTap={this._onTap} soundRoom={this.state.state == 1 ? soundRoom : soundRoom1} >

        <ViroSkyBox color="#ff69b4" />

        <ViroAnimatedComponent animation="testLoopRotate" run={true} loop={true} >
          <ViroNode>
            <ViroSpatialSound source={{uri : "http://www.kozco.com/tech/32.mp3"}} loop={true} position={this.state.position} />
          </ViroNode>
        </ViroAnimatedComponent>

        {this._getAdditionalSounds()}

        <ViroFlexView style={styles.containerVertical} position={polarToCartesian([2, 0, 0])} width={3} height={2} opacity={1}>
          <ViroFlexView style={styles.containerInner} >
            {this._getSurface()}
            <ViroImage style={{flex:1}} source={{uri: "http://weknowyourdreams.com/images/sun/sun-02.jpg"}} />
          </ViroFlexView>
        </ViroFlexView>

      </ViroScene>
    );
  },
  _onTap(component) {
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
  _getSoundSource(component) {
    if (this.state.soundSource == 1) {
      return require("./res/btn_tap.mp3")
    } else {
      return {uri: 'http://www.bensound.com/royalty-free-music?download=dubstep'};
    }
  },
  _getAdditionalSounds(component) {
    if (this.state.state == 1) {
      return (<ViroSpatialSound source={{uri : "http://www.kozco.com/tech/32.mp3"}} loop={true} position={this.state.position} />);
    } else {
      return;
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
  testLoopRotate:{properties:{rotateZ:"+=45"}, duration:500},
});

module.exports = FlexViewTest;
