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

var dynamicStatePosDefault = [-2, 0, 0];
var dynamicDynamicPosDefault = [0,0,0];
var dynamicDynamicPosDefault2 = [0,-2,0];
var kinematicDynamicPos1Default = [2,0.5,0];
var kinematicDynamicPos2Default = [2,-2,0];
var collisionOutputArray = [];
var collisionOutput = "Output:";

var ReleaseMenu = require("./ReleaseMenu.js");
var PhysicsCollisionTest = createReactClass({
  getInitialState() {
    return {
      bodyTypeRayTest:"Dynamic",
      bodyMassRayTest:1,
      dynamicStaticToggle:false,
      dynamicdynamicToggle:false,
      kinematicDynamicToggle:false,
      dynamicStatePos:dynamicStatePosDefault,
      dynamicDynamicPos:dynamicDynamicPosDefault,
      kinematicDynamicPos1:kinematicDynamicPos1Default,
      kinematicDynamicPos2:kinematicDynamicPos2Default
    };
  },

addToConsole(stringToAdd){
  collisionOutputArray.unshift(stringToAdd);

  if (collisionOutputArray.length + 1 > 20){
      collisionOutputArray.pop();
  }

  let output ="Output:";
  let i = 0;
  for (i; i < collisionOutputArray.length; i ++){
      output = output.concat(collisionOutputArray[i]);
  }

  collisionOutput = output;

  this.refs["collisionOutput"].setNativeProps({
    text: collisionOutput
  });
},

  testCollision(tag){
    return () => {
      this.addToConsole("\n");
      let that = this;
      if (tag == 1){
        this.setState({
          dynamicStaticToggle:!this.state.dynamicStaticToggle,
        });

        if (this.state.dynamicStaticToggle == false){
          return;
        }
        this.setState({
          dynamicStatePos:[0,0,-9990]
        });

        setTimeout(function(){
          that.setState({
            dynamicStatePos:[-2, 0, 0]
          });
        }, 300);

      } else if (tag == 2){
        this.setState({
          dynamicdynamicToggle:!this.state.dynamicdynamicToggle,
        });

        if (this.state.dynamicdynamicToggle == false){
          return;
        }

        this.setState({
          dynamicDynamicPos:[0,0,-9990]
        });

        setTimeout(function(){
          that.setState({
            dynamicDynamicPos:dynamicDynamicPosDefault
          });
        }, 200);
      } else if (tag == 3){
        this.setState({
          kinematicDynamicToggle:!this.state.kinematicDynamicToggle,
        });

        if (this.state.kinematicDynamicToggle == false){
          return;
        }

        this.setState({
          kinematicDynamicPos1:[0,0,-9990],
          kinematicDynamicPos2:[0,0,-9990]
        });

        setTimeout(function(){
          that.setState({
            kinematicDynamicPos1:kinematicDynamicPos1Default,
            kinematicDynamicPos2:kinematicDynamicPos2Default
          });
        }, 200);

      }
    }
  },

  onCollide(collidedTag, collidedPoint, collidedNormal){
      let text = "Viro onCollisionBox Javascript: (" + collidedTag + ")  collidedPoint:" + collidedPoint[0] +","+ collidedPoint [1]+"," + collidedPoint[2] + "\n";
      this.addToConsole(text);
  },

  testRay(num){
    return () => {
      this.addToConsole("\n");
      if (num == 1){
         this.refs["scene1"].findCollisionsWithRayAsync([-3,-2,0], [-3,-2,-30], true, "testRayBlaster1").then((collide) => {
           this.addToConsole("Viro Finished find closests With Ray Async call " + collide+ " \n"); }); // Should collide
      } else if (num ==2){
         this.refs["scene1"].findCollisionsWithRayAsync([-3,-2,0], [-3,-2,-30], false, "testRayBlaster2").then((collide) => {
         this.addToConsole("Viro Finished find all With Ray Async call"+ collide+ " \n"); }); // Should collide
      } else if (num ==3){
         this.refs["scene1"].findCollisionsWithShapeAsync([-3,-2,0], [-3,-2,-30], "sphere", [20], "testSphereRay").then((collide) => {
           this.addToConsole("Viro Finished find closests With Shape Async call\n"); }); // Should collide
      } else if (num ==4){
         this.refs["scene1"].findCollisionsWithShapeAsync([-4,-2,-9], [-4,-2,-9], "sphere", [2.5], "testSphere2").then((collide) => {
        this.addToConsole("Viro Finished find all With Shape Async at a point.\n"); }); // Should collide
      }
    }
  },

  togglePhysicsBody(){
    var finalType = "Dynamic";
    var finalMass = 1;
    if (this.state.bodyTypeRayTest === "Dynamic"){
      finalType ="Static";
      finalMass = 0;
    } else if (this.state.bodyTypeRayTest === "Static"){
      finalType = "Kinematic";
      finalMass = 0;
    }
    this.setState({
      bodyTypeRayTest: finalType,
      bodyMassRayTest: finalMass
    });
  },

  render: function() {
    return (
              <ViroScene ref="scene1">
                <ReleaseMenu position={[0 , -3, -4]} sceneNavigator={this.props.sceneNavigator}/>

                <ViroBox
                    physicsBody={{
                      type:'Static', restitution:0.8
                    }}
                    position={[0, -5, -10.5]}
                    scale={[1,1,1]}
                    materials={["Ground"]}
                    rotation={[0,0,0]}
                    height={1}
                    width={50}
                    length={50}
                    viroTag="Ground"
                    onCollision={this.onCollide}
                />

                <ViroText fontSize={25}  style={styles.centeredText} transformBehaviors={["billboard"]}
                  position={[0,3, -6]} width={10} height ={10} maxLines={20}
                  color={'#ffffff'} textAlign={'Center'}
                  ref="collisionOutput"
                  text={collisionOutput}
                />

                 {/* Left half of the screen, tests for collision with ray shot in scene */}
                 <ViroNode position={[-4 , -1, -3]} transformBehaviors={["billboard"]}>
                  <ViroText fontSize={35}  style={styles.centeredText}
                    position={[0,2, 0]} width={4} height ={2} maxLines={3}
                    text={"ToggleType " + this.state.bodyTypeRayTest} onClick={this.togglePhysicsBody}
                    />

                  <ViroText fontSize={35}  style={styles.centeredText}
                      position={[0,1, 0]} width={4} height ={2} maxLines={3}
                      text={"FindClosests With Ray"} onClick={this.testRay(1)}
                      />

                  <ViroText fontSize={35}  style={styles.centeredText}
                      position={[0,0, 0]} width={4} height ={2} maxLines={3}
                      text={"Find All collisions with Ray"} onClick={this.testRay(2)}
                      />

                  <ViroText fontSize={35}  style={styles.centeredText}
                      position={[0,-1, 0]} width={4} height ={2} maxLines={3}
                      text={"Find closests with Shape"} onClick={this.testRay(3)}
                      />

                  <ViroText fontSize={35}  style={styles.centeredText}
                      position={[0,-2, 0]} width={4} height ={2} maxLines={3}
                      text={"Find All collisions with shape at point."} onClick={this.testRay(4)}
                      />
                </ViroNode>

                <ViroNode position={[-3 ,-2, -7]}>
                    <ViroBox
                      position={[0, 0, 0]}
                      scale={[1, 1, 1]}
                      materials={[this.state.bodyTypeRayTest]}
                      height={1} width={1} length={1}
                      physicsBody={{
                        type:this.state.bodyTypeRayTest,
                        mass:this.state.bodyMassRayTest,
                        useGravity:false
                      }}
                      viroTag="RayTest Box1"
                      onCollision={this.onCollide}
                      />

                    <ViroBox
                      position={[0,0,-2]}
                      scale={[1, 1, 1]}
                      materials={[this.state.bodyTypeRayTest]}
                      height={1} width={1} length={1}
                      physicsBody={{
                        type:this.state.bodyTypeRayTest,
                        mass:this.state.bodyMassRayTest,
                        useGravity:false
                      }}
                      viroTag="RayTest Box2"
                      onCollision={this.onCollide}
                      />

                    <ViroBox
                      position={[0,0,-4]}
                      scale={[1, 1, 1]}
                      materials={[this.state.bodyTypeRayTest]}
                      height={1} width={1} length={1}
                      physicsBody={{
                        type:this.state.bodyTypeRayTest,
                        mass:this.state.bodyMassRayTest,
                        useGravity:false
                      }}
                      viroTag="RayTest Box3"
                      onCollision={this.onCollide}
                      />
                </ViroNode>

                {/* Right half of the screen, tests for collision points between objects */}
                <ViroNode position={[4 , -1, -3]} transformBehaviors={["billboard"]}>
                 <ViroText fontSize={35}  style={styles.centeredText}
                   position={[0,2, 0]} width={4} height ={2} maxLines={3}
                   color={this.state.dynamicStaticToggle ? '#0000ff' : '#ffffff'}
                   text={"Test Dynamic-Static"} onClick={this.testCollision(1)}/>

                 <ViroText fontSize={35}  style={styles.centeredText}
                     position={[0,1, 0]} width={4} height ={2} maxLines={3}
                     color={this.state.dynamicdynamicToggle ? '#0000ff' : '#ffffff'}
                     text={"Test Dynamic-Dynamic"} onClick={this.testCollision(2)}/>

                 <ViroText fontSize={35}  style={styles.centeredText}
                     position={[0,0, 0]} width={4} height ={2} maxLines={3}
                     color={this.state.kinematicDynamicToggle ? '#0000ff' : '#ffffff'}
                     text={"Test Kinematic-Dynamic"} onClick={this.testCollision(3)}/>

                </ViroNode>

                <ViroNode position={[3 , -2, -10.5]}>
                  <ViroBox
                      position={this.state.dynamicStatePos}
                      scale={[1, 1, 1]}
                      materials={["Dynamic"]}
                      height={1} width={1} length={1}
                      physicsBody={{
                        type:this.state.dynamicStaticToggle?'Dynamic':'Kinematic',
                        mass:this.state.dynamicStaticToggle?1:0
                      }}
                      viroTag="DynamicBox 1"
                      onCollision={this.onCollide}
                      />

                      <ViroBox
                          position={this.state.dynamicDynamicPos}
                          scale={[1, 1, 1]}
                          materials={["Dynamic"]}
                          height={1} width={1} length={1}
                          physicsBody={this.state.dynamicdynamicToggle ? {
                            type:'Dynamic',
                            mass:1
                          }:undefined}
                          viroTag="DynamicBoxAbove 2"
                          onCollision={this.onCollide}
                          />


                      <ViroBox
                          position={dynamicDynamicPosDefault2}
                          scale={[1, 1, 1]}
                          materials={["Dynamic"]}
                          height={1} width={1} length={1}
                          physicsBody={{
                            type:'Dynamic',
                            mass:1
                          }}
                          viroTag="DynamicBoxBelow 2"
                          onCollision={this.onCollide}
                      />

                      <ViroBox
                          position={this.state.kinematicDynamicPos1}
                          scale={[1, 1, 1]}
                          materials={["Kinematic"]}
                          height={1} width={1} length={1}
                          physicsBody={{
                            type:'Kinematic',
                            mass:0
                          }}
                          viroTag="KinematicBox 3"
                          onCollision={this.onCollide}
                          animation={{name:"loopMoveSmall",
                                      run:this.state.kinematicDynamicToggle }}
                          />

                      <ViroBox
                          position={this.state.kinematicDynamicPos2}
                          scale={[1, 1, 1]}
                          materials={["Dynamic"]}
                          height={1} width={1} length={1}
                          physicsBody={{
                              type:'Dynamic',
                              mass:1,
                          }}
                          viroTag="DynamicBox 3"
                          onCollision={this.onCollide}
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
    moveDownSmall:{properties:{positionY:"-=1.6"}, duration:2000, easing:"Linear"},
    moveUpSmall:{properties:{positionY:"+=1.6"}, duration:2000, easing:"Linear"},
    loopMoveSmall:[
      ["moveDownSmall", "moveUpSmall"]
    ]
});



ViroMaterials.createMaterials({
  Static: {
    shininess: 2.0,
    diffuseColor: "#ff333399"
  },

  Kinematic: {
      cullMode: "None",
      shininess: 2.0,
      diffuseColor: "#3399ff99"

    },

  Dynamic: {
        cullMode: "None",
        shininess: 2.0,
        diffuseColor: "#33cc3399"

      },

  Ground:{
    cullMode: "None",
    shininess: 2.0,
    diffuseColor: "#ff9999"
  }
 });

module.exports = PhysicsCollisionTest;
