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
  ViroMaterials,
  Viro360Video,
  ViroNode,
  ViroImage,
  ViroVideo,
  ViroFlexView,
  ViroUtils,
  ViroText,
  ViroSound,
  ViroSpatialSound,
  ViroSurface,
  ViroSkyBox,
  Viro3DObject,
  ViroButton,
  ViroSpinner,
  ViroOmniLight,
  ViroAnimations,
  Viro360Image,
  ViroDirectionalLight,
  ViroAmbientLight,
  ViroSceneNavigator
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;
var LocalButtonImage = require("./res/icon_live.jpg");

var LeakTest1 = React.createClass({
  getInitialState() {
    return {
      isErrorState: true,
      videoURL: {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf-ERROR.mp4"},
      imageURL: {uri:"https://s3-us-west-2.amazo-ERROR-naws.com/viro/Explorer/360_horseshoe-ERROR.jpg"},
      objURL:   {uri:"https://s3-us-west-2.amazonaws.com/viro/heart-ERROR.obj"},
    };
  },

  render: function() {
    var newMaterial = "redColor";
    var transformText = "redColor";
    var heartMat = "heartRed";

    return (
      <ViroScene>
          <ViroAmbientLight color={"#ffffff"} />

           <Viro360Image
                source={this.state.imageURL}
                onError={this._onErrorImage360}
            />

          <ViroNode position={[0.8 , 0, -3.5]}>
              <Viro3DObject
                source={this.state.objURL}
                scale={[1.8 , 1.8  , 1.8]}
                position={[-2.5 , -4.3 , -1.15]}
                materials={[heartMat]}
                onError={this._onErrorOBJ}
              />

              <ViroImage
                width={1} height={1}
                format="RGBA8" mipmap={true}
                position={[-2, 0, 0]}
                materials={[newMaterial]}
                scale={[0.5, 0.5, 0.1]}
                source={this.state.imageURL}
                onError={this._onErrorImage}
              />

              <ViroVideo
                position={[2, -1, 0]}
                rotation={[0, 0, 0]}
                paused={false}
                loop={false}
                height={1}
                width={1}
                onError={this._onErrorVideo}
                onFinish={this._onFinishVideo}
                source={this.state.videoURL}
               />
          </ViroNode>

          <ViroText
            style={styles.baseTextTwo}
            position={[0.8 , -2, -3.5]}
            width={2} height={2}
            fontFamily={'Arial'}
            text={"Toggle Error State"}
            textLineBreakMode='justify'
            onClick={this._toggleErrorState}
          />

      </ViroScene>
    );
  },

  _toggleErrorState(event) {
    if (this.state.isErrorState) {
      this.setState({
        isErrorState: false,
        videoURL: {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf.mp4"},
        imageURL: {uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/360_horseshoe.jpg"},
        objURL:   {uri:"https://s3-us-west-2.amazonaws.com/viro/heart.obj"},
      });
    }
    else {
      this.setState({
        isErrorState: true,
        videoURL: {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf-ERROR.mp4"},
        imageURL: {uri:"https://s3-us-west-2.amazo-ERROR-naws.com/viro/Explorer/360_horseshoe-ERROR.jpg"},
        objURL:   {uri:"https://s3-us-west-2.amazonaws.com/viro/heart-ERROR.obj"},
      });
    }
    console.log("started loading image");
  },

  _onErrorImage(event) {
    console.log("Image error: " + event.nativeEvent.error);
  },

  _onErrorImage360(event) {
    console.log("Image 360 error: " + event.nativeEvent.error);
  },

  _onErrorVideo(event) {
    console.log("Video error: " + event.nativeEvent.error);
  },

  _onErrorVideo360(event) {
    console.log("Video 360 error: " + event.nativeEvent.error);
  },

  _onErrorOBJ(event) {
    console.log("OBJ error: " + event.nativeEvent.error);
  },

  _onFinishVideo(event) {
    console.log("Video finished");
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
