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
  ViroScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroUtils,
  ViroText,
  ViroQuad,
  ViroOmniLight,
  ViroAnimations,
} from 'react-viro';

var createReactClass = require('create-react-class');

var bouncePosDefault = [-2, 5, 0];
var frictionPosDefault = [2,0,2];

var ReleaseMenu = require("./ReleaseMenu.js");
var PhysicsRayTest = createReactClass({
  getInitialState() {
    return {
      restitution: 0,
      friction: 0,
      toggleBounce:false,
      toggleFriction:false,
      bouncePos:bouncePosDefault,
      frictionPos:frictionPosDefault
    };
  },

  toggleProperty(num){
      return () => {
          let that = this;
          if (num == 1){
            this.setState({
             toggleBounce:!this.state.toggleBounce
            });

            if (this.state.toggleBounce == true){
              return;
            }
              this.setState({
                bouncePos:[0,0,-9990]
              });

              setTimeout(function(){
                that.setState({
                  bouncePos:[-2, 5, 0]
                });
              }, 200);


          } else if (num == 2){
            this.setState({
             toggleFriction:!this.state.toggleFriction
            });

            if (this.state.toggleFriction == true){
                return;
            }
              this.setState({
                frictionPos:[0,0,-9990]
              });

              setTimeout(function(){
                that.setState({
                  frictionPos:frictionPosDefault
                });
              }, 200);

          }
      }
  },

  toggleBounce(){
    let restitution =  this.state.restitution + 0.1;

    if (restitution > 1){
      restitution = 0;
    }

    this.setState({
     restitution:restitution
    });
  },

  toggleFriction(){
    let friction = this.state.friction + 0.1;
    if (friction > 1){
      friction = 0;
    }
    this.setState({
     friction:friction
    });
  },
  onReset(){
    let that = this;
    this.setState({
      reset:true
    });

    setTimeout(function(){
      that.setState({
        reset:false
      });

    }, 500);
  },

  render: function() {
    if (this.state.reset){
      return (<ViroScene />);
    }

    return (
              <ViroScene ref="scene1">
                <ReleaseMenu position={[-2,3,-5]} sceneNavigator={this.props.sceneNavigator}/>
                <ViroBox
                    physicsBody={{
                      type:'static', restitution:1.0
                    }}
                    position={[0, -5, -10.5]}
                    scale={[1,1,1]}
                    materials={["ground"]}
                    rotation={[0,0,0]}
                    height={1}
                    width={50}
                    length={50}
                    viroTag="Ground"
                    onCollision={this.onCollide}
                />

                 {/* Left half of the screen, tests for collision with ray shot in scene */}
                 <ViroNode position={[-2 , -1, -5]} transformBehaviors={["billboard"]}>
                 <ViroText fontSize={35}  style={styles.centeredText}
                   position={[0,3, 0]} width={4} height ={2} maxLines={3}
                   color={this.state.toggleBounce ? '#0000ff' : '#ffffff'}
                   text={"Release Blue Box " } onClick={this.toggleProperty(1)}
                   />
                  <ViroText fontSize={35}  style={styles.centeredText}
                    position={[0,2, 0]} width={4} height ={2} maxLines={3}
                    text={"Restitution: " + this.state.restitution} onClick={this.toggleBounce}
                    />
                  <ViroText fontSize={35}  style={styles.centeredText}
                        position={[0,1, 0]} width={4} height ={2} maxLines={3}
                        color={this.state.toggleFriction ? '#0000ff' : '#ffffff'}
                        text={"Push Green Box "} onClick={this.toggleProperty(2)}
                        />
                  <ViroText fontSize={35}  style={styles.centeredText}
                      position={[0,0, 0]} width={4} height ={2} maxLines={3}
                      text={"Friction: " + this.state.friction} onClick={this.toggleFriction}
                      />
                      <ViroText fontSize={35}  style={styles.centeredText}
                          position={[0,-1, 0]} width={4} height ={2} maxLines={3}
                          text={"Hard Reset."} onClick={this.onReset}/>
                </ViroNode>

                <ViroNode position={[-3 ,-2, -7]}>
                    <ViroBox
                      position={this.state.bouncePos}
                      scale={[1, 1, 1]}
                      materials={'orange'}
                      height={1} width={1} length={1}
                      physicsBody={{
                        type:this.state.toggleBounce ? 'dynamic' : 'kinematic',
                        mass:this.state.toggleBounce ? 5 :0,
                        restitution:this.state.restitution
                      }}
                      />

                    <ViroBox
                      position={this.state.frictionPos}
                      scale={[1, 1, 1]}
                      materials={'green'}
                      height={1} width={1} length={1}
                      physicsBody={{
                        type:this.state.toggleFriction ? 'dynamic' : 'kinematic',
                        mass:this.state.toggleFriction ? 5 :0,
                        friction:this.state.friction,
                        force: this.state.toggleFriction ? {value:[0,0,-30]} : {value:[0,0,0]}
                      }}
                      />
                </ViroNode>

                <ViroOmniLight
                      position={[0, 0, 0]}
                      color={"#ffffff"}
                      attenuationStartDistance={30}
                      attenuationEndDistance={40}/>
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

ViroAnimations.registerAnimations({
    moveDown:{properties:{positionY:"-=3"}, duration:2000, easing:"Linear"},
    moveUp:{properties:{positionY:"+=3"}, duration:2000, easing:"Linear"},
    loopMove:[
      ["moveDown", "moveUp"]
    ],
    moveDownSmall:{properties:{positionY:"-=2"}, duration:2000, easing:"Linear"},
    moveUpSmall:{properties:{positionY:"+=2"}, duration:2000, easing:"Linear"},
    loopMoveSmall:[
      ["moveDownSmall", "moveUpSmall"]
    ]
});



ViroMaterials.createMaterials({
  orange: {
      cullMode: "None",
      shininess: 2.0,
      diffuseColor: "#3399ff99"

    },

  green: {
        cullMode: "None",
        shininess: 2.0,
        diffuseColor: "#33cc3399"

      },

  ground:{
    cullMode: "None",
    shininess: 2.0,
    diffuseColor: "#ff9999"
  }
 });

module.exports = PhysicsRayTest;
