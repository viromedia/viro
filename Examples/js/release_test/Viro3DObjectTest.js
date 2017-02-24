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

var Viro360ImageTest = require('./Viro360ImageTest');

var Viro3DObjectTest = React.createClass({

  getInitialState() {
    return {
      showLeftArrow:false,
      showPoiDot:false,
      showRightArrow:false,
    };
  },

  render: function() {
    return (
     <ViroScene>
     <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

     <Viro360Image
      rotation={[0,0,0]}
      source={{uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/360_horseshoe.jpg"}}  />


     <ViroAnimatedComponent animation="fadeIn" run={this.state.showSceneItems} loop={false}>
     <ViroImage source={{uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/poi_dot.png"}} position={[0, -4, -3]} transformBehaviors={["billboard"]} onClick={this._showFacebook} />
     </ViroAnimatedComponent>

     <ViroAnimatedComponent animation="fadeIn" run={this.state.showSceneItems} loop={false}>
     <ViroImage source={{uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/poi_dot.png"}} position={[2, -4, -3]} transformBehaviors={["billboard"]} onClick={this._showNext} />
     </ViroAnimatedComponent>

     </ViroScene>

    );
  },


  _showFacebook() {
          this.props.sceneNavigator.push({scene:FacebookScene});
      },

  _showNext() {
          this.props.sceneNavigator.push({scene:Viro360VideTest});
      },

  _moveForward1() {
      if(this.state.animationPosition1==true) {
        this.setState({
            runAnimation1: true,
            animationName1: "moveIn1",
            animationPosition1: false,
          });
        }
      else {
        this.setState({
            runAnimation1: true,
            animationName1: "moveOut1",
            animationPosition1: true,
          });
        }
      },

});

ViroAnimations.registerAnimations({
  loopRotate:{properties:{rotateY:"+10"}, duration:250},
  scaleSphere:{properties:{scaleX:1.1, scaleY:1.1, scaleZ:1.1}, duration:300},
  spinSphere:[
      ["loopRotate"],
      ["scaleSphere"],
  ],
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

module.exports = Viro3DObjectTest;
