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
  View,
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
   ViroParticleEmitter,
   ViroAnimatedComponent,
   ViroQuad,
   ViroSkyBox,
   ViroPortal,
   ViroPortalFrame,
   ViroSphere,
   Viro3DObject,
 } from 'react-viro';

var createReactClass = require('create-react-class');

var lightMask1:3              // 0011
var lightMask2:5              // 0101
var lightMask3:13             // 1101
var allLights: 15             // 1111


var durationFinal = 3000;
var fireworkParticle = require("./res/particle_firework.png");
var cloudParticle = require("./res/particle_bubble.png");
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroShadowTest = createReactClass({
  getInitialState() {
    return {
      emissionRatePerSecond:5,
      particleLifetime:2000,
      maxParticles:30,
      emissionBurst:undefined,
      spawnVolume:undefined,
      spawnOnSurface:true,
      velocityScale:3,
      accelerationScale:0,
      explosion:undefined,

      disablePhysicsModifier:false,
      disableAppearanceModifier:false,
      disableSpawnModifier:false,

      opacity:undefined,
      scale:undefined,
      rotation:undefined,
      color:undefined,

      duration:durationFinal,
      delay:0,
      loop:true,
      run:true,
      fixedToEmitter:true,

      quadSize:0.1,
      quadSource:1,
      quadBloom: -1.0 ,

      runAnimation:false,
      blendMode:"Add"

    };
  },

  toggleProperty(num){
      return () => {
          let that = this;
          if (num == 1){
            var emissionRatePerSecondCurrent = this.state.emissionRatePerSecond + 10;
            if (emissionRatePerSecondCurrent > 100){
              emissionRatePerSecondCurrent = 5;
            }
            this.setState({
             emissionRatePerSecond:emissionRatePerSecondCurrent
            });
          } else if (num == 2){
            var particleLifetimeCurrent = this.state.particleLifetime + 500;
            if (particleLifetimeCurrent > 10000){
              particleLifetimeCurrent = 500;
            }
            this.setState({
             particleLifetime:particleLifetimeCurrent
            });
          } else if (num == 3){
            var maxParticlesCurrent = this.state.maxParticles + 40;
            if (maxParticlesCurrent > 800){
              maxParticlesCurrent = 100;
            }
            this.setState({
             maxParticles:maxParticlesCurrent
            });
          } else if (num == 4){
            var emissionBurst = this.state.emissionBurst;
            if (emissionBurst != undefined){
              emissionBurst = undefined;
            } else {
              emissionBurst = [
                {time:500, min:40, max:40, cycles:1, cooldownPeriod:1000},
                {time:1000, min:50, max:50, cycles:2, cooldownPeriod:1000},
                {time:4000, min:100, max:100, cycles:2, cooldownPeriod:100}
              ]
            }
            this.setState({
             emissionBurst:emissionBurst
            });
          } else if (num == 5){
            if (this.state.spawnVolume === undefined){
              this.setState({
                  spawnOnSurface:!this.state.spawnOnSurface,
              });
              return;
            }

            var shape = {
              shape:this.state.spawnVolume.shape,
              params:this.state.spawnVolume.params,
              spawnOnSurface:!this.state.spawnOnSurface
            };

            this.setState({
             spawnOnSurface:!this.state.spawnOnSurface,
             spawnVolume:shape
            });
          } else if (num == 6){
            var spawnShape = this.state.spawnVolume;
            if (spawnShape === undefined){
              spawnShape = {shape:"sphere", params:[1], spawnOnSurface:this.state.spawnOnSurface};
            } else if (spawnShape.shape =="sphere"){
              spawnShape = {shape:"box", params:[2, 2, 2], spawnOnSurface:this.state.spawnOnSurface};
            } else if (spawnShape.shape =="box"){
              spawnShape = undefined;
            }
            this.setState({
             spawnVolume:spawnShape
            });
          } else if (num == 7){
            let velocityScaleCurrent = this.state.velocityScale + 1;
            if (velocityScaleCurrent > 7){
              velocityScaleCurrent = 0;
            }
            this.setState({
             velocityScale:velocityScaleCurrent
            });

          } else if (num == 8){
            let accelerationScaleCurrent = this.state.accelerationScale + 1;
            if (accelerationScaleCurrent > 15){
              accelerationScaleCurrent = 0;
            }
            this.setState({
             accelerationScale:accelerationScaleCurrent
            });
          } else if (num == 9){       // Explode with a decceleration period, like fireworks.
            var explosion = this.state.explosion;
            if (explosion == undefined){
              explosion = {impulse:1.12, position:[0,0,0], decelerationPeriod:3.0};
            } else {
              explosion = undefined;
            }

            this.setState({
             explosion:explosion
            });
          } else if (num == 10){      // Explode, but do not deccelerate.
            var explosion = this.state.explosion;
            if (explosion == undefined){
              explosion = {impulse:1.12, position:[0,0,0]};
            } else {
              explosion = undefined;
            }

            this.setState({
             explosion:explosion
            });
          } else if (num == 11){      // Opacity that causes a particle to dissappear
            var opacity = this.state.opacity;
            if (opacity != undefined){
              opacity = undefined;
            } else {
              opacity = {initialRange:[1.0, 1.0], factor:"time",
                interpolation:[
                  {endValue:0.0, interval:[0, this.state.particleLifetime/3*3]}
                ]
              };
            }

            this.setState({
             opacity:opacity
            });
          } else if (num == 12){
            var scale = this.state.scale;
            if (scale != undefined){
              scale = undefined;
            } else {
              scale = {
                initialRange:[[0, 0, 0],[0, 0, 0]],
                factor:"time",
                interpolation:[
                  {endValue:[6.0, 6.0, 6.0], interval:[ 0 ,this.state.particleLifetime/3*3]}
                ]
              };
            }

            this.setState({
             scale:scale
            });
          } else if (num == 13){
            var rotation = this.state.rotation;
            if (rotation != undefined){
              rotation = undefined;
            } else {
              rotation = {initialRange:[0.0, 1.0], factor:"time",
                interpolation:[
                  {endValue:0.0, interval:[0,this.state.particleLifetime/3*1]},
                  {endValue:360.0, interval:[this.state.particleLifetime/3*1,this.state.particleLifetime/3*2]},
                  {endValue:0.0, interval:[this.state.particleLifetime/3*2,this.state.particleLifetime/3*3]}
                ]
              };
            }

            this.setState({
             rotation:rotation
            });
          } else if (num == 14){
            var color = this.state.color;
            if (color != undefined){
              color = undefined;
            } else {
              color = {initialRange:["#ffffff", "#ffffff"], factor:"time",
                interpolation:[
                  {endValue:"#fdfa00", interval:[0,this.state.particleLifetime/3*1]},
                  {endValue:"#ff00ff", interval:[this.state.particleLifetime/3*1,this.state.particleLifetime/3*2]},
                  {endValue:"#ff0f0f", interval:[this.state.particleLifetime/3*2,this.state.particleLifetime/3*3]}
                ]
              };
            }

            this.setState({
             color:color
            });
          } else if (num == 15){
            var durationCurrent = this.state.duration + 500;
            if (durationCurrent > 6000){
              durationCurrent = durationFinal;
            }
            this.setState({
             duration:durationCurrent
            });
          } else if (num == 16){
            var delayCurrent = this.state.delay + 500;
            if (delayCurrent > 5000){
              delayCurrent = 0;
            }
            this.setState({
             delay:delayCurrent
            });
          } else if (num == 17){
            this.setState({
             loop:!this.state.loop
            });
          } else if (num == 18){
            this.setState({
             run:!this.state.run
            });
          } else if (num == 19){
            this.setState({
             fixedToEmitter:!this.state.fixedToEmitter
            });
          } else if (num == 20){
            this.setState({
             quadSource:this.state.quadSource == 1 ? 0 : 1
            });
          } else if (num == 21){
            var quadSizeCurrent = this.state.quadSize + 0.1;
            if (quadSizeCurrent > 0.5){
              quadSizeCurrent = 0.1;
            }

            this.setState({
             quadSize:quadSizeCurrent
            });
          } else if (num == 22){
            console.log("Daniel quad bloom is : " + this.state.quadBloom);
            this.setState({
             quadBloom:this.state.quadBloom == 0.5 ? -1.0 : 0.5
            });
          } else if (num == 23){
            this.setState({
             disableSpawnModifier:!this.state.disableSpawnModifier
            });
          } else if (num == 24){
            this.setState({
             disableAppearanceModifier:!this.state.disableAppearanceModifier
            });
          } else if (num == 25){
            this.setState({
             disablePhysicsModifier:!this.state.disablePhysicsModifier
            });
          }  else if (num == 26){
            this.setState({
              runAnimation:!this.state.runAnimation
            });
          } else if (num == 27){
            var mode = this.state.blendMode;
            if (mode == "Add"){
              mode = "Alpha";
            } else if (mode == "Alpha"){
              mode = "Multiply";
            } else if (mode == "Multiply"){
              mode = "Add";
            }
            this.setState({blendMode:mode});
          }
      }
  },

  render: function() {
    if (this.state.reset){
      return (<ViroScene />);
    }

    return (
              <ViroScene ref="scene1">
              <ReleaseMenu position={[0 , -3, -4]} sceneNavigator={this.props.sceneNavigator}/>

                 {/* Left half of the screen, tests for collision with ray shot in scene */}
                 <ViroNode position={[-4 , 7, -3]} transformBehaviors={["billboard"]}>
                 <ViroText fontSize={35}  style={styles.centeredText} lightBitMask={0} // 0 to avoid influencing the test
                   position={[0,0, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle emissionRatePerSecond " + this.state.emissionRatePerSecond }
                   onClick={this.toggleProperty(1)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-1, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle particleLifetime " + this.state.particleLifetime }
                   onClick={this.toggleProperty(2)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-2, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle maxParticles " + this.state.maxParticles }
                   onClick={this.toggleProperty(3)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-3, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle emissionBurst" }
                   onClick={this.toggleProperty(4)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-4, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle spawnOnSurface " + this.state.spawnOnSurface }
                   onClick={this.toggleProperty(5)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-5, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle shape " + (this.state.spawnVolume != undefined ? this.state.spawnVolume.shape : "point") }
                   onClick={this.toggleProperty(6)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-6, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle velocityScale " + this.state.velocityScale }
                   onClick={this.toggleProperty(7)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-7, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle accelerationScale " + this.state.accelerationScale }
                   onClick={this.toggleProperty(8)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-8, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle explosion with slowdown"}
                   onClick={this.toggleProperty(9)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-9, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle explosion no slowdown "}
                   onClick={this.toggleProperty(10)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-10, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle opacity modifier "}
                   onClick={this.toggleProperty(11)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-11, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle scale modifier " }
                   onClick={this.toggleProperty(12)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-12, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle rotation modifier " }
                   onClick={this.toggleProperty(13)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-13, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle color modifier " }
                   onClick={this.toggleProperty(14)}
                 />
                 </ViroNode>
                 <ViroNode position={[5 , 5, -8]} transformBehaviors={["billboard"]}>

                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,0, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle particle duration" + this.state.duration }
                   onClick={this.toggleProperty(15)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-1, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle delay " + this.state.delay }
                   onClick={this.toggleProperty(16)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-2, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle loop " + this.state.loop }
                   onClick={this.toggleProperty(17)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-3, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle run " + this.state.run}
                   onClick={this.toggleProperty(18)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-4, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle fixedToEmitter " + this.state.fixedToEmitter }
                   onClick={this.toggleProperty(19)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-5, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle quadSource " }
                   onClick={this.toggleProperty(20)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-6, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle quadSize " + this.state.quadSize}
                   onClick={this.toggleProperty(21)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-7, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle quadBloom " + this.state.quadBloom }
                   onClick={this.toggleProperty(22)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-8, 0]} width={6} height ={1} maxLines={1}
                   text={"Disable SpawnModifier " + this.state.disableSpawnModifier }
                   onClick={this.toggleProperty(23)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-9, 0]} width={6} height ={1} maxLines={1}
                   text={"Disable Appearance Modifier " + this.state.disableAppearanceModifier }
                   onClick={this.toggleProperty(24)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-10, 0]} width={6} height ={1} maxLines={1}
                   text={"Disable Physics Modifier  " + this.state.disablePhysicsModifier }
                   onClick={this.toggleProperty(25)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-11, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle runAnimation " + this.state.runAnimation}
                   onClick={this.toggleProperty(26)}
                 />
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,-12, 0]} width={6} height ={1} maxLines={1}
                   text={"Toggle blendMode " + this.state.blendMode}
                   onClick={this.toggleProperty(27)}
                 />
                </ViroNode>


                <ViroSkyBox color="#ff0000" />

                  <ViroNode position={[0,-2,-4]}  animation={{name:"sequentialAnimParticle", loop:true, run:this.state.runAnimation}}>
                    <ViroParticleEmitter
                        delay={this.state.delay}
                        position={[0,0,0]}
                        duration={this.state.duration}
                        visible={true}
                        run={this.state.run}
                        loop={this.state.loop}
                        fixedToEmitter={this.state.fixedToEmitter}

                        image={{
                               source:this.state.quadSource == 1? fireworkParticle : cloudParticle,                 // Image source of the quad particle.
                               height:this.state.quadSize,
                               width:this.state.quadSize,
                               bloomThreshold:this.state.quadBloom,
                               blendMode:this.state.blendMode
                        }}

                        spawnBehavior={this.state.disableSpawnModifier? undefined :{
                          particleLifetime:[this.state.particleLifetime, this.state.particleLifetime],
                          emissionRatePerSecond:[this.state.emissionRatePerSecond, this.state.emissionRatePerSecond],
                          emissionBurst:this.state.emissionBurst,
                          spawnVolume:this.state.spawnVolume,
                          maxParticles:this.state.maxParticles
                        }}

                        particleAppearance={this.state.disableAppearanceModifier? undefined :{
                          opacity:this.state.opacity,
                          color:this.state.color,
                          rotation:this.state.rotation,
                          scale:this.state.scale
                        }}

                        particlePhysics={this.state.disablePhysicsModifier? undefined :{
                          velocity:{
                            initialRange:[
                              [-0.3 * this.state.velocityScale, 1* this.state.velocityScale, 0],
                              [0.3 * this.state.velocityScale, 1* this.state.velocityScale, 0]
                            ]
                          },
                          acceleration:{
                            initialRange:[
                              [0,-1 * this.state.accelerationScale,0],
                              [0,-1 * this.state.accelerationScale,0]
                            ]
                          },
                          explosiveImpulse:this.state.explosion

                        }}
                      />
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
    color: '#ffffff',
    flex: 1,
  },
  centeredText: {
       fontFamily: 'Arial',
       color: '#ffffff',
       flex: 1,
  },

});

ViroMaterials.createMaterials({
  blue: {
    lightingModel: "Blinn",

      cullMode: "None",
      shininess: 2.0,
      diffuseColor: "#3399ff99"
    },
    ground: {
      lightingModel: "Blinn",

        cullMode: "None",
        shininess: 2.0,
        diffuseColor: "#ffffff"
      },
    shadowCatcher: {
      writesToDepthBuffer: false,
    },
  green: {
        cullMode: "None",
        shininess: 2.0,
        diffuseColor: "#33cc3399"

      },
 });

 ViroAnimations.registerAnimations({
   moveRightParticle:{properties:{positionX:"+=4",}, duration:3000, delay:0},
   moveLeftParticle:{properties:{positionX:"-=4",}, duration:3000, delay:0},
   sequentialAnimParticle:[
         ["moveRightParticle","moveLeftParticle"]
   ],
 });

module.exports = ViroShadowTest;
