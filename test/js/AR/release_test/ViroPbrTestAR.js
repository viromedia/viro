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
 ViroSphere,
 ViroARScene,
 ViroLightingEnvironment,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;
var createReactClass = require('create-react-class');
var ReleaseMenu = require("./ARReleaseMenu.js");
var ViroPbrTest = createReactClass({
 getInitialState() {
   return {
      roughness:0,
      metalness:0,
      refreshPass:true,
      envLight:true
   };
 },

 render: function() {
   var mat = "none";
   if (!this.state.refreshPass){
     mat = "sphere";
   }

   return (
     <ViroARScene position={[0,0,0]} reticleEnabled={false} >
       <ViroNode position={[0,-2, 0]}>
          <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[-2, -1, -2]} width={4} height ={2}
              text={"Toggle metalness: " + this.state.metalness} onClick={this._toggleMetalness}/>
          <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[1, -1, -2]} width={4} height ={2}
              text={"Toggle roughness: " + this.state.roughness} onClick={this._toggleRoughness}/>
              <ViroNode position={[0,0,-3 ]}>
              <ViroSphere
                  position={[0, 0, 0]}
                  scale={[0.5, 0.5, 0.5]}
                  widthSegmentCount={15}
                  heightSegmentCount={15}
                  radius={1}
                  materials={[mat]} />
              <ViroSphere
                  position={[1.5, 0, 0]}
                  scale={[0.5, 0.5, 0.5]}
                  widthSegmentCount={15}
                  heightSegmentCount={15}
                  radius={1}
                  materials={[mat]}
                  />
              <ViroSphere
                  position={[-1.5, 0, 0]}
                  scale={[0.5, 0.5, 0.5]}
                  widthSegmentCount={15}
                  heightSegmentCount={15}
                  radius={1}
                  materials={[mat]}
                  />

              <ViroOmniLight
                  intensity={300}
                  position={[-5, 5, 2]}
                  color={"#FFFFFF"}
                  attenuationStartDistance={0}
                  attenuationEndDistance={20} />
             <ViroOmniLight
                  intensity={300}
                  position={[5, -5, 2]}
                  color={"#FFFFFF"}
                  attenuationStartDistance={0}
                  attenuationEndDistance={20} />
            <ViroOmniLight
                  intensity={300}
                  position={[5, 5, 2]}
                  color={"#FFFFFF"}
                  attenuationStartDistance={0}
                  attenuationEndDistance={20} />
             <ViroOmniLight
                  intensity={300}
                  position={[-5, -5, 2]}
                  color={"#FFFFFF"}
                  attenuationStartDistance={0}
                  attenuationEndDistance={20} />
              </ViroNode>
     </ViroNode>


     {/* Release Menu */}
     <ViroText position={polarToCartesian([2, -30, -15])} text={"Release Menu"}
       style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
       transformBehaviors={["billboard"]}/>

     </ViroARScene>
   );
 },
 _toggleRoughness() {
   var rough = this.state.roughness + 0.025;
   if (rough > 1){
      rough = 0;
   }

   ViroMaterials.createMaterials({
     sphere: {
       roughness: rough,
       metalness: this.state.metalness,
       lightingModel: "PBR",
       diffuseColor: "#ff0000"
     }
   });

   this.setState({
      roughness: rough,
      refreshPass: true
   });

   let that = this;
   setTimeout(function(){
     that.setState({refreshPass:false});
   }, 200);

 },

  _toggleMetalness() {
    var metal = this.state.metalness + 0.025;
    if (metal > 1){
       metal = 0;
    }

    ViroMaterials.createMaterials({
      sphere: {
        roughness: this.state.roughness,
        metalness: metal,
        lightingModel: "PBR",
        diffuseColor: "#ff0000"
      }
    });

    this.setState({
       metalness:metal,
       refreshPass: true
    });

       let that = this;
       setTimeout(function(){
       that.setState({refreshPass:false});
       }, 200);

  },
});

ViroMaterials.createMaterials({
sphere: {
  roughness: 0,
  metalness: 0,
  lightingModel: "PBR",
  diffuseColor: "#ff0000"
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

module.exports = ViroPbrTest;
