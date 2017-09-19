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

/*
 * TODO: Add your API key below!
 */
var apiKey = "YOUR_API_KEY_HERE";

var vrScenes = {
    '360 Photo Tour': require('./js/360PhotoTour/MainScene'),
    'Hello World': require('./js/HelloWorld/HelloWorldScene'),
    'Human Body': require('./js/HumanBody/MainScene'),
    'ProductShowcase': require('./js/ProductShowcase/ProductShowcase'),
    'Viro Media Player': require('./js/ViroMediaPlayer/ViroTheatre'),
}

var arScenes = {
  'AR Sample': require('./js/ARSample/HelloWorldSceneAR.js'),
}

var showARScene = true;

var ViroCodeSamplesSceneNavigator = React.createClass({
  render: function() {

    if (showARScene) {
      return (
        <ViroARSceneNavigator
          initialScene={{
            scene: arScenes['AR Sample'],
          }}
          apiKey={apiKey} />
        );
    } else {
      return (
        <ViroSceneNavigator
          initialScene={{
            scene: vrScenes['360 Photo Tour'],
          }}
          apiKey={apiKey} />
      );

    }
  }
});

AppRegistry.registerComponent('ViroCodeSamples', () => ViroCodeSamplesSceneNavigator);

// register the component again as ViroSample to enable use with the testbed app
AppRegistry.registerComponent('ViroSample', () => ViroCodeSamplesSceneNavigator);
