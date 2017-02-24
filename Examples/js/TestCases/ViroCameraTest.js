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
  ViroSurface,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;

var ViroButtonTest = require('./ViroButtonTest');
var ViroControllerTest = require('./ViroControllerTest');

// **This test has not been done.  This is placeholder for scene and navigation arrows**

var ViroCameraTest = React.createClass({

  getInitialState() {
    return {

    };
  },

  render: function() {
    return (
     <ViroScene>
     <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

     <Viro360Image
      rotation={[0,0,0]}
      source={require("./res/360_park.jpg")}
      />


      <ViroImage source={require('./res/icon_left_w.png')} position={[-2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showPrevious} />
      <ViroText text="ViroCamera" position={[0, -5, -3]} transformBehaviors={["billboard"]} />
      <ViroImage source={require('./res/icon_right_w.png')} position={[2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showNext} />


     </ViroScene>

    );
  },

  _showPrevious() {
    this.props.sceneNavigator.pop();
  },

  _showNext() {
    this.props.sceneNavigator.push({scene:ViroControllerTest});
  },


});


module.exports = ViroCameraTest;
