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

var ViroAmbientLightTest = React.createClass({

  getInitialState() {
    return {
      setColor:"#ffffff",
    };
  },

  render: function() {
    return (
     <ViroScene>
     <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

     <ViroAmbientLight
         color={this.state.setColor}
      />

      <ViroSphere
        heightSegmentCount={20}
        widthSegmentCount={20}
        radius={1}
        position={[0, 0, -3]}
        materials={["sphere1"]}
        facesOutward={false}
        />


      <ViroAnimatedComponent animation="fadeIn" run={true} loop={false}>
      <ViroImage source={require('./res/icon_left_w.png')} position={[-2, -4, -3]} scale={[0, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showPrevious} />
      </ViroAnimatedComponent>

      <ViroImage source={require('./res/poi_dot.png')} position={[-2, -2, -3]} transformBehaviors={["billboard"]} onClick={this._colorWhite} />
      <ViroText text="White" position={[-2, -3, -3]} transformBehaviors={["billboard"]} />

      <ViroImage source={require('./res/poi_dot.png')} position={[0, -2, -3]} transformBehaviors={["billboard"]} onClick={this._colorRed} />
      <ViroText text="Red" position={[0, -3, -3]} transformBehaviors={["billboard"]} />

      <ViroImage source={require('./res/poi_dot.png')} position={[2, -2, -3]} transformBehaviors={["billboard"]} onClick={this._colorBlue} />
      <ViroText text="Blue" position={[2, -3, -3]} transformBehaviors={["billboard"]} />

      <ViroText text="ViroAmbientLight" position={[0, -5, -3]} transformBehaviors={["billboard"]} />

      <ViroAnimatedComponent animation="fadeIn" run={true} loop={false}>
      <ViroImage source={require('./res/icon_right_w.png')} position={[2, -4, -3]} scale={[0, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />
      </ViroAnimatedComponent>

     </ViroScene>

    );
  },

  _showPrevious() {
    this.props.sceneNavigator.pop();
  },

  _colorWhite() {
    this.setState({
        setColor:"#ffffff",
      });
  },

  _colorRed() {
    this.setState({
        setColor:"#ff0000",
      });
  },

  _colorBlue() {
    this.setState({
        setColor:"#0000ff",
      });
  },

  _showNext() {
    this.props.sceneNavigator.push({scene:ViroAnimatedComponentTest});
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


module.exports = ViroAmbientLightTest;
