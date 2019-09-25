
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
      intensity : 1000.0,
    }
  },
  render: function() {
    return (
        <ViroScene ref="arscene">
          <ViroAmbientLight color={"#ff0000"} intensity={this.state.intensity} />
          <ViroBox position={[0,0,-2]} scale={[1,1,1]} materials={"box"} onClick={this._onClick}/>
        </ViroScene>
    );
  },
  _onClick() {
    if (this.state.intensity >= 1200) {
      this.setState({
        intensity : 300
      })
    } else {
      this.setState({
        intensity : this.state.intensity + 100
      })
    }
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
