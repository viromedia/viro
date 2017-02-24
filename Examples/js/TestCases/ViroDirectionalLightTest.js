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

var ViroControllerTest = require('./ViroControllerTest');
var ViroFlexViewTest = require('./ViroFlexViewTest');

var ViroDirectionalLightTest = React.createClass({

  getInitialState() {
    return {
      setColor:"#ffffff",
      setDirection:[0,-1,0]
    };
  },

  render: function() {
    return (
     <ViroScene>
     <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

     <ViroDirectionalLight
         color={this.state.setColor}
         direction={this.state.setDirection}
      />

      <ViroSphere
        heightSegmentCount={20}
        widthSegmentCount={20}
        radius={1}
        position={[0, 0, -3]}
        materials={["sphere1"]}
        facesOutward={false}
        />


      <ViroImage source={require('./res/icon_left_w.png')} position={[-2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showPrevious} />

      <ViroImage source={require('./res/poi_dot.png')} position={[-2, -2, -3]} transformBehaviors={["billboard"]} onClick={this._colorWhite} />
      <ViroText text="Green/Above" position={[-2, -3, -3]} height={1} width={1} transformBehaviors={["billboard"]} />

      <ViroImage source={require('./res/poi_dot.png')} position={[0, -2, -3]} transformBehaviors={["billboard"]} onClick={this._colorRed} />
      <ViroText text="Red/Left" position={[0, -3, -3]} height={1} width={1} transformBehaviors={["billboard"]} />

      <ViroImage source={require('./res/poi_dot.png')} position={[2, -2, -3]} transformBehaviors={["billboard"]} onClick={this._colorBlue} />
      <ViroText text="Blue/Back" position={[2, -3, -3]} height={1} width={1} transformBehaviors={["billboard"]} />

      <ViroText text="ViroDirectionalLight" position={[0, -5, -3]} transformBehaviors={["billboard"]} />

      <ViroImage source={require('./res/icon_right_w.png')} position={[2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showNext} />

     </ViroScene>

    );
  },

  _showPrevious() {
    this.props.sceneNavigator.pop();
  },

  _colorWhite() {
    this.setState({
        setColor:"#80FF00",
        setDirection:[0,1,0],
      });
  },

  _colorRed() {
    this.setState({
        setColor:"#ff0000",
        setDirection:[1,0,0],
      });
  },

  _colorBlue() {
    this.setState({
        setColor:"#7F00FF",
        setDirection:[0,0,-1],
      });
  },

  _showNext() {
    this.props.sceneNavigator.push({scene:ViroFlexViewTest});
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

ViroMaterials.createMaterials({

   sphere1: {
     shininess : 2.0,
     lightingModel: "Blinn",
     diffuseTexture: require('./res/360_waikiki.jpg'),
   },

});


module.exports = ViroDirectionalLightTest;
