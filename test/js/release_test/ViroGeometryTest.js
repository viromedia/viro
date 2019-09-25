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
  ViroMaterials,
  ViroGeometry,
  ViroNode,
  ViroImage,
  ViroUtils,
  ViroText,
  ViroButton,
  ViroAmbientLight
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroGeometryTest = createReactClass({
  getInitialState() {
    return {

    };
  },

  render: function() {
    return (
        <ViroScene>
            <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />
            <ViroAmbientLight color="#FFFFFF" />

            <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

            <ViroNode position={[0.8, 0, -3.5]} >
                <ViroGeometry position={[-2.5, 0, -0.1]} scale={[1.0, 1.0, 1.0]}
                              materials="geometryBlue"
                              vertices={[ [1, 0, 0], [0, 1, 0], [-1, 0, 0] ]}
                              normals={[ [0, 0, 1], [0, 0, 1], [0, 0, 1] ]}
                              texcoords={[ [1, 0], [0, 1], [-1, 0] ]}
                              triangleIndices={[ [0, 1, 2 ] ]} />
                <ViroText position={[-2.5, -0.5, -0.1]} text="Blue Triangle" />

                <ViroGeometry position={[-0.5, 0, -0.1]} scale={[1.0, 1.0, 1.0]}
                              materials="geometryRed"
                              vertices={[ [1, 0, 0], [0, 1, 0], [-1, 0, 0] ]}
                              triangleIndices={[ [0, 1, 2 ] ]} />
                <ViroText position={[-0.5, -0.5, -0.1]} text="Red Triangle" />

                <ViroGeometry position={[1, 0, -0.1]} scale={[1.0, 1.0, 1.0]}
                              materials="geometryTexture"
                              vertices={[ [0, 0, 0], [1, 0, 0], [1, 1, 0], [0, 1, 0] ]}
                              normals={[ [0, 0, 1], [0, 0, 1], [0, 0, 1], [0, 0, 1] ]}
                              texcoords={[ [0, 0], [1, 0], [1, 1], [0, 1] ]}
                              triangleIndices={[ [0, 1, 2, 2, 3, 0] ]} />
                <ViroText position={[2, -0.5, -0.1]} width={2} text="Textured Square" />

                <ViroGeometry position={[0, -2, -0.1]} scale={[1.0, 1.0, 1.0]}
                              materials={["geometryTexture", "geometryTexture"]}
                              vertices={[ [-1, 0, 0], [0, 0, 0], [0, 1, 0], [-1, 1, 0],
                                          [ 1, 0, 0], [2, 0, 0], [2, 1, 0], [ 1, 1, 0]]}
                              normals={[ [0, 0, 1], [0, 0, 1], [0, 0, 1], [0, 0, 1],
                                         [0, 0, 1], [0, 0, 1], [0, 0, 1], [0, 0, 1]]}
                              texcoords={[ [0, 0], [1, 0], [1, 1], [0, 1],
                                           [0, 0], [1, 0], [1, 1], [0, 1] ]}
                              triangleIndices={[ [0, 1, 2, 2, 3, 0],
                                                 [4, 5, 6, 6, 7, 4] ]} />
                <ViroText position={[0.5, -2.5, -0.1]} width={2} text="Two Textured Squares" />
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
  geometryBlue: {
    diffuseColor: "#0000FFFF",
  },
  geometryRed: {
    diffuseColor: "#FF0000FF",
  },
  geometryTexture: {
    diffuseTexture: require('./res/grid_bg.jpg')
  },
});

module.exports = ViroGeometryTest;
