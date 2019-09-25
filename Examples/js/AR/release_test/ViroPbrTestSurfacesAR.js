'use strict';

import React, { Component } from 'react';
import {
 AppRegistry,
 StyleSheet,
 Text,
 View
} from 'react-native';

import {
 ViroScene,
 ViroMaterials,
 ViroNode,
 ViroCamera,
 ViroAmbientLight,
 ViroOmniLight,
 ViroSpotLight,
 ViroDirectionalLight,
 ViroImage,
 Viro360Image,
 ViroFlexView,
 ViroUtils,
 ViroText,
 Viro3DObject,
 ViroSphere,
 ViroARScene,
 ViroLightingEnvironment,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;
var createReactClass = require('create-react-class');
var ReleaseMenu = require("./ARReleaseMenu.js");
var ViroPbrTestSurfaces = createReactClass({
 getInitialState() {
   return {
      refreshPass:true,
      envLight:true,
      matMap:"none"
   };
 },

/*

*/
 render: function() {
   var mat = "none";
   if (!this.state.refreshPass){
     mat = "sphere";
   }

   return (
     <ViroARScene position={[0,0,0]} reticleEnabled={false} >


       <ViroNode position={[0,-.5, -.5]}>
             <Viro3DObject source={require('./res/cylinder_pbr.vrx')}
                           scale={[0.10,0.10,0.10]}
                           position={[0,0,-1]}
                           rotation={[0, 0, 0]}
                           resources={[require('./res/blinn2_Base_Color.png'),
                                       require('./res/blinn2_Metallic.png'),
                                       require('./res/blinn2_Mixed_AO.png'),
                                       require('./res/blinn2_Normal_DirectX.png'),
                                       require('./res/blinn2_Roughness.png')
                                                 ]}
                           type="VRX" />

              <ViroNode position={[0.25, 0.3, 0]}>
              <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[1, 0, -2]} width={4} height ={2}
                    text={"Toggle Material Map: " + this.state.envLight} onClick={this._toggleMatMap}/>
              </ViroNode>
              <ViroSphere
                  position={[0, 0, 0]}
                  scale={[0.1, 0.1, 0.1]}
                  widthSegmentCount={15}
                  heightSegmentCount={15}
                  radius={1}
                  materials={[this.state.matMap]} />
              <ViroSphere
                  position={[0.25, 0, 0]}
                  scale={[0.1, 0.1, 0.1]}
                  widthSegmentCount={15}
                  heightSegmentCount={15}
                  radius={1}
                  materials={[this.state.matMap]} />
              <ViroSphere
                  position={[-.25, 0, 0]}
                  scale={[0.1, 0.1, 0.1]}
                  widthSegmentCount={15}
                  heightSegmentCount={15}
                  radius={1}
                  materials={[this.state.matMap]} />
              <ViroOmniLight
                  intensity={300}
                  position={[-5, 5, 1]}
                  color={"#FFFFFF"}
                  attenuationStartDistance={0}
                  attenuationEndDistance={20} />
             <ViroOmniLight
                  intensity={300}
                  position={[5, -5, 1]}
                  color={"#FFFFFF"}
                  attenuationStartDistance={0}
                  attenuationEndDistance={20} />
            <ViroOmniLight
                  intensity={300}
                  position={[5, -5, 1]}
                  color={"#FFFFFF"}
                  attenuationStartDistance={0}
                  attenuationEndDistance={20} />
             <ViroOmniLight
                  intensity={300}
                  position={[-5, -5, 1]}
                  color={"#FFFFFF"}
                  attenuationStartDistance={0}
                  attenuationEndDistance={20} />
     </ViroNode>

     {/* Release Menu */}
     <ViroText position={polarToCartesian([2, -30, -15])} text={"Release Menu"}
       style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
       transformBehaviors={["billboard"]}/>

     </ViroARScene>
   );
 },

 _toggleEnvLight(){
   this.setState({
      envLight: !this.state.envLight
   });
 },
 _toggleMatMap(){
   var finalMap = this.state.matMap == "sphere1" ? "sphere2" : "sphere1";

   this.setState({
      matMap: finalMap
   });
 },
});
//  ambientOcclusion: require('../res/heart_d.jpg'),

ViroMaterials.createMaterials({
  sphere1: {
    lightingModel: "PBR",
    diffuseTexture:  require('./res/iron-rusted4-basecolor.png'),
    metalnessTexture: require('./res/iron-rusted4-metalness.png'),
    roughnessTexture: require('./res/iron-rusted4-roughness.png'),
    normalTexture: require('./res/iron-rusted4-normal.png')
  },
  sphere2: {
    lightingModel: "PBR",
    diffuseTexture:  require('./res/bamboo-wood-semigloss-albedo.png'),
    metalnessTexture: require('./res/bamboo-wood-semigloss-metal.png'),
    roughnessTexture: require('./res/bamboo-wood-semigloss-roughness.png'),
    normalTexture: require('./res/bamboo-wood-semigloss-normal.png'),
    ambientOcclusionTexture: require('./res/bamboo-wood-semigloss-ao.png'),
  },
none:{
  roughness: 0,
}
});

var styles = StyleSheet.create({
container: {
  flex: 1,
  justifyContent: 'center',
  alignItems: 'center',
},
elementText: {
  fontFamily: 'HelveticaNeue-Medium',
  fontSize: 30,
  color: '#ffffff',
  textAlign: 'center',
},
baseTextTwo: {
    fontFamily: 'Arial',
    fontSize: 20,
    color: '#ffffff',
    flex: 1,
    textAlignVertical: 'center',
    textAlign: 'center',
},
});

module.exports = ViroPbrTestSurfaces;
