/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

 /**
This class tests the following:
 - Looping animation using poidot Material
 - Animation color, animate color to yellow for ViroImage
 - Opacity animation, animate opacity to 0 when onTap event invoked
 - run flag, animate position on scene mount

 Scale, position and rotation are tested in the above animations. This test only tests AnimatedComponent
 with ViroImage elements. Another test will contain more animation tests with different components.
 */
'use strict';

import React, { Platform } from 'react';
import {
  StyleSheet,
  ViroScene,
  ViroOmniLight,
  ViroImage,
  Viro360Photo,
  ViroMaterials,
  ViroAnimations,
  ViroAnimatedComponent,
  ViroSkyBox,
  ViroQuad,
} from 'react-viro';

var createReactClass = require('create-react-class');

var ColorAnimationTest = createReactClass({
  getInitialState() {
    return {
       runAnimation: false,
    };
  },
  render: function() {
    return (
      <ViroScene >
        <ViroSkyBox color="#ffffff" />
        <ViroAnimatedComponent animation="testLoopOpacity" run={true} loop={true} >
          <ViroImage source={require('./res/card_main.png')} position={[0, .5, -1]} />
        </ViroAnimatedComponent>

        <ViroAnimatedComponent animation="testAnimateTexture" run={true} loop={true} >
          <ViroImage source={require('./res/card_main.png')} position={[0, -.5, -1]} />
        </ViroAnimatedComponent>
      </ViroScene>
    );
  },
});

ViroMaterials.createMaterials({
  sunTexture: {
    diffuseTexture: require("./res/sun_2302.jpg"),
  },
  redColor: {
    diffuseColor: "#ff0000"
  },
  blueColor: {
    diffuseColor: "#ff0000"
  },
  yellowColor: {
    diffuseColor: "#ffff0000"
  },
  cardpetite: {
    shininess : 1.0,
    lightingModel: "Constant",
    diffuseTexture: require('./res/card_petite_ansu.png'),
  },
  cardmain: {
    shininess : 1.0,
    lightingModel: "Constant",
    diffuseTexture: require('./res/card_main.png'),
  },
});

ViroAnimations.registerAnimations({
    cardIn:{properties:{scaleX:1, scaleY:.6, scaleZ:1, opacity: 1}, easing:"Bounce", duration: 5000},

    testLoopColorYellow:{properties:{material:"yellowColor"}, duration:3000},
    testLoopColorYellowMaterial:{properties:{material:"redColor"}, duration:3000},
    testLoopColorBlue:{properties:{material:"blueColor"}, duration:3000},
    testLoopColor:[
      ["testLoopColorYellow", "testLoopColorBlue"]
    ],
    testAnimateTexture:{properties:{material:"cardpetite"}, duration:3000},

    testMakeOpaque: {properties:{opacity:1.0}, duration: 1000},
    testMakeTransparent: {properties:{opacity:0}, duration: 1000},
    testLoopOpacity: [
      ["testMakeTransparent", "testMakeOpaque"]
    ]
});

module.exports = ColorAnimationTest;
