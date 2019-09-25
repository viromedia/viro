
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
  ViroQuad,
  ViroAmbientLight
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var createReactClass = require('create-react-class');

var testARScene = createReactClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      intensity : 5.0,
    }
  },
  render: function() {
    return (
        <ViroARScene ref="arscene" onAmbientLightUpdate={this._ambientLightUpdate}>
          <ViroAmbientLight color={"#ff0000"} intensity={this.state.intensity} />
          <ViroBox position={[0,0,-1]} scale={[.1,.1,.1]} materials={"box"} />
        </ViroARScene>
    );
  },
  _ambientLightUpdate(lightInfo) {
    console.log("kirby intensity " + lightInfo.intensity);
    this.setState({
      intensity : lightInfo.intensity
    })
  }
});

ViroMaterials.createMaterials({
  transparent: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#00000055"
  },
  box :{
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseColor: "#ffffff"
  }
});

module.exports = testARScene;
