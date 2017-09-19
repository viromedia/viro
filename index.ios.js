/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */
'use strict';

import React, { Component } from 'react';

import {
  AppRegistry,
} from 'react-native';

import {
  ViroSceneNavigator,
  ViroARSceneNavigator,
} from 'react-viro';


var scenes = {
    '360 Photo Tour': require('./js/360PhotoTour/MainScene'),
    'Hello World': require('./js/HelloWorld/HelloWorldScene'),
    'Human Body': require('./js/HumanBody/MainScene'),
    'ProductShowcase': require('./js/ProductShowcase/ProductShowcase'),
    'Viro Media Player': require('./js/ViroMediaPlayer/ViroTheatre'),
    'AR Sample': require('./js/ARSample/HelloWorldSceneAR.js'),
}

var showARScene = false;

var ViroCodeSamplesSceneNavigator = React.createClass({
  render: function() {
    // The 'viroAppProps={{...this.props}}' line below is used to pass
    // the initial properties from this base component to the ViroSceneNavigator
    // which will allow the scenes to access them.
    if (showARScene) {
      return (
        <ViroARSceneNavigator
          initialScene={{
            scene: scenes['AR Sample'],
          }}
          apiKey="YOUR_API_KEY_HERE"/>
        );
    } else {
      return (
        <ViroSceneNavigator
          initialScene={{
            scene: scenes['Viro Media Player'],
          }}
          viroAppProps={{...this.props}}
          apiKey="YOUR_API_KEY_HERE"/>
      );

    }
  }
});

AppRegistry.registerComponent('ViroCodeSamples', () => ViroCodeSamplesSceneNavigator);

// register the component again as ViroSample to enable use with the testbed app
AppRegistry.registerComponent('ViroSample', () => ViroCodeSamplesSceneNavigator);
