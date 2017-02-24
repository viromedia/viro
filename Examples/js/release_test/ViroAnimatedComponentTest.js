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

var ViroAmbientLightTest = require('./ViroAmbientLightTest');
var ViroBoxTest = require('./ViroBoxTest');

var ViroAnimatedComponentTest = React.createClass({

  getInitialState() {
    return {
      animationName1:"loopRotate",
      runAnimation1:false,
      animationName2:"loopRotate",
      runAnimation2:false,
      animationName3:"loopRotate",
      runAnimation3:false,
    };
  },

  render: function() {
    return (
     <ViroScene>
     <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

      <ViroAnimatedComponent
      animation="loopRotate"
      run={true}
      loop={true}>
      <ViroSphere
        heightSegmentCount={20}
        widthSegmentCount={20}
        radius={.5}
        position={[0, 0, -3]}
        materials={["sphere1"]}
        facesOutward={false}
        onClick={this._startAnimation1}
        />
      </ViroAnimatedComponent>

      <ViroAnimatedComponent
      animation={this.state.animationName1}
      run={this.state.runAnimation1}
      onStart={this._startAnimation2}
      onFinish={this._startAnimation3}>
      <ViroSphere
        heightSegmentCount={20}
        widthSegmentCount={20}
        radius={.5}
        position={[-3, -3, -5]}
        materials={["sphere1"]}
        facesOutward={false}
        />
      </ViroAnimatedComponent>

      <ViroAnimatedComponent
      animation={this.state.animationName2}
      run={this.state.runAnimation2}
      delay={1000}>
      <ViroSphere
        heightSegmentCount={20}
        widthSegmentCount={20}
        radius={.5}
        position={[2, -2, -5]}
        materials={["sphere1"]}
        facesOutward={false}
        />
      </ViroAnimatedComponent>

      <ViroAnimatedComponent
      animation={this.state.animationName3}
      run={this.state.runAnimation3}>
      <ViroBox
        heightSegmentCount={20}
        widthSegmentCount={20}
        radius={.5}
        position={[-3, 0, -5]}
        materials={["sphere1"]}
        />
      </ViroAnimatedComponent>

      <ViroImage source={require('./res/icon_left_w.png')} position={[-2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showPrevious} />
      <ViroText text="ViroAnimatedComponent" position={[0, -5, -3]} transformBehaviors={["billboard"]} />
      <ViroImage source={require('./res/icon_right_w.png')} position={[2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showNext} />

     </ViroScene>

    );
  },

  _showPrevious() {
    this.props.sceneNavigator.pop();
  },

  _startAnimation1() {
    this.setState({
        animationName1:"moveRight",
        runAnimation1:true
      });
  },

  _startAnimation2() {
    this.setState({
        animationName2:"moveDown",
        runAnimation2:true
      });
  },

  _startAnimation3() {
    this.setState({
        animationName3:"scaleSphere",
        runAnimation3:true
      });
  },

  _showNext() {
    this.props.sceneNavigator.push({scene:ViroAnimatedComponentTest});
  },

});

ViroAnimations.registerAnimations({
  fadeIn:{properties:{scaleX:1, scaleY:1, scaleZ:1}, duration: 5000},
  loopRotate:{properties:{rotateY:"+10"}, duration:250},
  scaleSphere:{properties:{scaleX:3, scaleY:3, scaleZ:3}, duration:5000},
  moveRight:{properties:{positionX:"3", positionY:"3", positionZ:"-3",}, duration:5000},
  moveDown:{properties:{positionY:"+3",}, duration:5000},
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


module.exports = ViroAnimatedComponentTest;
