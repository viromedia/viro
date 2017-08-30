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
   ViroMaterials,
   ViroNode,
   ViroCamera,
   ViroImage,
   ViroUtils,
   ViroText,
   ViroAnimations,
   ViroAnimatedComponent,
   ViroSurface,
   ViroSphere,
   Viro3DObject,
   ViroQuadEmitter,
 } from 'react-viro';

 let polarToCartesian = ViroUtils.polarToCartesian;

 // **This test has not been done.  This is placeholder for scene and navigation arrows**

 var ReleaseMenu = require("./ReleaseMenu.js");

 var ViroParticleTest = React.createClass({
   getInitialState() {
     return {
       apply:true,
       num:3
     };
   },

   getSnow(){
     var views = [];
       views.push((
         <ViroQuadEmitter
             position={[0, 23, 0]}
             duration={12000}
             visible={true}
             delay={0}
             run={true}
             loop={true}
             fixedToEmitter={true}

             quad={{
                    source:require("./res/cloud.png"),                 // Image source of the quad particle.
                    height:0.5,
+                   width:0.5,
             }}

             spawnModifier={{
               particleLifetime:[5000,5000],
               emissionRatePerSecond:[70, 300],
-              spawnVolume:{shape:"box", params:[60, 1, 60], spawnOnSurface:false},
               maxParticles:2000
             }}

             appearanceModifier={{
               opacity:{min:0, max:0, factor:"time",
                 modifier:[
                   {finalValue:1.0, interval:[0,500]},
                   {finalValue:0.0, interval:[3500,5000]}
                 ]
               },
               rotation:{min:0, max:360, factor:"time",
                 modifier:[
                   {finalValue:720, interval:[0,4000]},
                 ]
               },
               scale:{min:[1,1,1], max:[5,5,5], factor:"time",
                 modifier:[
                   {finalValue:[2,2,2], interval:[0,1000]},
                   {finalValue:[1,1,1], interval:[3000,4000]}
                 ]
               },

             }}

             physicsModifier={{
               velocity:{min:[-10,-8,0], max:[10,-10,0]},
               acceleration:{min:[0,-4.81,0], max:[0,-4.81,0]}
             }}
           />
       ));
     return views;
   },

   getCircleFireworks(){
     var views = [];
     var colors =["#ff0000","#00ff00","#0000ff","#ffff00","#ff00e9","#00ff1d","#0000ff","#ff0000"];
     var position=[[0.5, 0.5, -1],
                   [1.2, 0.8, -1],
                   [1.8, 0.5, -1],
                   [0.8, 0.7, -1.3],
                   [1.3, 0.6, -0.8]];
     for (var i = 0; i < position.length; i ++){
     var randomCoolDown = Math.floor((Math.random() * 5000) + 3000);
     var randomInterval = Math.floor((Math.random() * 4000) + 2000);

       views.push((
         <ViroQuadEmitter
             position={position[i]}
             duration={12000}
             visible={true}
             delay={((i+1) *1000)}
             run={true}
             loop={true}
             fixedToEmitter={true}

             quad={{
                    source:require("./res/cloud.png"),                 // Image source of the quad particle.
                    height:0.01,
                    width:0.01,
                    bloomThreshold:1.0
             }}

             spawnModifier={{
               particleLifetime:[2000,2000],
               emissionRatePerSecond:[0,0],
               emissionBurst:[
                 {time:500, min:200, max:200, cycles:10, cooldownPeriod:randomCoolDown}
               ],
               spawnVolume:{shape:"sphere", params:[0.15, 0.25, 0], spawnOnSurface:true},
               maxParticles:2000
             }}

             appearanceModifier={{
               opacity:{min:0.5, max:0.5, factor:"time",
                 modifier:[
                   {finalValue:0.1, interval:[0,1000]},
                   {finalValue:0.0, interval:[1000,1100]}
                 ]
               },
               color:{min:0xff0000, max:"#ff0000", factor:"time",
                 modifier:[
                   {finalValue:colors[i], interval:[0,500]},
                   {finalValue:colors[6-i], interval:[700,1400]}
                 ]
               }
             }}

             physicsModifier={{
               initialExplosiveImpulse:{impulse:0.1, position:[0,0,0], deccelerationPeriod:1.0},
             }}
           />
       ));
     }
     return views;
   },

   // Ensure that fireworks continue spawning all around user.
   // This just spawns the fireworks in front of the camera facing
   // the z direction.
   getFireWorks(){
     var views = [];
     var colors =["#ff0000","#00ff00","#0000ff","#ffff00","#ff00e9","#00ff1d","#0000ff","#ff0000"];
     var position=[[0, 0.5, -1],
                   [1, 0.8, -1],
                   [2, 0.5, -1],
                   [1.75, 0.4, .75],
                   [0.75, 0.4, .75],
                   [0.5, 0.3, .5],
                   [1.5, 0.3, .5]];
     for (var i = 0; i < position.length; i ++){
     var randomCoolDown = Math.floor((Math.random() * 4000) + 3000);
     var randomInterval = Math.floor((Math.random() * 700) + 500);
       views.push((
         <ViroQuadEmitter
             position={position[i]}
             duration={12000}
             visible={true}
             delay={((i+1) *1000)}
             run={true}
             loop={true}
             fixedToEmitter={true}

             quad={{
                    source:require("./res/cloud.png"),                 // Image source of the quad particle.
                    height:0.01,
                    width:0.01,
                    bloomThreshold:1.0
             }}

             spawnModifier={{
               particleLifetime:[1800,2000],
               emissionRatePerSecond:[0,0],
               emissionBurst:[
                 {time:500, min:400, max:400, cycles:10, cooldownPeriod:randomCoolDown}
               ],
               spawnVolume:{shape:"sphere", params:[0.15], spawnOnSurface:true},
               maxParticles:2000
             }}

             appearanceModifier={{
               opacity:{min:1.0, max:1.0, factor:"time",
                 modifier:[
                   {finalValue:1.0, interval:[0,1500]},
                   {finalValue:0.0, interval:[1500,2000]}
                 ]
               },
               scale:{min:[1,1,1], max:[1,1,1], factor:"time",
                 modifier:[
                   {finalValue:[1,1,1], interval:[0,1000]},
                   {finalValue:[1,1,1], interval:[1000,2000]}
                 ]
               },
               color:{min:0xff0000, max:"#ff0000", factor:"time",
                 modifier:[
                   {finalValue:colors[i], interval:[0,500]},
                   {finalValue:colors[6-i], interval:[700,1400]},
                   {finalValue:"#0ff0ff", interval:[1700,2000]}
                 ]
               }
             }}

             physicsModifier={{
               initialExplosiveImpulse:{impulse:0.1, position:[0,0,0], deccelerationPeriod:1.0},
             }}
           />
       ));
     }
     return views;
   },

   onClicky(){
     var finalNum = this.state.num + 3;
     if (finalNum>40){
       finalNum = 3;
     }
      //this.setState({apply:!this.state.apply});
       this.setState({apply:!this.state.apply, num:finalNum});
   },
   render: function() {
     return (
      <ViroScene >
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
      <ViroText style={styles.baseTextTwo}  position={[0,-1.5, -3]} width={4} height ={4}
             text={"Toggle " + this.state.apply}
             onClick={this.onClicky}/>

             <ViroNode position={[0,0,0]}>
             {this.getCircleFireworks()}
             {this.getFireWorks()}
             </ViroNode>
      </ViroScene>
     );
   },
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

 module.exports = ViroParticleTest;
