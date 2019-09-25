
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
  ViroARScene,
  ViroARPlane,
  ViroMaterials,
  ViroImage,
  ViroARPlaneSelector,
  ViroQuad,
  ViroConstants,
  ViroARNode,
  ViroBox,
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var createReactClass = require('create-react-class');

var testARScene = createReactClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      text : "not tapped",
      visible: true,
      everythingVisible: false,
      success: false,
      video : "",
    }
  },
  render: function() {
    return (
        <ViroARScene >
          <ViroARNode position={[0,0,-1]} onDrag={()=>{}}>
            <ViroBox position={[0,.13,0]} scale={[.2,.2,.2]} />
            <ViroImage rotation={[-90,0,0]} scale={[.3,.3,.3]}
                source={require('./res/dark_circle_shadow.png')}/>
          </ViroARNode>
        </ViroARScene>
    );
  },
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
    lightingModel: "Constant",
    diffuseColor: "#ff0000"
  },
  green: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#00ff00"
  },
});

module.exports = testARScene;
