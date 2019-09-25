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
  ViroText,
  Viro3DObject,
  ViroDirectionalLight,
  ViroQuad,
  ViroQuadEmitter,
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var createReactClass = require('create-react-class');

var quad = {
  source : require("./res/ironman.png"),
  height : .1,
  width : .1,
}

var spawnModifier = {
  emissionRatePerSecond : [50, 50],
  emissionRatePerMeter : [10, 10],
  particleLifetime: 1500,
  maxParticles: 300,
  despawnDistance: .5,
  emissionBurst: [
    {
      time : 1000,
      min : 10,
      max : 20,
      cycles : 5,
      cooldownPeriod : 500, 
    },
  ],
  spawnVolume : {
    shape : "sphere",
    size : [.1]
  }
}

var appearanceModifier = {
  stretchFactor : [2,1,1],
  opacity : {
    min : 1,
    max : 1,
    modifier : [
      {
        factor : "time",
        finalValue : .5,
        interval : [0, 1000]
      },
      {
        factor : "time", 
        finalValue : 1,
        interval : [1000, 1500]
      }
    ]
  }
}

var physicsModifier = {
  velocity : {
    min : [0,1,0],
    max : [0,2,0]
  },
  acceleration : {
    min : [0, -10, 0],
    max : [0, -15, 0],
    modifier : [
      {
        factor : "time",
        finalValue : [0, -20, 0],
        interval : [0, 100]
      }
    ]
  }
}

var testARScene = createReactClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      text : "not tapped",
      showObj : true,
    }
  },
  render: function() {
    return (
        <ViroARScene onTrackingInitialized={()=>{console.log("ARScene tracking initialized!")}} >
          <ViroDirectionalLight color="#ffffff" direction={[0,0,-1]} />
          {this._showObj()}
          <ViroARPlane minHeight={0} maxHeight={0} onAnchorFound={()=>{console.log("ARPlane component 2 found!")}} >
            <ViroQuad rotation={[-90,0,0]} materials={"red"} opacity={.5}
              onClick={this._pushNextScene} />
          </ViroARPlane>

          <ViroQuadEmitter 
            position={[0,0,-1]}
            duration={5000}
            loop={true}
            run={true}
            quad={quad}
            spawnModifier={spawnModifier}
            appearanceModifier={appearanceModifier}
            physicsModifier={physicsModifier}/>
        </ViroARScene>
    );
  },
  _pushNextScene() {
    this.props.sceneNavigator.push({'scene' : require("./selectASurface.js")});
  },
  _showObj() {

    if (this.state.showObj) {
      return (
        <ViroARPlane minHeight={0} maxHeight={0} onAnchorFound={()=>{console.log("ARPlane component 1 found!")}} >
          <ViroQuad rotation={[-90,0,0]} materials={"blue"} opacity={.5} />
          <Viro3DObject source={require('./res/aliengirl.obj')} scale={[.005,.005,.005]}
                  materials={["aliengirl"]} type="OBJ"
                  onClick={()=>{this.setState({showObj : false})}} />
        </ViroARPlane>)
    }
  },
  _onTap() {
    console.log("tapped video!!!!")
    this.setState({
      text : "tapped!"
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
  aliengirl: {
    lightingModel: "Constant",
    diffuseTexture: require('./res/aliengirl_diffuse.png'),
    normalTexture: require('./res/aliengirl_normal.png'),
    specularTexture: require('./res/aliengirl_specular.png')
  },
  blue: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#0000ff"
  },
  black: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#000000"
  },
  red: {
    shininess: 2.0,
    lightingModel: "Constant",
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
