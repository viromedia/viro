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
 ViroSphere,
 ViroLightingEnvironment,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;
var createReactClass = require('create-react-class');
var ReleaseMenu = require("./ReleaseMenu.js");
var ViroPbrTest = createReactClass({
 getInitialState() {
   return {
      roughness:0,
      metalness:0,
      refreshPass:true,
      envLight:true
   };
 },

 getEnvLight(){
   if (this.state.envLight){
     return    ((    <ViroLightingEnvironment source={require('./res/ibl_newport_loft.hdr')}/>));
   }
 },

 render: function() {
   var mat = "none";
   if (!this.state.refreshPass){
     mat = "sphere";
   }

   return (
    <ViroScene>
       <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
       <Viro360Image source={require('./res/ibl_newport_loft.hdr')} isHdr={true} />
       {this.getEnvLight()}
       <ViroNode position={[0,-2, 0]}>
       <ViroImage source={require('./res/poi_dot.png')} position={[0, -4, -2]} transformBehaviors={["billboard"]} onClick={this._showNext} />
          <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[-2, -1, -2]} width={4} height ={2}
              text={"Toggle metalness: " + this.state.metalness} onClick={this._toggleMetalness}/>
          <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[1, -1, -2]} width={4} height ={2}
              text={"Toggle roughness: " + this.state.roughness} onClick={this._toggleRoughness}/>
          <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[0, 0, -2]} width={4} height ={2}
              text={"Toggle Environment light: " + this.state.envLight} onClick={this._toggleEnvLight}/>
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
                  position={[-10, 10, 10]}
                  color={"#FFFFFF"}
                  attenuationStartDistance={0}
                  attenuationEndDistance={20} />
             <ViroOmniLight
                  intensity={300}
                  position={[10, -10, 10]}
                  color={"#FFFFFF"}
                  attenuationStartDistance={0}
                  attenuationEndDistance={20} />
            <ViroOmniLight
                  intensity={300}
                  position={[10, -10, 10]}
                  color={"#FFFFFF"}
                  attenuationStartDistance={0}
                  attenuationEndDistance={20} />
             <ViroOmniLight
                  intensity={300}
                  position={[-10, -10, 10]}
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
