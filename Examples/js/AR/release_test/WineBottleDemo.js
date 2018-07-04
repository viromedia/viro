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
  ViroDirectionalLight,
  ViroAmbientLight,
  ViroARTrackingTargets,
  ViroARImageMarker,
  ViroVideo,
  ViroMaterials,
  Viro3DObject,
  ViroMaterialVideo,
  ViroNode,
} from 'react-viro';

var createReactClass = require('create-react-class');

var WineBottleDemo = createReactClass({
  getInitialState: function() {
    return {
      pauseVideo:true,
    };
  },
  render: function() {
    return (
      <ViroARScene physicsWorld={{gravity:[0, -10,0]}}>
        <ViroDirectionalLight color="#ffffff" direction={[0,-1,-.2]}/>
        <ViroAmbientLight color="#ffffff" intensity={200}/>
        <ViroARImageMarker target={"label"} onAnchorFound={this._onMarkerFound}>
        <ViroNode position={[0, 0, -.008]} rotation={[-90, 0, 0]} dragType="FixedToWorld" onDrag={()=>{}}>
          <Viro3DObject
            source={require('./res/wine/winebottle_label.obj')}
            materials="bottle"
            type="OBJ"
            scale={[.28,.28,.28]}
            />
          <ViroMaterialVideo material="bottle" paused={this.state.pauseVideo} loop={true} />
        </ViroNode>
        </ViroARImageMarker>
      </ViroARScene>
    );
  },
  _onMarkerFound(){
        this.setState({
            pauseVideo:false
        });
    },
});

ViroARTrackingTargets.createTargets({
  label : {
    source : require('./res/wine/19crimesTarget.jpg'),
    orientation : "Up",
    physicalWidth : 0.04 // real world width in meters
  },
});

ViroMaterials.createMaterials({
  bottle: {
    fresnelExponent: .5,
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/wine/19crimes.mp4"),
  },
});

module.exports = WineBottleDemo;
