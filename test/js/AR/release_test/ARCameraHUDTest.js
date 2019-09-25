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
  View
} from 'react-native';

import {
  ViroSceneNavigator,
  ViroARScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroARCamera,
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
  ViroQuad,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;
var createReactClass = require('create-react-class');

var count = 0;

var ARCameraHUDTest = createReactClass({

  getInitialState() {
    return {
        activeCamera:1,
        fov: 60,
    };
  },

  render: function() {
    return (
     <ViroARScene>
       <ViroText position={polarToCartesian([6, -30, -15])} text={"Release Menu"}
         style={styles.centeredText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
         transformBehaviors={["billboard"]}/>

       <ViroAmbientLight color="#ffffff"/>
       <ViroOmniLight position={[0, 0, -5]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

        <ViroARCamera>
                <ViroBox position={[0, 0, -5]} materials={"blueColor"} />
                <Viro3DObject position={[0,.0,-3]} scale={[.2,.2,.2]} type="VRX"
                  source={require('./res/pug_01.vrx')} animation={{name:"01", delay:0, loop:true, run:true}}
                  resources={[require('./res/PUG_bake_otimizado.jpg')]} />
        </ViroARCamera>

        <ViroText style={styles.centeredText} position={[-2, -1, -4]} text={"This text is in AR"}
               width={2} height={2}  onClick={this._toggleCamera} />

     </ViroARScene>

    );
  },

  _onStart(){
    console.log("Animation Camera onStart();");
  },
  _onFinish(){
    console.log("Animation Camera onFinish();");
  },
});

var styles = StyleSheet.create({
  centeredText: {
      fontFamily: 'Arial',
      fontSize: 25,
      color: '#ffffff',
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});

ViroMaterials.createMaterials({
  redColor: {
      fresnelExponent: .5,
      shininess: 2.0,
      diffuseColor: "#ff0000"
  },
  blueColor: {
      fresnelExponent: .5,
      shininess: 2.0,
      diffuseColor: "#0000ff"
  },
  blue: {
      shininess: 2.0,
      lightingModel: "Lambert",
      diffuseColor: "#0000ff"
    },
});

ViroAnimations.registerAnimations({
    moveRight:{properties:{positionX:"+=2"}, duration: 2000, easing:"Linear"},
    moveLeft:{properties:{positionX:"-=2"}, duration: 2000, easing:"Linear"},
    testLoopMove:[
        ["moveRight", "moveLeft"]
    ],
});


module.exports = ARCameraHUDTest;
