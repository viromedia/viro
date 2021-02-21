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
  ViroVRSceneNavigator,
  ViroARSceneNavigator,
} from 'react-viro';

var createReactClass = require('create-react-class');

/*
 * TODO: Add your API key below!!
 */
var apiKey = "YOUR_API_KEY_HERE";

var vrScenes = {
    '360PhotoTour': require('./js/360PhotoTour/MainScene'),
    'HumanBody': require('./js/HumanBody/MainScene'),
    'ProductShowcase': require('./js/ProductShowcase/ProductShowcase'),
    'ViroMediaPlayer': require('./js/ViroMediaPlayer/ViroTheatre'),
    'ParticleEmitters': require('./js/ParticleEmitters/ViroParticleTemplates'),
    'PhysicsSample': require('./js/PhysicsSample/BasicPhysicsSample'),
}

var arScenes = {
  'ARSimpleSample': require('./js/ARSample/HelloWorldSceneAR.js'),
  'ARPhysicsSample': require('./js/ARPhysicsSample/BasicPhysicsSample.js'),
  'ARCarDemo' : require('./js/ARCarDemo/ARCarDemo.js'),
  'ARPosterDemo' : require('./js/ARPosterDemo/ARPosterDemo.js'),
  'BusinessCard' : require('./js/ARBusinessCard/BusinessCard.js'),
}

var showARScene = true;

var ViroCodeSamplesSceneNavigator = createReactClass({
  render: function() {

    if (showARScene) {
      return (
          <ViroARSceneNavigator 
          initialScene={{ 
            scene: arScenes['ARSimpleSample'], 
        }} apiKey={apiKey} />
        );
    } else {
      return (
        <ViroVRSceneNavigator
          initialScene={{
            scene: vrScenes['360PhotoTour'],
          }}
          apiKey={apiKey} />
      );

    }
  }
});

// Uncomment the below line to use the ARDrivingCar Demo. Don't forget to set the apiKey variable in ARDrivingCar.js
// ViroCodeSamplesSceneNavigator = require('./js/ARDrivingCarDemo/ARDrivingCar');

module.exports = ViroCodeSamplesSceneNavigator;
