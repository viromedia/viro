'use strict';

import React, { Component } from 'react';

import {StyleSheet} from 'react-native';

import {
  ViroScene,
  ViroText,
  Viro360Image,
  ViroOrbitCamera,
  ViroCamera,
  ViroFlexView,
  ViroMaterials,
  ViroUtils,
  ViroQuad,
  ViroSound,
  ViroSpatialSound,
  ViroAnimations, 
  ViroAnimatedComponent,
  ViroNode,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;

var createReactClass = require('create-react-class');

var gameScene = require('./WhackAMole');

var MainMenu = createReactClass({
  getInitialState() {
    return {
      hideStart : false,
      playMusic : true,
    };
  },
  render: function() {
    return (
      <ViroScene>
        <ViroSound source={require("./res/bensound-ofeliasdream.mp3")} paused={!this.state.playMusic}
          loop={true} onFinish={this._successOnFinish} />

        <ViroAnimatedComponent animation="fadeAway" run={this.state.hideStart} loop={false} >
          <ViroNode onClick={this._onStartClick} >
            <ViroText text={"Click to begin"} height={2}
              position={[0, 0, -3]} transformBehaviors={"billboard"} style={styles.helloWorldTextStyle} />
            <ViroText text={"Click to begin"} height={2}
              position={[0, 0, 3]} transformBehaviors={"billboard"} style={styles.helloWorldTextStyle} />
            <ViroText text={"Click to begin"} height={2}
              position={[-3, 0, 0]} transformBehaviors={"billboard"} style={styles.helloWorldTextStyle} />
            <ViroText text={"Click to begin"} height={2}
              position={[3, 0, 0]} transformBehaviors={"billboard"} style={styles.helloWorldTextStyle} />
          </ViroNode>
        </ViroAnimatedComponent>

        <ViroAnimatedComponent animation="fadeIn" run={this.state.hideStart} loop={false} >
          <ViroNode onClick={this._startGame} opacity={0}>
            <ViroText text={"Click again to begin"} height={2}
              position={[0, 0, -3]} transformBehaviors={"billboard"} style={styles.helloWorldTextStyle} />
            <ViroText text={"Click again to begin"} height={2}
              position={[0, 0, 3]} transformBehaviors={"billboard"} style={styles.helloWorldTextStyle} />
            <ViroText text={"Click again to begin"} height={2}
              position={[-3, 0, 0]} transformBehaviors={"billboard"} style={styles.helloWorldTextStyle} />
            <ViroText text={"Click again to begin"} height={2}
              position={[3, 0, 0]} transformBehaviors={"billboard"} style={styles.helloWorldTextStyle} />
          </ViroNode>
        </ViroAnimatedComponent>

      </ViroScene>
    );
  },
  /*
          <ViroSound source={{uri: 'http://www.bensound.com/royalty-free-music?download=ofeliasdream'}} paused={false}
          loop={true} onFinish={this._successOnFinish} />

          <ViroSound source={require("./res/bensound-ofeliasdream.mp3")} paused={false}
          loop={true} onFinish={this._successOnFinish} />
  */
  _onStartClick() {
    this.setState({
      hideStart : true,
    });
  },
  _startGame() {
    this.setState({
      playMusic : false
    })
    this.props.sceneNavigator.push({scene:gameScene});
  }
});

var styles = StyleSheet.create({
  helloWorldTextStyle: {
    fontFamily: 'Arial',
    fontSize: 50,
    color: '#ffffff',
    flex: 1,
    textAlignVertical: 'center',
    textAlign: 'center',  
  },
});

ViroMaterials.createMaterials({
  label_flex_view: {
    lightingModel: "Constant",
    diffuseTexture: require('./res/textures/text_box.png'),
  },
  redColor: {
    diffuseColor: "#ff0000"
  },
  blueColor: {
    diffuseColor: "#0000ff"
  },
});

ViroAnimations.registerAnimations({
  fadeAway:{properties:{opacity:"0"}, duration:1000},
  fadeIn:{properties:{opacity:"1"}, duration:1000},
});

module.exports = MainMenu;

