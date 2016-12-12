/**
 * Copyright (c) 2015-present, Viro, Inc.
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
  ViroSceneNavigator,
  ViroScene,
  ViroBox,
  Materials,
  ViroNode,
  ViroOrbitCamera,
} from 'react-viro';

var scene1 = React.createClass({
    render: function() {
    return (
        <ViroScene position={[0,0,0]} reticleEnabled={false} >
          <ViroOrbitCamera position={[0, 0, 0]} focalPoint={[0, 0, -3]}/>
          <ViroNode position={[2, -3, 0]} >
            <ViroBox materials="wework_title" position={[0, 0, -5]} scale={[2, 4, 2]} transformBehaviors={["billboard"]} />
            <ViroBox materials="box_texture" position={[1, 5, -2]} scale={[3,3,3]} transformBehaviors={["billboard"]} />
          </ViroNode>
        </ViroScene>
    );
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
    fontSize: 20,
    textAlign: 'center',
    color: '#333333',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

Materials.createMaterials({
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
    diffuseTexture: require("./js/res/new_menu_screen.jpg"),
    fresnelExponent: .5,
  },
  box_texture: {
    diffuseTexture: require("./js/res/sun_2302.jpg"),
  }
});


module.exports = scene1;