/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 */
 'use strict';

 import React, { Component } from 'react';
 import {
   AppRegistry,
   View
 } from 'react-native';

 import {
   ViroImage,
   ViroQuadEmitter,
 } from 'react-viro';

  export function getEmptyEffect() {
    var views = [];
    return views;
  }

  export function getGrayScale() {
    var views = [];
    return views;
  }

  export function getSepia() {
    var views = [];
    return views;
  }

  export function getThermal() {
    var views = [];
    return views;
  }

  export function getSinCity() {
    var views = [];
    return views;
  }

  export function getBarrel() {
    var views = [];
    return views;
  }

  export function getPinCushion() {
    var views = [];
    return views;
  }

  export function getSnow() {
     var views = [];
       views.push((
         <ViroQuadEmitter key="snow_effect"
             position={[0, 23, 0]}
             duration={12000}
             visible={true}
             delay={0}
             run={true}
             loop={true}
             fixedToEmitter={true}

             quad={{
                    source:require("../res/cloud.png"),                 // Image source of the quad particle.
                    height:0.5,
                    width:0.5,
             }}

             spawnModifier={{
               particleLifetime:[5000,5000],
               emissionRatePerSecond:[70, 300],
               spawnVolume:{shape:"box", params:[60, 1, 60], spawnOnSurface:false},
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
   }

  export function getFireWorks(){
     var views = [];
     var colors =["#ff0000","#00ff00","#0000ff","#ffff00","#ff00e9","#00ff1d","#0000ff","#ff0000"];
     var position=[[0, 0.3, -1],
                   [-1, 0.5, -1],
                   [2, 0.2, -1],
                   [1.75, 0.1, .75],
                   [-0.75, 0.1, .75],
                   [-0.5, 0.0, .5],
                   [1.5, 0.0, .5]];
     for (var i = 0; i < position.length; i ++){
     var randomCoolDown = Math.floor((Math.random() * 1000) + 1000);
     var randomInterval = Math.floor((Math.random() * 500) + 500);
       views.push((
         <ViroQuadEmitter
             position={position[i]}
             duration={12000}
             visible={true}
             delay={((i+1) * 400)}
             run={true}
             loop={true}
             fixedToEmitter={true}

             quad={{
                    source:require("../res/cloud.png"),                 // Image source of the quad particle.
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
                   {finalValue:0.0, interval:[1500,1700]}
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
   }
