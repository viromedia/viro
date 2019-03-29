/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 */
'use strict';

import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View,
} from 'react-native';


import {
  ViroSceneNavigator,
  ViroScene,
  ViroARScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroFlexView,
  ViroUtils,
  ViroText,
} from 'react-viro';

var createReactClass = require('create-react-class');

var postEffectTests = [
  ["GrayScale"],
  ["Sepia"],
  ["SinCity"],
  ["BarallelDistortion"],
  ["PincushionDistortion"],
  ["Toonify"],
  ["Inverted"],
  ["ThermalVision"],
  ["Pixelated"],
  ["ThermalVision", "BarallelDistortion"],
  ["PincushionDistortion", "GrayScale"],
  ["None"], // this should be the last one!
];

var postEffectTestsTag = [
  ["GrayScale"],
  ["Sepia"],
  ["SinCity"],
  ["BarallelDistortion"],
  ["PincushionDistortion"],
  ["Toonify"],
  ["Inverted"],
  ["ThermalVision"],
  ["Pixelated"],
  ["ThermalVision + BarallelDistortion"],
  ["PincushionDistortion + GrayScale"],
  ["None"], // this should be the last one!
];
var ViroPostProcesTest = createReactClass({
  getInitialState() {
    return {
      postProcessEffect:[],
      number:0
    };
  },

  toggleProperty(num){
      return () => {
          let num = this.state.number + 1;
          if (num >= postEffectTests.length){
            num = 0;
          }
          this.setState({
           number:num
          });
      }
  },

  render: function() {
    return (
      <ViroARScene ref="scene1" postProcessEffects={postEffectTests[this.state.number]}>
        <ViroNode position={[0 , 0, -5]} transformBehaviors={["billboard"]}>

          <ViroText fontSize={35}  style={styles.centeredText}
            position={[0,0, 0]} width={7} height ={3} maxLines={3}
            text={"Toggle property: " + postEffectTestsTag[this.state.number] }
            onClick={this.toggleProperty(1)} />

          <ViroText position={[-2 , 0, -.5]} text={"Release Menu"}
            style={styles.instructionText} onClick={this._goBackToReleaseMenu}
            transformBehaviors={["billboard"]}/>
        </ViroNode>

      </ViroARScene>

    );
  },
  _goBackToReleaseMenu() {
    this.props.arSceneNavigator.replace("ARReleaseMenu", {
      scene: require("./ARReleaseMenu")
    });
  },
});

var styles = StyleSheet.create({

  centeredText: {
    fontFamily: 'Arial',
    color: '#ffffff',
    flex: 1,
  },
  instructionText: {
    fontFamily: 'Arial',
    fontSize: 30,
    color: '#cccccc',
    flex: 1,
    textAlignVertical: 'center',
    textAlign: 'center',
  },

});

module.exports = ViroPostProcesTest;
