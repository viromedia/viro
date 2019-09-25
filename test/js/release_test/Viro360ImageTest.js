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
  ViroQuad,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var Uri360Image = {uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/360_horseshoe.jpg"};
var Local360Image = require("./res/360_park.jpg");
var Stereo360Image = require("./res/exploding_scene.jpg");
var ReleaseMenu = require("./ReleaseMenu.js");

var Viro360ImageTest = createReactClass({

  getInitialState() {
    return {
      current360Image:Stereo360Image,
      format:"RGBA8",
      rotationY: 0,
      showLeftArrow:false,
      showRightArrow:false,
    };
  },

  render: function() {
    return (
     <ViroScene>
     <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
     <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

     <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

     <Viro360Image
      rotation={[0,this.state.rotationY,0]}
      source={this.state.current360Image}
      onLoadStart={this._onBackgroundPhotoLoadStart}
      onLoadEnd={this._onBackgroundPhotoLoadEnd}
      format={this.state.format}
      stereoMode={this.state.current360Image == Stereo360Image ? "TopBottom":"None"}
      />


     <ViroText style={styles.elementText}  position={[-1,-3, -5]} width={2} height ={2}
                    text={"Toggle Format: " + this.state.format}
                    onClick={this._toggleFormat}/>
     <ViroText style={styles.elementText}  position={[1,-3, -5]} width={2} height ={2}
                         text={"Toggle Rotation: " + this.state.rotationY}
                         onClick={this._toggleRotation}/>
     <ViroImage source={require('./res/poi_dot.png')}  position={[0, -4, -3]} transformBehaviors={["billboard"]} onClick={this._showOther} />
     <ViroText text="Viro360Image" position={[0, -5, -3]} transformBehaviors={["billboard"]} />
     </ViroScene>

    );
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./Viro360VideoTest')});
  },

  _onBackgroundPhotoLoadStart(){
      console.log("Viro360ImageTest: _onLoadStart");
  },

  _onBackgroundPhotoLoadEnd() {
      console.log("Viro360ImageTest: _onLoadEnd");
  },

  _showOther() {
    var newImage;
    if (this.state.current360Image == Uri360Image){
      newImage = Local360Image;
    } else if (this.state.current360Image == Local360Image){
      newImage = Stereo360Image;
    } else {
      newImage = Uri360Image;
    }
    this.setState({
        current360Image:newImage,
      });
  },
    _toggleRotation() {
        var newRotation = this.state.rotationY + 10;
        if (newRotation == 360){
            newRotation = 0;
        }
        this.setState({
                          rotationY:newRotation
                });
    },
  _toggleFormat() {
        var newState;
        if (this.state.format == "RGBA8"){
            newState="RGB565";
        } else if (this.state.format == "RGB565"){
            newState="RGBA8";
        }
         this.setState({
                            format:newState
                        });
   },
});

ViroAnimations.registerAnimations({
  fadeIn:{properties:{scaleX:1, scaleY:1, scaleZ:1}, duration: 5000},
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
});

module.exports = Viro360ImageTest;
