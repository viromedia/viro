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

var ViroColorMaskTest = createReactClass({
  getInitialState() {
    return {
      occludingSurfaceVisible: false,
    };
  },

  render: function() {
    return (
        <ViroScene>
            <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />
            <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

            <ViroNode position={[0.8, 0, -3.5]} >
                <ViroSphere position={[-3, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="maskAll"/>
                <ViroText position={[-3, -0.85, 0]} fontSize={16} text="White" textAlign="Center" />

                <ViroSphere position={[-2, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="maskRed"/>
                <ViroText position={[-2, -0.85, 0]} fontSize={16} text="Red" textAlign="Center" />


                <ViroSphere position={[-1, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="maskGreen"/>
                <ViroText position={[-1, -0.85, 0]} fontSize={16} text="Green" textAlign="Center" />

                <ViroSphere position={[0, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="maskBlue"/>
                <ViroText position={[0, -0.85, 0]} fontSize={16} text="Blue" textAlign="Center" />

                <ViroSphere position={[1, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="maskPurple"/>
                <ViroText position={[1, -0.85, 0]} fontSize={16} text="Purple" textAlign="Center" />

                <ViroSphere position={[2, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="maskYellow"/>
                <ViroText position={[2, -0.85, 0]} fontSize={16} text="Yellow" textAlign="Center" />

                <ViroSphere position={[3, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="maskCyan"/>
                <ViroText position={[3, -0.85, 0]} fontSize={16} text="Cyan" textAlign="Center" />

                <ViroQuad position={[0, -2, 0.5]} scale={[1.0, 1.0, 1.0]}
                             materials="maskTransparentOcclude"
                             width={1.2}
                             height={1.2}
                             renderingOrder={-1}
                             visible={this.state.occludingSurfaceVisible} />
                <ViroSphere position={[0, -2, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials="maskSphereBlue"/>
                <ViroText position={[0, -3.85, 0]} fontSize={20} text="Toggle Occlusion" textAlign="Center"
                          onClick={this._toggleOcclusion} />

            </ViroNode>
        </ViroScene>
    );
  },

  _toggleOcclusion() {
    var newVisible = !this.state.occludingSurfaceVisible;
    this.setState({
      occludingSurfaceVisible:newVisible,
    });
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
  maskAll: {
    colorWriteMask: ["All"],
    diffuseColor: "#FFFFFFFF",
  },
  maskRed: {
    colorWriteMask: ["Red"],
    diffuseColor: "#FFFFFFFF",
  },
  maskGreen: {
    colorWriteMask: ["Green"],
    diffuseColor: "#FFFFFFFF",
  },
  maskBlue: {
    colorWriteMask: ["Blue"],
    diffuseColor: "#FFFFFFFF",
  },
  maskPurple: {
    colorWriteMask: ["Red", "Blue"],
    diffuseColor: "#FFFFFFFF",
  },
  maskCyan: {
    colorWriteMask: ["Blue", "Green"],
    diffuseColor: "#FFFFFFFF",
  },
  maskYellow: {
    colorWriteMask: ["Red", "Green"],
    diffuseColor: "#FFFFFFFF",
  },

  maskSphereBlue: {
    diffuseColor: "#0000FFFF",
  },
  maskTransparentOcclude: {
    diffuseColor: "#FFFFFFFF",
    colorWriteMask: ["None"],
  },
});

module.exports = ViroColorMaskTest;
