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
 ViroLightingEnvironment,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;
var createReactClass = require('create-react-class');
var ReleaseMenu = require("./ReleaseMenu.js");
var ViroPbrTestSurfaces = createReactClass({
 getInitialState() {
   return {
      refreshPass:true,
      envLight:true,
      matMap:"none"
   };
 },

 getEnvLight(){
   if (this.state.envLight){
     return ((<ViroLightingEnvironment source={require('./res/ibl_newport_loft.hdr')}/>));
   }
 },

/*

*/
 render: function() {
   var mat = "none";
   if (!this.state.refreshPass){
     mat = "sphere";
   }

   return (
    <ViroScene>
       {this.getEnvLight()}
       <Viro360Image source={require('./res/ibl_newport_loft.hdr')} isHdr={true} />
       <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
       <ViroNode position={[0,-2, 0]}>
       <ViroImage source={require('./res/poi_dot.png')} position={[0, -4, -2]} transformBehaviors={["billboard"]} onClick={this._showNext} />
       <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[-1, 0, -2]} width={4} height ={2}
       text={"Toggle Environment light: " + this.state.envLight} onClick={this._toggleEnvLight}/>
              <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[1, 0, -2]} width={4} height ={2}
              text={"Toggle Material Map: " + this.state.envLight} onClick={this._toggleMatMap}/>
              <ViroNode position={[0,0,-3 ]}>

              <Viro3DObject source={require('./res/cylinder_pbr.vrx')}
                            position={[0, 2, -2.0]}
                            rotation={[0, 0, 0]}
                            resources={[require('./res/blinn2_Base_Color.png'),
                                        require('./res/blinn2_Metallic.png'),
                                        require('./res/blinn2_Mixed_AO.png'),
                                        require('./res/blinn2_Normal_DirectX.png'),
                                        require('./res/blinn2_Roughness.png')
                                                  ]}
                            type="VRX" />


              <ViroSphere
                  position={[0, 0, 0]}
                  scale={[0.5, 0.5, 0.5]}
                  widthSegmentCount={15}
                  heightSegmentCount={15}
                  radius={1}
                  materials={[this.state.matMap]} />
              <ViroSphere
                  position={[1.5, 0, 0]}
                  scale={[0.5, 0.5, 0.5]}
                  widthSegmentCount={15}
                  heightSegmentCount={15}
                  radius={1}
                  materials={[this.state.matMap]} />
              <ViroSphere
                  position={[-1.5, 0, 0]}
                  scale={[0.5, 0.5, 0.5]}
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
     </ViroNode>
     </ViroScene>
   );
 },

 _showNext() {
   this.props.sceneNavigator.replace({scene:require('./ViroButtonTest')});
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
