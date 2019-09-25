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
  ViroAnimatedComponent,
  ViroScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroImage,
  ViroVideo,
  ViroFlexView,
  ViroUtils,
  ViroText,
  ViroQuad,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
  ViroButton,
  ViroSpinner,
  ViroOmniLight,
  ViroAnimations,
  ViroDirectionalLight,
  ViroAmbientLight,
  ViroSceneNavigator
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;
var LocalButtonImage = require("./res/icon_live.jpg");

var LeakTest1 = createReactClass({
  getInitialState() {
    return {

    };
  },

  render: function() {
    var newMaterial = "redColor";
    var transformText = "redColor";
    var heartMat = "heartRed";

    return (
      <ViroScene>
          <ViroAmbientLight color={"#ffffff"} />
          <ViroNode position={[0.8, 0, -3.5]}>
              <Viro3DObject
                source={require('../res/heart.obj')}
                scale={[1.8, 1.8, 1.8]}
                position={[-3.2, 2.5, -4.5]}
                type="OBJ"
                materials={[heartMat]}
              />

              <ViroBox
                position={[-1 , 1, 0]}
                scale={[0.4 , 0.4 , 0.4]}
                rotation={[0,45,0]}
                materials={[newMaterial,newMaterial,newMaterial,newMaterial,newMaterial,newMaterial]}
                height={1}
                width={1}
                length={1}
              />

              <ViroButton
                position={[0, 1, 0]}
                materials={[newMaterial]}
                source={LocalButtonImage}
                hoverSource={LocalButtonImage}
                clickSource={LocalButtonImage}
              />

              <ViroFlexView
                position={[1, 1, 0]}
                scale={[0.2, 0.2, 0.1]}
                materials={[newMaterial]}
                width={3}
                height={2}
              />

              <ViroImage
                width={1} height={1}
                format="RGBA8" mipmap={true}
                position={[-2, 0, 0]}
                materials={[newMaterial]}
                scale={[0.5, 0.5, 0.1]}
                source={require('./res/card_main.png')}
              />

              <ViroNode
                position={[-1, 0, 0]}
                scale={[0.5, 0.5, 0.1]}
                rotation={[0,0,0]}>
                    <ViroText
                      materials={[newMaterial]}
                      style={styles.baseTextTwo}
                      text="This is a text in a ViroNode"
                     />
              </ViroNode>

              <ViroSphere
                position={[0, 0, 0]}
                scale={[0.3, 0.3, 0.3]}
                widthSegmentCount={5}
                heightSegmentCount={5}
                radius={1}
                materials={[newMaterial]}
              />

              <ViroSpinner
                materials={[newMaterial]}
                position={[1, 0, 0]}
                scale={[0.3, 0.3, 0.1]}
              />

              <ViroQuad
                position={[-2, -1, 0]}
                scale={[0.5, 0.5, 0.1]}
                materials={[newMaterial]}
                width={1}
                height={1}
              />

              <ViroText
                materials={[newMaterial]}
                position={[-1, -1, 0]}
                scale={[0.5 , 0.5, 0.1]}
                style={styles.baseTextTwo}
                text="This is a Viro Text"
              />

              <ViroVideo
                position={[2, -1, 0]}
                rotation={[0, 0, 0]}
                paused={false}
                loop={true}
                height={1}
                width={1}
                source={require('../A9Demo/res/a9_3d_assets/amazon_vid.mp4')}
               />
          </ViroNode>

          <ViroText
            style={styles.baseTextTwo}
            position={[0.8 , -2, -3.5]}
            width={2} height={2}
            fontFamily={'Arial'}
            text={"Push Leak Scene 2"}
            textLineBreakMode='Justify'
            onClick={this._pushNextScene}
          />
      </ViroScene>
    );
  },

  _pushNextScene() {
    var scene = require("./LeakTest2");
    this.props.sceneNavigator.push("Leak 2", {scene:scene});
  },

});

var styles = StyleSheet.create({
  baseTextTwo: {
      fontFamily: 'Arial',
      fontSize: 20,
      color: '#ffffff',
      flex: 1,
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
  heartRed: {
    fresnelExponent: .5,
    shininess: 2.0,
    diffuseColor: "#ff0000",
    diffuseTexture: require('../res/heart_d.jpg'),
  },
 });

module.exports = LeakTest1;
