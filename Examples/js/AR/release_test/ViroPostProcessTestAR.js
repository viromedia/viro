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
   ViroSurface,
   ViroSkyBox,
   ViroPortal,
   ViroPortalFrame,
   ViroSphere,
   Viro3DObject,
 } from 'react-viro';

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
];
var ViroPostProcesTest = React.createClass({
  getInitialState() {
    return {
      postProcessEffect:[],
      number:0
    };
  },

  toggleProperty(num){
      return () => {

          let that = this;
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
                   onClick={this.toggleProperty(1)}
                   />
                   <ViroText position={[0 , 0, -6]} text={"Release Menu"}
                     style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
                     transformBehaviors={["billboard"]}/>
                </ViroNode>
            </ViroARScene>

    );
  },
});

var styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  elementText: {
    fontFamily: 'HelveticaNeue-Medium',
    fontSize: 30,
    color: '#ffffff',
    textAlign: 'center',
  },
  baseTextTwo: {
    fontFamily: 'Arial',
    color: '#ffffff',
    flex: 1,
  },
  centeredText: {
       fontFamily: 'Arial',
       color: '#ffffff',
       flex: 1,
  },instructionText: {
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
    lightingModel: "Blinn",

      cullMode: "None",
      shininess: 2.0,
      diffuseColor: "#3399ff99"
    },
    ground: {
      lightingModel: "Blinn",

        cullMode: "None",
        shininess: 2.0,
        diffuseColor: "#ffffff"
      },
    shadowCatcher: {
      writesToDepthBuffer: false,
    },
  green: {
        cullMode: "None",
        shininess: 2.0,
        diffuseColor: "#33cc3399"

      },

  ground:{
    cullMode: "None",
    shininess: 2.0,
    diffuseColor: "#ff9999"
  }
 });

module.exports = ViroPostProcesTest;
