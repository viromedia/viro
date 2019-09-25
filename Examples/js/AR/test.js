/**
 * Copyright (c) 2017-present, Viro Media, Inc.
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
  ViroScene,
  ViroARScene,
  ViroARPlane,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroImage,
  ViroVideo,
  ViroSkyBox,
  Viro360Video,
  ViroText
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var createReactClass = require('create-react-class');

var testARScene = createReactClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      text : "not tapped",
      visible: true,
      everythingVisible: false
    }
  },
  render: function() {
    return (
        <ViroARScene position={[0,0,0]} reticleEnabled={false} onTrackingInitialized={()=>{this.setState({"everythingVisible": true})}}>
            <ViroNode visible={this.state.everythingVisible} >
            <ViroVideo
              height={.2} width={.2} position={[0,.15,-.5]}
              visible={this.state.visible}
              onClick={()=>{this._onTap()}}
              source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber2Bottom.mp4"}}
              transformConstraints={"billboard"}
            />

            <ViroVideo
              height={.2} width={.2} position={[.5,.15,-.5]}
              onClick={()=>{this._onTap()}}
              source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}}
              transformConstraints={"billboard"}
            />

            <ViroVideo
              height={.2} width={.2} position={[-.5,.15,-.5]}
              onClick={()=>{this._onTap()}}
              source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}}
            />

            <ViroVideo
              height={.2} width={.2} position={[-.5,-.5,-.5]}
              onClick={()=>{this._onTap()}}
              source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}}
              onDrag={()=>{console.log("dragging!!!!")}}
              transformConstraints={"billboard"}
            />

            <ViroText style={styles.welcome} position={[0,-.5, -1]} text={this.state.text} />
            <ViroText style={styles.welcome} position={[-.5,-.5, -1]} text={this.state.text} />
            <ViroText style={styles.welcome} position={[.5,-.5, -1]} text={this.state.text} />

          </ViroNode>

        </ViroARScene>
    );
  },
  /*
          <ViroARPlane minHeight={0} maxHeight={0} >
            <ViroVideo
              height={.3} width={.3} position={[0,.15,0]} onFinish={this._onFinish("Video")}
              onClick={()=>{console.log("tapped video!!!!")}}
              onDrag={()=>{console.log("dragging!!!!")}}
              source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}}
            />
          </ViroARPlane>



          <ViroARPlane minHeight={.5} minWidth={1} >
            <ViroBox materials="wework_title" scale={[.3, .3, .3]} position={[0, .15, 0]} />
          </ViroARPlane>
  */
  _onTap() {
    console.log("tapped video!!!!")
    this.setState({
      text : "tapped!",
      visible: !this.state.visible
    })
    this.setTimeout( () => {
      this.setState({
        text : "not tapped"
      });
    }, 1000);
  },
  _onLoadStart(component) {
    return () => {
      console.log("scene1 " + component + " load start");
    }
  },
  _onLoadEnd(component) {
    return () => {
      console.log("scene1 " + component + " load end");
    }
  },
  _onFinish(component) {
    return () => {
      console.log("scene1 " + component + " finished");
    }
  }
});

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#FFFFFF',
  },
  welcome: {
    fontSize: 13,
    textAlign: 'center',
    color: '#ffffff',
    margin: 2,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

ViroMaterials.createMaterials({
  blue: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#0000ff"
  },
  black: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#000000"
  },
  red: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#ff0000"
  },
  wework_title: {
    shininess: 1.0,
    lightingModel: "Constant",
    diffuseTexture: {"uri": "https://s3-us-west-2.amazonaws.com/viro/guadalupe_360.jpg"},
    diffuseTexture: require("../res/new_menu_screen.jpg"),
    fresnelExponent: .5,
  },
  box_texture: {
    diffuseTexture: require("../res/sun_2302.jpg"),
  }
});

module.exports = testARScene;
