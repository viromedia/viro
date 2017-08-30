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

   // Ensure that fireworks continue spawning all around user.
   // This just spawns the fireworks in front of the camera facing
   // the z direction.
  export function getFireWorks() {
     var views = [];
     for (var i = -1; i < 2; i ++){
       views.push((
         <ViroQuadEmitter key={"firework" + i}
             position={[i * 3, 1, -5]}
             duration={12000}
             visible={true}
             delay={((i+1) *1000)}
             run={true}
             loop={true}
             fixedToEmitter={true}

             quad={{
                    source:require("../res/cloud.png"),                 // Image source of the quad particle.
                    height:0.1,
                    width:0.1,
             }}

             spawnModifier={{
               particleLifetime:[2000,2000],
               emissionRatePerSecond:[0,0],
               emissionBurst:[
                 {time:500, min:100, max:100, cycles:10, cooldownPeriod:2350}
               ],
               spawnVolume:{shape:"sphere", params:[0.5], spawnOnSurface:true},
               maxParticles:500
             }}

             appearanceModifier={{
               opacity:{min:1.0, max:1.0, factor:"time",
                 modifier:[
                   {finalValue:1.0, interval:[0,1500]},
                   {finalValue:1.0, interval:[1500,2000]}
                 ]
               },
               rotation:{min:0, max:0, factor:"time",
                 modifier:[
                   {finalValue:360, interval:[0,2000]},
                 ]
               },
               scale:{min:[1,1,1], max:[1,1,1], factor:"time",
                 modifier:[
                   {finalValue:[3,3,3], interval:[0,1000]},
                   {finalValue:[1,1,1], interval:[1000,2000]}
                 ]
               },
               color:{min:0xff0000, max:"#ff0000", factor:"time",
                 modifier:[
                   {finalValue:"#ff0f00", interval:[0,500]},
                   {finalValue:"#ff00ff", interval:[500,1000]},
                   {finalValue:"#0ff0ff", interval:[1000,2000]}

                 ]
               }
             }}

             physicsModifier={{
               initialExplosiveImpulse:{impulse:5, position:[0,0,0]},
               velocity:{min:[0,10,0], max:[0,10,0]},
               acceleration:{min:[0,-9.81,0], max:[0,-9.81,0]}
             }}
           />
       ));
     }
     return views;
   }
