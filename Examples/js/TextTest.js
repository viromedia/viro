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
  ViroCamera,
  ViroAmbientLight,
  ViroOmniLight,
  ViroSpotLight,
  ViroDirectionalLight,
  ViroImage,
  ViroVideo,
  Viro360Image,
  Viro360Video,
  ViroFlexView,
  ViroUtils,
  ViroText,
  ViroAnimations,
  ViroAnimatedComponent,
  ViroQuad,
  ViroSkyBox,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var TextTest = createReactClass({

  _platformUpdate: function(updates) {
    console.log("Platform updated! platform: " + updates.platform + ", headset: " + updates.headset + ", controller: " + updates.controller);
  },
  render: function() {
    return (
      <ViroScene reticleEnabled={true} onPlatformUpdate={this._platformUpdate} >
        <ViroSkyBox color="#ff69b4" />

        <ViroFlexView style={styles.containerVertical} position={polarToCartesian([2, 0, 0])} width={3} height={2}>
          <ViroFlexView style={styles.containerInner} >
            <ViroText style={styles.baseTextTwo} text="I'm testing this to see if it works" />
            <ViroImage style={{flex:1}} source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}} />
          </ViroFlexView>
          <ViroFlexView style={styles.containerInner} >
            <ViroImage style={{flex:1}} source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}} />
          </ViroFlexView>
        </ViroFlexView>
        <ViroFlexView style={styles.containerVertical} width={2} height={2} materials="cardpetite" position={[-2, 0, -1]}>
          <ViroText style={styles.baseTextTwo}  text="Centered Text!" />
          <ViroText style={styles.baseTextTwo}  text="Centered Text Two!" />
        </ViroFlexView>
        <ViroFlexView style={styles.containerVerticalTwo} position={[2.3, 0, -2.5]} width={3} height={3} >
          <ViroText style={styles.baseTextThree} height={.5} text="Base text three!" />
          <ViroText style={styles.baseTextThree} height={.5} text="Base text three again!" />
        </ViroFlexView>

        <ViroText style={styles.baseTextTwo} position={[0,-1, -1]} width={2} height={4} text="I'm testing this to see if it works, this is the best most tremendous text engine. You won't be disappointed, believe me!" />
      </ViroScene>
    );
  },
});

const styles = StyleSheet.create({
  containerVertical: {
    flexDirection: 'column',
  },
  containerVertical: {
    flexDirection: 'column',
  },
  containerVerticalTwo: {
    flexDirection: 'column',
    justifyContent: 'flex-start'
  },
  containerInner: {
    flexDirection: 'row',
    flex: 1,
    padding: .1,
    backgroundColor: "#0000ff",
  },
  containerHorizontal: {
    flexDirection: 'row',
    padding: .1,
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    color: '#333333',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
  baseTextTwo: {
      fontFamily: 'Arial',
      fontSize: 20,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
  baseTextThree: {
     fontFamily: 'Courier',
     fontSize: 20,
     color: '#ffffff',
     flex: 1,
     textAlign: 'center',
 },
});

ViroMaterials.createMaterials({
  sunTexture: {
    diffuseTexture: require("./res/sun_2302.jpg"),
  },
  redColor: {
    diffuseColor: "#ff0000"
  },
  cardpetite: {
    shininess : 1.0,
    lightingModel: "Constant",
    diffuseTexture: require('./res/card_petite_ansu.png'),
  },
  box_material: {
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseColor: "#ffffff"
  }
});

ViroAnimations.registerAnimations({
    moveRight:{properties:{positionX:"+=3"}, duration: 3000},
    moveLeft:{properties:{positionX:"-=3"}, duration: 3000},
    rotate:{properties:{rotateZ:"+=90"}, duration:3000},
    rotateY:{properties:{rotateY:"+=180"}, duration:1500},
    rotateAndMovePicture:[
        ["moveLeft", "moveRight"],
        ["moveRight", "moveLeft"],
        ["rotateY", "rotateY"],
        ["rotate"],

    ],
});

module.exports = TextTest;
