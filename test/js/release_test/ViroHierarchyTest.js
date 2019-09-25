/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
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
  ViroSurface,
  ViroText,
  ViroAnimations,
  ViroAnimatedComponent,
  ViroQuad,
  ViroSkyBox,
} from 'react-viro';

var createReactClass = require('create-react-class');
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroHierarchyTest = createReactClass({

  getInitialState() {
    return {
      color:1,
      fontSize:100,
    }
  },

  render: function() {
    var newStyle = styles.centeredText;

    return (
      <ViroScene reticleEnabled={true} onPlatformUpdate={this._platformUpdate} >
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

        <ViroSkyBox color="#ff69b4" />
        <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

        <ViroNode  position={[0, 0, -50]}>

          <ViroFlexView style={styles.containerVertical} width={70} height={70} materials="redColorPlane" position={[0, 0, 0]}>
            <ViroText fontSize={this.state.fontSize} style={newStyle}  text="This is Large" textAlign={"center"} textAlignVertical={"center"}/>
          </ViroFlexView>

          <ViroFlexView style={styles.containerVertical} width={20} height={20} materials="greenColorPlane" position={[-20, 0, 1]}>
            <ViroText fontSize={this.state.fontSize} style={newStyle}  text="This is Large" textAlign={"center"} textAlignVertical={"center"}/>
          </ViroFlexView>

          <ViroFlexView style={styles.containerVertical} width={20} height={20} materials="blueColorPlane" position={[20, 0, 1]}>
            <ViroText fontSize={this.state.fontSize} style={newStyle}  text="This is Large" textAlign={"center"} textAlignVertical={"center"}/>
          </ViroFlexView>

          <ViroQuad position={[-30, -30,  1]} width={10} height={10}/>
          <ViroQuad position={[ 30,  30,  1]} width={10} height={10}/>
          <ViroQuad position={[ 30, -30, -1]} width={10} height={10}/>
          <ViroQuad position={[-30,  30, -1]} width={10} height={10}/>

       </ViroNode>

      </ViroScene>
    );
  },

  _showNext() {
     this.props.sceneNavigator.replace({scene:require('./Viro3DObjectTest')});
  },
});


var styles = StyleSheet.create({
  containerVertical: {
    flexDirection: 'column',
  },
  centeredText: {
    fontFamily: 'Arial',
    color: '#ffffff',
    flex: 1,
  },
  containerVertical: {
    flexDirection: 'column',
  },
});

ViroMaterials.createMaterials({
  redColorPlane: {
    diffuseColor: "#ff0000"
  },
  blueColorPlane: {
    diffuseColor: "#0000ff"
  },
  greenColorPlane: {
    diffuseColor: "#00ff00"
  },
});

module.exports = ViroHierarchyTest;
