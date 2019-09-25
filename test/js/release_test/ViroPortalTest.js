/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 */
 'use strict';

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
   ViroARScene,
   ViroBox,
   ViroMaterials,
   ViroNode,
   ViroOrbitCamera,
   ViroCamera,
   ViroAmbientLight,
   ViroOmniLight,
   ViroSpotLight,
   ViroDirectionalLight,
   ViroImage,
   ViroVideo,
   Viro360Image,
   Viro360Video,
   ViroFlexView,
   ViroUtils,
   ViroText,
   ViroAnimations,
   ViroQuad,
   ViroSkyBox,
   ViroPortalScene,
   ViroPortal,
   ViroSphere,
   Viro3DObject,
 } from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;
var Uri360Image = require("../res/sun_2302.jpg");

var ViroPortalTest = createReactClass({
   getInitialState() {
     return {

     };
   },

   render: function() {
     var image = Uri360Image;
      return (
        <ViroARScene>
            <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>
            <ViroPortalScene position={[0, 0, 0]} scale={[1, 1, 1]}>
               <ViroPortal>
                 <Viro3DObject source={require('./res/portal_ring.obj')}
                               position={[0, 0, -2]}
                               rotation={[0, 0, 0]}
                               scale={[0.1, 0.1, 0.1]}
                               materials={["ring"]}
                               type="OBJ"
                 />
               </ViroPortal>

               <ViroSkyBox color="#098765" />

                <ViroBox width={0.25} height={0.25} position={[-1.5, 0, -2]} animation={{name:"parallelAnim", loop:true, run:true}} />
            </ViroPortalScene>
        </ViroARScene>
      );
   },
 });

var styles = StyleSheet.create({
  baseTextTwo: {
      fontFamily: 'Arial',
      fontSize: 10,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});

ViroMaterials.createMaterials({
  ring: {
    lightingModel: "Lambert",
    diffuseTexture: require('./res/portal_ring_d.png'),
  },
});

ViroAnimations.registerAnimations({
  moveRight:{properties:{positionX:"+=3",}, duration:1000, delay:0},
  moveLeft:{properties:{positionX:"-=3",}, duration:1000, delay:0},
  loopRotate:{properties:{rotateY:"+=180"}, duration:500},
  parallelAnim:[
        ["moveRight","moveLeft"],
        ["loopRotate"]
  ],
});

module.exports = ViroPortalTest;
