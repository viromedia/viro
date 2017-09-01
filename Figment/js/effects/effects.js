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
   ViroUtils,
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

  export function getSnow(){
    var views = [];
      views.push((
        <ViroQuadEmitter
            position={[0, 4.5, 0]}
            duration={2000}
            visible={true}
            delay={0}
            run={true}
            loop={true}
            fixedToEmitter={true}

            quad={{
                   source:require("../res/particle_snow.png"),                 // Image source of the quad particle.
                   height:0.01,
                   width:0.01,
                   bloomThreshold:1.0
            }}

            spawnModifier={{
              particleLifetime:[5000,5000],
              emissionRatePerSecond:[800, 800], // or 300 with a max of 2000
              spawnVolume:{shape:"box", params:[20, 1, 20], spawnOnSurface:false},
              maxParticles:4000
            }}

            appearanceModifier={{
              opacity:{min:0, max:0, factor:"time",
                modifier:[
                  {finalValue:1.0, interval:[0,500]},
                  {finalValue:0.0, interval:[4000,5000]}
                ]
              },
              rotation:{min:0, max:360, factor:"time",
                modifier:[
                  {finalValue:1080, interval:[0,5000]},
                ]
              },
              scale:{min:[5,5,5], max:[10,10,10], factor:"time",
                modifier:[
                  {finalValue:[6,6,6], interval:[0,1000]},
                  {finalValue:[10,10,10], interval:[3000,5000]},
                  {finalValue:[5,5,5], interval:[4000,5000]}
                ]
              },

            }}

            physicsModifier={{
              velocity:{min:[-2,-.5,0], max:[2,-3.5,0]}
            }}
          />
      ));
    return views;
  }
  export function getBubbles(){
    var views = [];
      views.push((
        <ViroQuadEmitter
            position={[0, -5.0, 0]}
            duration={5000}
            visible={true}
            delay={0}
            run={true}
            loop={true}
            fixedToEmitter={true}

            quad={{
                   source:require("../res/particle_bubble.png"),                 // Image source of the quad particle.
                   height:0.1,
                   width:0.1,
                   bloomThreshold:1.0
            }}

            spawnModifier={{
              particleLifetime:[14000,14000],
              emissionRatePerSecond:[80, 150], // or 300 with a max of 2000
              spawnVolume:{shape:"box", params:[15, 1, 15], spawnOnSurface:false},
              maxParticles:2000
            }}

            appearanceModifier={{
              opacity:{min:0.0, max:0.0, factor:"time",
                modifier:[
                  {finalValue:1.0, interval:[0,500]},
                  {finalValue:0.0, interval:[13700,14000]}
                ]
              },
              scale:{min:[1,1,1], max:[1,1,1], factor:"time",
                modifier:[
                  {finalValue:[1.5,1.5,1.5], interval:[4000,9700]},
                  {finalValue:[3,3,3], interval:[13700,14000]}
                ]
              },

            }}

            physicsModifier={{
              velocity:{min:[-.1,.7,0], max:[.1,.95,0]}
            }}
          />
      ));
    return views;
  }

  export function getFireWorks(){
    var views = [];
    var radius = 3;
    let polarToCartesian = ViroUtils.polarToCartesian;

    var colors =["#ff2d2d","#42ff42","#003fff","#ffff00","#ffb5f8","#00ff1d","#00edff","#ffb14c", "#ff7cf4"];
    var f = new Array(30);
    f[1] = getFireWork(polarToCartesian([radius, -90 , 30]), colors[1], 1000);
    f[2] = getFireWork(polarToCartesian([radius, -70 , 20]), colors[1], 2000);
    f[3] = getFireWork(polarToCartesian([radius, -50 , 40]), colors[2], 3000);
    f[4] = getFireWork(polarToCartesian([radius, -30 , 28]), colors[4], 4000);
    f[5] = getFireWork(polarToCartesian([radius, -10 , 25]), colors[5], 4500);
    f[6] = getFireWork(polarToCartesian([radius, 10 , 20]), colors[7], 5000);
    f[7] = getFireWork(polarToCartesian([radius, 30 , 40]), colors[7], 4000);
    f[8] = getFireWork(polarToCartesian([radius, 50 , 35]), colors[2], 3000);
    f[9] = getFireWork(polarToCartesian([radius, 70 , 25]), colors[5], 2000);
    f[10] = getFireWork(polarToCartesian([radius, 80 , 10]), colors[5], 1000);

    f[11] = getFireWorkGravity(polarToCartesian([radius, -85 , 20]), colors[7], 3000);
    f[12] = getFireWorkGravity(polarToCartesian([radius, -60 , 45]), colors[0], 1500);
    f[13] = getFireWorkGravity(polarToCartesian([radius, -40 , 40]), colors[3], 2500);
    f[14] = getFireWorkGravity(polarToCartesian([radius, -20 , 25]), colors[5], 1500);
    f[15] = getFireWorkGravity(polarToCartesian([radius, -5 , 30]), colors[6], 500);
    f[29] = getFireWorkGravity(polarToCartesian([radius, 5 , 25]), colors[6], 4500);
    f[16] = getFireWorkGravity(polarToCartesian([radius, 20 , 20]), colors[3], 1500);
    f[17] = getFireWorkGravity(polarToCartesian([radius, 40 , 40]), colors[3], 2500);
    f[18] = getFireWorkGravity(polarToCartesian([radius, 60 , 25]), colors[3], 1500);
    f[19] = getFireWorkGravity(polarToCartesian([radius, 85 , 36]), colors[3], 3500);

    f[20] = getFireWorkBrightDarkBright(polarToCartesian([radius, -80 , 30]), colors[0], colors[3], 2340);
    f[21] = getFireWorkBrightDarkBright(polarToCartesian([radius, -50 , 40]), colors[1], colors[4], 0);
    f[22] = getFireWorkBrightDarkBright(polarToCartesian([radius, -25 , 20]), colors[2], colors[5], 800);
    f[23] = getFireWorkBrightDarkBright(polarToCartesian([radius, -15 , 33]), colors[3], colors[7], 2000);
    f[24] = getFireWorkBrightDarkBright(polarToCartesian([radius, 8 , 40]), colors[4], colors[6], 1900);
    f[25] = getFireWorkBrightDarkBright(polarToCartesian([radius, 15 , 20]), colors[4], colors[6], 3700);
    f[26] = getFireWorkBrightDarkBright(polarToCartesian([radius, 35 , 33]), colors[4], colors[6], 200);
    f[27] = getFireWorkBrightDarkBright(polarToCartesian([radius, 65 , 40]), colors[4], colors[6], 3720);
    f[28] = getFireWorkBrightDarkBright(polarToCartesian([radius, 90 , 25]), colors[4], colors[6], 1020);

    var i =0;
    for ( i =0; i < f.length ; i ++){
      views.push(f[i]);
    }
    return views;
  }

  function getFireWork(position, color, delay){
     var viroFireworkColors =["#ff2d2d","#42ff42","#00edff","#ffff00","#ffb5f8","#00ff1d","#00edff","#ffb14c", "#ff7cf4"];
     var colorRand = viroFireworkColors[Math.floor((Math.random() * 5) + 0)];
       return((
         <ViroQuadEmitter
             position={position}
             duration={5000}
             visible={true}
             run={true}
             loop={true}
             fixedToEmitter={true}

             quad={{
                    source:require("../res/particle_firework.png"),                 // Image source of the quad particle.
                    height:0.01,
                    width:0.01,
                    bloomThreshold:0.0
             }}

             spawnModifier={{
               particleLifetime:[1400,1500],
               emissionRatePerSecond:[0,0],
               emissionBurst:[
                 {time:delay, min:500, max:500, cycles:10, cooldownPeriod:1200}
               ],
               spawnVolume:{shape:"sphere", params:[0.15], spawnOnSurface:true},
               maxParticles:800
             }}

             appearanceModifier={{
               opacity:{min:1.0, max:1.0, factor:"time",
                 modifier:[
                   {finalValue:0.5, interval:[0,1000]},
                   {finalValue:0.0, interval:[1000,1500]}
                 ]
               },

               color:{min:color, max:colorRand, factor:"time",
                 modifier:[
                   {finalValue:color, interval:[600,1500]}
                 ]
               }
             }}

             physicsModifier={{
               initialExplosiveImpulse:{impulse:0.12, position:[0,0,0], deccelerationPeriod:1.0},
             }}
           />
       ));
  }

  function getFireWorkBrightDarkBright(position, color, color2, delay){
       var viroFireworkColors =["#ff2d2d","#42ff42","#00edff","#ffff00","#ffb5f8","#00ff1d","#00edff","#ffb14c", "#ff7cf4"];
       var colorRand = viroFireworkColors[Math.floor((Math.random() * 5) + 0)];

         return((
           <ViroQuadEmitter
               position={position}
               duration={5000}
               visible={true}
               run={true}
               loop={true}
               fixedToEmitter={true}

               quad={{
                      source:require("../res/particle_firework.png"),                 // Image source of the quad particle.
                      height:0.01,
                      width:0.01,
                      bloomThreshold:0.0
               }}

               spawnModifier={{
                 particleLifetime:[1200,1200],
                 emissionRatePerSecond:[0,0],
                 emissionBurst:[
                   {time:delay, min:500, max:500, cycles:10, cooldownPeriod:1400}
                 ],
                 spawnVolume:{shape:"sphere", params:[0.15], spawnOnSurface:true},
                 maxParticles:800
               }}

               appearanceModifier={{
                 opacity:{min:1.0, max:1.0, factor:"time",
                   modifier:[
                     {finalValue:1.0, interval:[0,350]},
                     {finalValue:0.0, interval:[350,500]},
                     {finalValue:1.0, interval:[600,1000]},
                     {finalValue:0.0, interval:[1000,1200]}
                   ]
                 },

                 color:{min:color, max:colorRand, factor:"time",
                   modifier:[
                     {finalValue:color, interval:[0,500]},
                     {finalValue:color2, interval:[500,600]},
                     {finalValue:color2, interval:[600,1200]},
                   ]
                 }
               }}

               physicsModifier={{
                 initialExplosiveImpulse:{impulse:0.10, position:[0,0,0], deccelerationPeriod:1.2},
               }}
             />
         ));
     }

  function getFireWorkGravity(position, color, delay){
     var viroFireworkColors =["#ff2d2d","#42ff42","#00edff","#ffff00","#ffb5f8","#00ff1d","#00edff","#ffb14c", "#ff7cf4"];
     var colorRand1 = viroFireworkColors[Math.floor((Math.random() * 5) + 0)];
     var colorRand2 = viroFireworkColors[Math.floor((Math.random() * 5) + 0)];

       return((
         <ViroQuadEmitter
             position={position}
             duration={5000}
             visible={true}
             run={true}
             loop={true}
             fixedToEmitter={true}

             quad={{
                    source:require("../res/particle_firework.png"),                 // Image source of the quad particle.
                    height:0.01,
                    width:0.01,
                    bloomThreshold:0.0
             }}

             spawnModifier={{
               particleLifetime:[600,600],
               emissionRatePerSecond:[0,0],
               emissionBurst:[
                 {time:delay, min:400, max:500, cycles:2, cooldownPeriod:1550}
               ],
               spawnVolume:{shape:"sphere", params:[0.15], spawnOnSurface:true},
               maxParticles:800
             }}

             appearanceModifier={{
               opacity:{min:1.0, max:1.0, factor:"time",
                 modifier:[
                   {finalValue:0.0, interval:[0,600]}
                 ]
               },

               color:{min:color, max:colorRand2, factor:"time",
                 modifier:[
                   {finalValue:color, interval:[0,300]},
                   {finalValue:colorRand1, interval:[300,400]},
                   {finalValue:colorRand2, interval:[4000,600]}
                 ]
               }
             }}

             physicsModifier={{
               initialExplosiveImpulse:{impulse:0.1, position:[0,0,0]},
                  acceleration:{min:[0,-1.41,0], max:[0,-1.41,0]}
             }}
           />
       ));
  }
