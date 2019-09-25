'use strict';

import React, { Component } from 'react';
import {
 ViroScene,
 ViroMaterials,
 ViroNode,
 ViroOmniLight,
 Viro360Image,
 ViroSphere,
 ViroController,
 ViroLightingEnvironment,
} from 'react-viro';

var createReactClass = require('create-react-class');
var ViroPbrBallTest = createReactClass({
  getInitialState() {
    return {

    };
  },

  render: function() {
    return (

    <ViroScene>
      <Viro360Image source={require('./res/ibl_mans_outside.hdr')} isHdr={true} />
      <ViroLightingEnvironment source={require('./res/ibl_mans_outside.hdr')}/>

      <ViroOmniLight
          intensity={300}
          position={[-10, 10, 1]}
          color={"#FFFFFF"}
          attenuationStartDistance={20}
          attenuationEndDistance={30} />
      <ViroOmniLight
          intensity={300}
          position={[10, 10, 1]}
          color={"#FFFFFF"}
          attenuationStartDistance={20}
          attenuationEndDistance={30} />
      <ViroOmniLight
          intensity={300}
          position={[-10, -10, 1]}
          color={"#FFFFFF"}
          attenuationStartDistance={20}
          attenuationEndDistance={30} />
      <ViroOmniLight
          intensity={300}
          position={[10, -10, 1]}
          color={"#FFFFFF"}
          attenuationStartDistance={20}
          attenuationEndDistance={30} />

      <ViroSphere
          position={[-3, 1.5, -9]}
          radius={1}
          materials={"sphereA"} />
      <ViroSphere
          position={[0, 1.5, -9]}
          radius={1}
          materials={"sphereB"}/>
      <ViroSphere
          position={[3, 1.5, -9]}
          radius={1}
          materials={"sphereC"}/>
      <ViroSphere
          position={[-3, -1.5, -9]}
          radius={1}
          materials={"sphereD"} />
      <ViroSphere
          position={[0, -1.5, -9]}
          radius={1}
          materials={"sphereE"}/>
      <ViroSphere
          position={[3, -1.5, -9]}
          radius={1}
          materials={"sphereF"}/>
    </ViroScene>
  );
  },
 });

ViroMaterials.createMaterials({
  sphereA: {
    roughness: 0.0,
    metalness: 1.0,
    lightingModel: "PBR",
    diffuseColor: "#FFFFFF"
  },
  sphereB: {
    roughness: 0.2,
    metalness: 1.0,
    lightingModel: "PBR",
    diffuseColor: "#FFFFFF"
  },
  sphereC: {
    roughness: 0.5,
    metalness: 1.0,
    lightingModel: "PBR",
    diffuseColor: "#FFFFFF"
  },
  sphereD: {
    roughness: 0.0,
    metalness: 0.3,
    lightingModel: "PBR",
    diffuseColor: "#FFFFFF"
  },
  sphereE: {
    roughness: 0.2,
    metalness: 0.3,
    lightingModel: "PBR",
    diffuseColor: "#FFFFFF"
  },
  sphereF: {
    roughness: 0.5,
    metalness: 0.3,
    lightingModel: "PBR",
    diffuseColor: "#FFFFFF"
  },
});

module.exports = ViroPbrBallTest;
