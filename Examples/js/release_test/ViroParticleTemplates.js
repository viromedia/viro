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
   ViroQuad,
   ViroSphere,
   Viro3DObject,
   ViroQuadEmitter,
 } from 'react-viro';

 let polarToCartesian = ViroUtils.polarToCartesian;
 var viroFireworkColors =["#ff2d2d","#42ff42","#00edff","#ffff00","#ffb5f8","#00ff1d","#00edff","#ffb14c", "#ff7cf4"];

var createReactClass = require('create-react-class');

 // **This test has not been done.  This is placeholder for scene and navigation arrows**

 var ReleaseMenu = require("./ReleaseMenu.js");

 var ViroParticleTest = createReactClass({
   getInitialState() {
     return {
       apply:true,
       num:3
     };
   },
   getBubbles(){
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
   },


     getSnow(){
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
     },


   getFireWorks(){
     var views = [];
     var radius = 3;
     var colors =["#ff2d2d","#42ff42","#003fff","#ffff00","#ffb5f8","#00ff1d","#00edff","#ffb14c", "#ff7cf4"];

     var f = new Array(30);
     f[1] = this.getFireWork(polarToCartesian([radius, -90 , 30]), colors[1], 0);
     f[2] = this.getFireWork(polarToCartesian([radius, -70 , 20]), colors[1], 3000);
     f[3] = this.getFireWork(polarToCartesian([radius, -50 , 40]), colors[2], 2000);
     f[4] = this.getFireWork(polarToCartesian([radius, -30 , 28]), colors[4], 1000);
     f[5] = this.getFireWork(polarToCartesian([radius, -10 , 25]), colors[5], 1500);
     f[6] = this.getFireWork(polarToCartesian([radius, 10 , 20]), colors[7], 300);
     f[7] = this.getFireWork(polarToCartesian([radius, 30 , 40]), colors[7], 1300);
     f[8] = this.getFireWork(polarToCartesian([radius, 50 , 35]), colors[2], 1900);
     f[9] = this.getFireWork(polarToCartesian([radius, 70 , 25]), colors[5], 2050);
     f[10] = this.getFireWork(polarToCartesian([radius, 80 , 10]), colors[5], 350);

     f[11] = this.getFireWorkGravity(polarToCartesian([radius, -85 , 20]), colors[7], 600);
     f[12] = this.getFireWorkGravity(polarToCartesian([radius, -60 , 45]), colors[0], 1400);
     f[13] = this.getFireWorkGravity(polarToCartesian([radius, -40 , 40]), colors[3], 1800);
     f[14] = this.getFireWorkGravity(polarToCartesian([radius, -20 , 25]), colors[5], 2500);
     f[15] = this.getFireWorkGravity(polarToCartesian([radius, -5 , 30]), colors[6], 1000);
     f[29] = this.getFireWorkGravity(polarToCartesian([radius, 5 , 25]), colors[6], 1500);
     f[16] = this.getFireWorkGravity(polarToCartesian([radius, 20 , 20]), colors[3], 2000);
     f[17] = this.getFireWorkGravity(polarToCartesian([radius, 40 , 40]), colors[3], 1600);
     f[18] = this.getFireWorkGravity(polarToCartesian([radius, 60 , 25]), colors[3], 100);
     f[19] = this.getFireWorkGravity(polarToCartesian([radius, 85 , 36]), colors[3], 2300);


     f[20] = this.getFireWorkBrightDarkBright(polarToCartesian([radius, -80 , 30]), colors[0], colors[3], 2340);
     f[21] = this.getFireWorkBrightDarkBright(polarToCartesian([radius, -50 , 40]), colors[1], colors[4], 0);
     f[22] = this.getFireWorkBrightDarkBright(polarToCartesian([radius, -25 , 20]), colors[2], colors[5], 800);
     f[23] = this.getFireWorkBrightDarkBright(polarToCartesian([radius, -15 , 33]), colors[3], colors[7], 200);
     f[24] = this.getFireWorkBrightDarkBright(polarToCartesian([radius, 8 , 40]), colors[4], colors[6], 1900);
     f[25] = this.getFireWorkBrightDarkBright(polarToCartesian([radius, 15 , 20]), colors[4], colors[6], 1700);
     f[26] = this.getFireWorkBrightDarkBright(polarToCartesian([radius, 35 , 33]), colors[4], colors[6], 200);
     f[27] = this.getFireWorkBrightDarkBright(polarToCartesian([radius, 65 , 40]), colors[4], colors[6], 3720);
     f[28] = this.getFireWorkBrightDarkBright(polarToCartesian([radius, 90 , 25]), colors[4], colors[6], 1020);

     var i =0;
     for ( i =0; i < f.length ; i ++){
       views.push(f[i]);
     }
     return views;
   },

    getFireWork(position, color, delay){
      var colorRand = viroFireworkColors[Math.floor((Math.random() * 5) + 0)];

        return((
          <ViroQuadEmitter
              position={position}
              duration={4000}
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
    },


      getFireWorkBrightDarkBright(position, color, color2, delay){
        var colorRand = viroFireworkColors[Math.floor((Math.random() * 5) + 0)];

          return((
            <ViroQuadEmitter
                position={position}
                duration={3000}
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
      },

    getFireWorkGravity(position, color, delay){

      var colorRand1 = viroFireworkColors[Math.floor((Math.random() * 5) + 0)];
      var colorRand2 = viroFireworkColors[Math.floor((Math.random() * 5) + 0)];

        return((
          <ViroQuadEmitter
              position={position}
              duration={3000}
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
                  {time:delay, min:400, max:500, cycles:2, cooldownPeriod:1000}
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
