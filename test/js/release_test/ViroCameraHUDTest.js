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
  ViroSphere,
  Viro3DObject,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;
var createReactClass = require('create-react-class');

var count = 0;

// **This test has not been done.  This is placeholder for scene and navigation arrows**

var ReleaseMenu = require("./ReleaseMenu.js");

var ViroCameraHUDTest = createReactClass({

  getInitialState() {
    return {
        activeCamera:0,
        fov: 60,
    };
  },

  render: function() {
    return (
     <ViroScene onClick={this._toggleCamera} ref="cameraScene">
     <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
     <ViroAmbientLight color="#ffffff"/>
     <ViroOmniLight position={[0, 0, -5]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

     <ViroCamera
         position={[0, 0, 1]}
         rotation={[0, 0, 0]}
         active={this.state.activeCamera == 0} />

     <ViroCamera
         fieldOfView={this.state.fov}
         position={[0,0,0]}
         active={this.state.activeCamera == 1}
         animation={{name:"testLoopMove",
                     loop:true,
                     run:true,
                     onStart:this._onStart,
                     onFinish:this._onFinish}}>
              <ViroBox position={[0, 0, -5]} materials={"blueColor"} />
              <Viro3DObject source={require('./res/gltf/DuckGlb.glb')}
                            position={[0, .5, -3]}
                            scale={[0.5,0.5,0.5]}
                            type="GLB"
                            />
      </ViroCamera>


      <ViroCamera
          position={[0, 0, 1]}
          rotation={[0, 0, 0]}
          fieldOfView={this.state.fov}
          active={this.state.activeCamera == 2} >
          <ViroBox
             position={[0, 0, -4]}
             scale={[0.8 , 0.8 , 0.8]}
             materials={["redColor"]}
             height={1}
             width={1}
             length={1} />

            <ViroText style={styles.centeredText} position={[-0.5, -1, -6]} text={"HUD Text"}
                    width={2} height={2} />
      </ViroCamera>

      <ViroText style={styles.centeredText} position={[-2, -1, -4]} text={"Active Camera:" + this.state.activeCamera}
             width={2} height={2}  onClick={this._toggleCamera} />

     </ViroScene>

    );
  },

  _onStart(){
    console.log("Animation Camera onStart();");
  },
  _onFinish(){
    console.log("Animation Camera onFinish();");
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./SceneTest1')});
  },

  _toggleCamera(){
      var newCamera = this.state.activeCamera + 1;
      if (newCamera > 2) {
          newCamera = 0;
      }
      this.setState({
         activeCamera:newCamera
      });
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
  heart: {
      lightingModel: "Phong",
      diffuseTexture: require('../res/heart_d.jpg'),
    },
});

ViroAnimations.registerAnimations({
    moveRight:{properties:{positionX:"+=2"}, duration: 2000, easing:"Linear"},
    moveLeft:{properties:{positionX:"-=2"}, duration: 2000, easing:"Linear"},
    testLoopMove:[
        ["moveRight", "moveLeft"]
    ],
});


module.exports = ViroCameraHUDTest;
