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
  ViroVideo,
  ViroOmniLight,
  ViroSceneNavigator,
  Materials,
  ViroText,
  ViroUtils,
  Viro360Photo,
  Viro360Video,
  ViroImage,
  ViroMaterials,
} from 'react-viro';

var createReactClass = require('create-react-class');
var VIDEO_REF = "videoref";
let polarToCartesian = ViroUtils.polarToCartesian;

var localVideo = require("./res/Titan_Touchdown.mp4");
var chromaVideo = require("./res/alphachica.mp4");
var localVideoStereo = require("./res/stereoVid.mp4");
var uriSource = {uri:'https://s3.amazonaws.com/viro.video/Climber1Bottom.mp4'};
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroChromaKeyTest = createReactClass({
  getInitialState() {
    return {
      materialFlag:0,
    }
  },
  render: function() {
    var chromaKeyMaterial = "noFilter";
    if (this.state.materialFlag == 1) {
        chromaKeyMaterial = "whiteFilter";
    } else if (this.state.materialFlag == 2) {
        chromaKeyMaterial = "yellowFilter";
    } else if (this.state.materialFlag == 3) {
        chromaKeyMaterial = "cyanFilter";
    } else if (this.state.materialFlag == 4) {
        chromaKeyMaterial = "greenFilter";
    } else if (this.state.materialFlag == 5) {
        chromaKeyMaterial = "purpleFilter";
    } else if (this.state.materialFlag == 6) {
        chromaKeyMaterial = "redFilter";
    } else if (this.state.materialFlag == 7) {
        chromaKeyMaterial = "blueFilter";
    } else if (this.state.materialFlag == 8) {
        chromaKeyMaterial = "blackFilter";
    }
    console.log("flag " + this.state.materialFlag + " material " + chromaKeyMaterial);

    return (
      <ViroScene>
        <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
        <ViroImage source={require('./res/tvbars.jpg')} position={[0, 0, -1]}
                   materials={[chromaKeyMaterial]}
                   transformBehaviors={["billboard"]} />
        <ViroVideo width={1.7} height={0.95}
                   position={[0, -5, -2.9]} scale={[2, 2, 2]} paused={false}
                   source={require("./res/alphachica.mp4")} transformBehaviors={["billboard"]}
                   loop={true} muted={false} volume={1.0}
                   materials={[chromaKeyMaterial]} />
        <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />
        <ViroText position={[0, -1, -1]} text={this._getToggleText()}
                  style={styles.baseTextTwo} onClick={this._toggleFilter} />
      </ViroScene>
    );
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./ViroAnimatedComponentTest')});
  },

  _getToggleText() {
    if (this.state.materialFlag == 0) {
      return "No Filter";
    } else if (this.state.materialFlag == 1) {
      return "White Filter";
    } else if (this.state.materialFlag == 2) {
      return "Yellow Filter";
    } else if (this.state.materialFlag == 3) {
      return "Cyan Filter";
    } else if (this.state.materialFlag == 4) {
      return "Green Filter";
    } else if (this.state.materialFlag == 5) {
      return "Purple Filter";
    } else if (this.state.materialFlag == 6) {
      return "Red Filter";
    } else if (this.state.materialFlag == 7) {
      return "Blue Filter";
    } else if (this.state.materialFlag == 8) {
      return "Black Filter";
    } else {
      return "Invalid Filter";
    }
  },

  _toggleFilter() {
    var newFlag = (this.state.materialFlag + 1) % 9;
    this.setState({
      materialFlag:newFlag,
    });
  },
});

ViroMaterials.createMaterials({
  noFilter: {
    shininess: 2.0,
  },
  whiteFilter: {
    chromaKeyFilteringColor: "#FFFFFF"
  },
  yellowFilter: {
    chromaKeyFilteringColor: "#FFFF00"
  },
  cyanFilter: {
    chromaKeyFilteringColor: "#00FFFF"
  },
  greenFilter: {
    chromaKeyFilteringColor: "#00FF00"
  },
  purpleFilter: {
    chromaKeyFilteringColor: "#FF00FF"
  },
  redFilter: {
    chromaKeyFilteringColor: "#FF0000"
  },
  blueFilter: {
    chromaKeyFilteringColor: "#0000FF"
  },
  blackFilter: {
    chromaKeyFilteringColor: "#000000"
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

module.exports = ViroChromaKeyTest;
