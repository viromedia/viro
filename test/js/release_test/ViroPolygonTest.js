/**
 * Sample React Native App
 * https://github.com/facebook/react-native
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
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroCamera,
  ViroUtils,
  ViroText,
  ViroSkyBox,
  ViroPolygon
} from 'react-viro';

var createReactClass = require('create-react-class');
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroPolygonTest = createReactClass({
  getInitialState() {
    return {

    };
  },
  render: function() {

    return (
      <ViroScene>
        <ViroCamera position={[0, 0, 0]} active={!this.state.orbit}/>
        <ViroOrbitCamera focalPoint={[0, -.25, -2]} active={this.state.orbit}/>
        <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

        <ViroNode position={[0, 0, -5]}>
          <ViroPolygon vertices={[ [-1, -1], [-1, 1], [1, 1], [1, -1] ]}
                       position={[-3, 0, 0]}
                       materials={"bluePolygon"} />
          <ViroPolygon vertices={[ [ -1, -1], [-1, 1], [1, 1], [1, 0.5],
                                   [0.5, 0.5], [0.5, 0.25], [1, 0.25], [1, -1] ]}
                       position={[0, 0, 0]}
                       materials={"bluePolygon"} />
          <ViroPolygon vertices={[ [ -1, -1], [-1, 1], [1, 1], [1, 0.5],
                                   [0.5, 0.5], [0.5, 0.25], [1, 0.25], [1, -1] ]}
                       holes={[ [[-0.75, -0.75], [-0.75, -0.50], [-0.50, -0.50], [-0.50, -0.75]],
                                [[ 0.75, -0.75], [0.75, -0.50], [0.50, -0.50], [0.50, -0.75]] ]}
                       position={[3, 0, 0]}
                       materials={"bluePolygon"} />
        </ViroNode>
      </ViroScene>
    );
  },
});

var styles = StyleSheet.create({
  baseTextTwo: {
      fontFamily: 'Arial',
      fontSize: 15,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});

ViroMaterials.createMaterials({
  bluePolygon: {
    diffuseColor: "#0000FF",
  },
});

module.exports = ViroPolygonTest;
