'use strict';

import React, { Component } from 'react';

import {StyleSheet} from 'react-native';

import {
  ViroScene,
  ViroText,
  Viro360Image,
  ViroSphere,
  ViroMaterials,
  Viro3DObject,
  ViroSpotLight,
  ViroSkybox,
  ViroCamera,
  ViroAmbientLight,
  ViroNode,
  ViroImage,
  ViroAnimations,
  ViroFlexView,
  ViroAnimatedComponent
} from 'react-viro';

var createReactClass = require('create-react-class');

var HelloWorldScene = createReactClass({
  getInitialState() {
    return {

    };
  },
  render: function() {
    return (
      <ViroScene style={styles.container}>
        <Viro360Image source={require('./res/textures/360_space.jpg')} />
        <ViroCamera position={[0, 5, 25]} active={true} />
        <ViroAmbientLight color="#ffffff" />

        <ViroNode position={[0, 0, 0]}>
          <ViroFlexView style={{flexDirection: 'column', padding: .1, justifyContent: 'center'}}
                  width={1.8} height={1.0}
                  position={[7, 4, 0.0]}
                  materials={["label_flex_view"]}
                  transformBehaviors={["billboard"]}>
            <ViroText text="Sun" style={styles.textStyle} />
          </ViroFlexView>

          <Viro3DObject source={require('./res/3d_objects/orbit_ring_a.obj')}
              position={[0, 0, 0]}
              type="OBJ"
              materials={["earth_orbit"]} />
          <Viro3DObject source={require('./res/3d_objects/sun.obj')}
              position={[0, 0, 0]}
              type="OBJ"
              materials={["sun"]} />
          <Viro3DObject source={require('./res/3d_objects/e_sun_glow.obj')}
              position={[0, 0, 0]}
              type="OBJ"
              materials={["sun_glow"]}
              rotation={[90,0,0]}
              transformBehaviors={["billboard"]} />

          <ViroAnimatedComponent animation='loopRevolveAroundSun'
              run={true} loop={true}>

            <ViroNode position={[0, 0, 0]} >

              <ViroFlexView style={{flexDirection: 'column', padding: .1}}
                  width={1.0} height={1.0}
                  position={[-11.6, 1.5, 0.0]}
                  materials={["label_flex_view"]}
                  transformBehaviors={["billboard"]}>
              </ViroFlexView>

              <ViroAnimatedComponent animation='loopRotateEarth'
                                         run={true} loop={true}>
                <Viro3DObject source={require('./res/3d_objects/earth.obj')}
                                position={[-11.6, 0, 0]}
                                scale={[2.5, 2.5, 2.5]}
                                type="OBJ"
                                materials={["earth"]} />
              </ViroAnimatedComponent>

              <ViroAnimatedComponent animation='loopRevolveAroundEarth'
                           run={true} loop={true}>
                <ViroNode position={[-11.6, 0, 0]} >
                  <ViroNode position={[0.85, 0, 0]} >
                    <Viro3DObject source={require('./res/3d_objects/moon.obj')}
                                    scale={[2.5, 2.5, 2.5]}
                                    type="OBJ"
                                    materials={["moon"]} />
                  </ViroNode>
                </ViroNode>
              </ViroAnimatedComponent>
            </ViroNode>
          </ViroAnimatedComponent>
        </ViroNode>
      </ViroScene>
    );
  },
});

ViroAnimations.registerAnimations({
  loopRotateEarth:{properties:{rotateY:"+=45"}, duration:200},
  loopRevolveAroundEarth:{properties:{rotateY:"+=45"}, duration:200*27.32*45/365},
  loopRevolveAroundSun:{properties:{rotateY:"+=45"}, duration:200*365*45/365},
  loopNegativeRevolveAroundSun:{properties:{rotateY:"-=45"}, duration:200*365*45/365},
});

ViroMaterials.createMaterials({
   sun: {
     lightingModel: "Constant",
     diffuseTexture: require('./res/textures/d_sun_a.jpg'),
   },
   sun_glow: {
     lightingModel: "Constant",
     diffuseTexture: require('./res/textures/sun_glow.png'),
     writesToDepthBuffer: true,
     readsFromDepthBuffer: true
   },

   earth: {
     lightingModel: "Constant",
     diffuseTexture: require('./res/textures/d_earth.jpg'),
   },
   earth_glow: {
     lightingModel: "Constant",
     diffuseTexture: require('./res/textures/earth_glow.png'),
   },
   moon: {
     lightingModel: "Constant",
     diffuseTexture: require('./res/textures/d_moon.jpg'),
   },
   earth_orbit: {
     lightingModel: "Constant",
     diffuseTexture: require('./res/textures/d_orbit_earth.jpg'),
   },
   label_flex_view: {
     lightingModel: "Constant",
     diffuseTexture: require('./res/textures/text_box.png'),
   }
 });

var styles = StyleSheet.create({
  textStyle: {
    fontFamily: 'Avenir',
    fontSize: 200,
    color: '#ffffff',
  },
});

module.exports = HelloWorldScene;
