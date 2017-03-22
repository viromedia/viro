'use strict';

import React, { Component } from 'react';
import {StyleSheet} from 'react-native';

import {
  ViroSpotLight,
  ViroDirectionalLight,
  ViroAmbientLight,
  ViroOrbitCamera,
  ViroScene,
  Viro3DObject,
  ViroText,
  ViroSkyBox,
  ViroNode,
  ViroMaterials,
} from 'react-viro';

var MainScene = React.createClass({
  getInitialState() {
    return {

    };
  },

  render: function() {
    return (
     <ViroScene style={styles.container}>
        <ViroSkyBox source={{nx:require('./res/grid_bg.jpg'),
                             px:require('./res/grid_bg.jpg'),
                             ny:require('./res/grid_bg.jpg'),
                             py:require('./res/grid_bg.jpg'),
                             nz:require('./res/grid_bg.jpg'),
                             pz:require('./res/grid_bg.jpg')}} />
        <ViroOrbitCamera position={[0, 0, -0]} active={true} focalPoint={[0, 0, -1]} />
        <ViroDirectionalLight direction={[0, 0, -1]} color="#ffffff" />

        <ViroAmbientLight color="#aaaaaa" />

         <ViroNode position={[0, 0, -1]} >
            <Viro3DObject source={require('./res/heart.obj')}
                       materials={["heart"]} />
       </ViroNode>
       <ViroText text="Heart" position={[0.0, 0.0, -1]} style={styles.textStyle}
                 transformBehaviors={["billboardY"]}/>
     </ViroScene>
    );
  },
});

var materials = ViroMaterials.createMaterials({
   heart: {
     lightingModel: "Blinn",
     diffuseTexture: require('./res/Heart_D3.jpg'),
     specularTexture: require('./res/Heart_S2.jpg'),
     writesToDepthBuffer: true,
     readsFromDepthBuffer: true,
   },
});

var styles = StyleSheet.create({
  textStyle: {
    fontSize: 10,
    color: '#FFFFFF',
  },
});

module.exports = MainScene;
