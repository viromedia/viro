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
  ViroAmbientLight
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;
var ReleaseMenu = require("./ReleaseMenu.js");
var LocalButtonImage = require("./res/icon_live.jpg");

var ViroBlendingTest = createReactClass({
  getInitialState() {
    return {

    };
  },

  render: function() {
    return (
        <ViroScene>
            <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />
            <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

            <ViroNode position={[0.8, 0, -3.5]} >
                <ViroQuad position={[-3, 0, -0.1]} scale={[1.0, 1.0, 1.0]}
                             materials="surfaceNone"
                             width={0.7}
                             height={0.7} />
                <ViroSphere position={[-3.0, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="blendNone"/>

                <ViroQuad position={[-2, 0, -0.1]} scale={[1.0, 1.0, 1.0]}
                             materials="surfaceAlpha"
                             width={0.7}
                             height={0.7} />
                <ViroSphere position={[-2.0, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="blendAlpha"/>

                <ViroQuad position={[-1, 0, -0.1]} scale={[1.0, 1.0, 1.0]}
                             materials="surfaceAdd"
                             width={0.7}
                             height={0.7} />
                <ViroSphere position={[-1.0, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="blendAdd"/>

                <ViroQuad position={[0, 0, -0.1]} scale={[1.0, 1.0, 1.0]}
                             materials="surfaceSubtract"
                             width={0.7}
                             height={0.7} />
                <ViroSphere position={[0, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="blendSubtract"/>

                <ViroQuad position={[1, 0, -0.1]} scale={[1.0, 1.0, 1.0]}
                             materials="surfaceMultiply"
                             width={0.7}
                             height={0.7} />
                <ViroSphere position={[1.0, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="blendMultiply"/>

                <ViroQuad position={[2, 0, -0.1]} scale={[1.0, 1.0, 1.0]}
                             materials="surfaceScreen"
                             width={0.7}
                             height={0.7} />
                <ViroSphere position={[2.0, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="blendScreen"/>
            </ViroNode>
        </ViroScene>
    );
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./GroupTestDragEvents')});
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
  blendNone: {
    blendMode: "None",
    diffuseColor: "#FF000088",
  },
  blendAlpha: {
    blendMode: "Alpha",
    diffuseColor: "#FF000088",
  },
  blendAdd: {
    blendMode: "Add",
    diffuseColor: "#FF000088",
  },
  blendSubtract: {
    blendMode: "Subtract",
    diffuseColor: "#FF000088",
  },
  blendMultiply: {
    blendMode: "Multiply",
    diffuseColor: "#88888888",
  },
  blendScreen: {
    blendMode: "Screen",
    diffuseColor: "#88888888",
  },

  surfaceNone: {
    diffuseColor: "#FFFFFF",
  },
  surfaceAlpha: {
    diffuseColor: "#FFFFFF",
  },
  surfaceAdd: {
    diffuseColor: "#00FF00",
  },
  surfaceSubtract: {
    diffuseColor: "#FFFF00",
  },
  surfaceMultiply: {
    diffuseColor: "#888888",
  },
  surfaceScreen: {
    diffuseColor: "#888888",
  },
});

module.exports = ViroBlendingTest;
