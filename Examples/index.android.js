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
} from 'react-viro';

export default class ViroSample extends Component {
    render() {
        return (
                <ViroSceneNavigator>
                  <ViroScene />
                </ViroSceneNavigator>
            );
    }
};

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
    lightingModel: "Lambert",
    // TODO: we currently can't create any image-backed resources
    //diffuseTexture: require("./js/res/new_menu_screen.jpg"),
    readsFromDepthBuffer: false,
    cullMode: "None",
    fresnelExponent: .5,
  },
});

AppRegistry.registerComponent('ViroSample', () => ViroSample);
