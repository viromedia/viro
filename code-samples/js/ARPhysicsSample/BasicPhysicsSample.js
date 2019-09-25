'use strict';

import React, { Component } from 'react';
import {StyleSheet} from 'react-native';

import {
  ViroARScene,
  ViroText,
  Viro360Image,
  ViroMaterials,
  ViroBox,
  ViroQuad,
  Viro3DObject,
  ViroOmniLight,
  ViroController,
  ViroNode,
  ViroARPlane,
  ViroFlexView,
  ViroAmbientLight,
  ViroLightingEnvironment
} from 'react-viro';

var createReactClass = require('create-react-class');
var CONTROLLER_PUSH = 1;
var CONTROLLER_GRIP = 2;
var CONTROLLER_PULL = 3;

/*
 Basic Physics example demonstrating how to apply constant forces, and
 as well how to apply an impulse force, inreference to a ViroController
 in response to user actions.
 */
var BasicARPhysicsSample = createReactClass({
  getInitialState() {
    return {
      controllerConfig:CONTROLLER_GRIP,
      showCollisionBox:false,
      foundPlane:false,
      planePosition:[0,0,0],
      planeRotation:[0,0,0],
      totalCubes:0
    };
  },

  render: function() {
    return (
     <ViroARScene physicsWorld={{ gravity:[0,-9.81,0], drawBounds:this.state.showCollisionBox }} ref={(component)=>{this.sceneRef = component}}>
      <ViroAmbientLight color={"#FFFFFF"} intensity={10}/>
      <ViroLightingEnvironment source={require('./res/ibl_envr.hdr')}/>

      {/* ViroARPlaneSelector to detect for placing our "floor" plane and physics scene. */}
      <ViroARPlane
        key={"firstPlane"}
        ref={(component)=>{this.arPlaneRef = component}}
        onAnchorFound={this._onAnchorFound}>

        {/* Create our physics group node example, at the location of user-selected plane */}
        {this._getPhysicsGroup()}

      </ViroARPlane>
     </ViroARScene>
    );
  },

  /*
   When the anchor plane is found, set our physics example at the location of the plane.
   And then freeze the plane at that location to prevent it from re-adjusting to updates.
   */
  _onAnchorFound(anchorMap){
    if (anchorMap.type != "plane"){
      return;
    }

    var worldCenterPosition = [anchorMap.position[0] + anchorMap.center[0],
                               anchorMap.position[1] + anchorMap.center[1],
                               anchorMap.position[2] + anchorMap.center[2]];
    this.arPlaneRef.setNativeProps({"pauseUpdates":true});
    this.setState({foundPlane:true, planePosition:worldCenterPosition, planeRotation:anchorMap.rotation});
  },

  _getPhysicsGroup(){
    if (!this.state.foundPlane){
      return;
    }

    this.ballProperties = {friction:0.6, type:'Dynamic', mass:4, enabled:true, useGravity:true, shape:{type:'Sphere', params:[0.14]}, restitution:0.65};
    return (
      <ViroNode position={this.state.planePosition}>

        {/* Create 2 UI buttons for controlling interaction with the basketballs */}
        {this._getHUDControl()}

        {/* Bind controls for interacting with the scene.*/}
        <ViroController ref={(component)=>{this.controllerRef = component}} />

        {/* A Single Ball we have spawned in our scene */}
        <Viro3DObject ref={(obj)=>{this.ball = obj}}
                      source={require('./res/object_basketball_pbr.vrx')}
                      scale={[0.5, 0.5, 0.5]}
                      position={[0, 0, 1.0]}
                      rotation={[0, 0, 0]}
                      resources={[require('./res/blinn1_Base_Color.png'),
                                  require('./res/blinn1_Metallic.png'),
                                  require('./res/blinn1_Roughness.png'),
                                  require('./res/blinn1_Normal_OpenGL.png')]}
                      type="VRX"
                      physicsBody={this.ballProperties}
                      viroTag="BallTag"
                      onClick={this.state.controllerConfig == CONTROLLER_PUSH ? this.onItemPushImpulse("BallTag") : undefined}
                      onDrag={this.state.controllerConfig == CONTROLLER_GRIP ? ()=>{} : undefined}/>

        {/* Render cube object in the scene, if any.*/}
        {this._renderCubes()}

        {/* Quad representing the ground. */}
        <ViroQuad position={[0,0,0]} scale={[6.0, 8.0, 1.0]} rotation={[-90, 0, 0]} physicsBody={{ type:'Static', restitution:0.75 }}
          onClickState={this.state.controllerConfig == CONTROLLER_PULL ? this.onItemPullForce("Surface") : undefined}
          ref={(component)=>{this.floorSurface = component}} onCollision={this._onFloorCollide} materials={'ground'}/>
      </ViroNode>
    );
  },

  _getHUDControl(){
    return (
      <ViroNode position={[0, 1.5, -7.75]} transformBehaviors={["billboardX", "billboardY"]}>
        <ViroFlexView style={{flexDirection: 'column'}} width={1} height={0.8} materials="hud_text_bg" position={[-1.5,0,0]} onClick={this._resetScene}>
          <ViroText style={styles.hud_text}  text={ "Reset Scene"} />
        </ViroFlexView>

        <ViroFlexView style={{flexDirection: 'column'}} width={1} height={0.8} materials="hud_text_bg" position={[0,0,0]} onClick={this._toggleControllerInteraction}>
          <ViroText style={styles.hud_text}  text={this._getControllerTextMode()} />
        </ViroFlexView>

        <ViroFlexView style={{flexDirection: 'column'}} width={1} height={0.8} materials="hud_text_bg" position={[1.5,0,0]} onClick={this._toggleCollisionBox}>
          <ViroText style={styles.hud_text}  text={"Toggle Phyz boxes"} />
        </ViroFlexView>

        <ViroFlexView style={{flexDirection: 'column'}} width={1} height={0.8} materials="hud_text_bg" position={[3,0,0]} onClick={this._addCube}>
          <ViroText style={styles.hud_text}  text={"Add Cube"} />
        </ViroFlexView>
      </ViroNode>
    )
  },

  _getControllerTextMode(){
    if (this.state.controllerConfig == 1){
      return "Controller Push Mode";
    } else if (this.state.controllerConfig == 2){
      return "Controller Drag Mode";
    } else {
      return "Controller Pull Mode";
    }
  },

  _resetScene() {
    // Reset the ball to it's default position.
    setTimeout(() => {
        this.ball.setNativeProps({"physicsBody":null});
        this.ball.setNativeProps({"position":[0, 1, -0.3]});
        this.floorSurface.setNativeProps({materials:["ground"]});

        setTimeout(() => {
            this.ball.setNativeProps({"physicsBody":this.ballProperties});
        }, 500);
    }, 500);
  },

  _toggleCollisionBox(){
    this.setState({showCollisionBox:!this.state.showCollisionBox});
  },

  _toggleControllerInteraction(){
    var endConfig = 1;
    if (this.state.controllerConfig == 1){
      endConfig = 2;
    } else if (this.state.controllerConfig == 2){
      endConfig = 3;
    }
    this.setState({controllerConfig:endConfig});
  },

  /*
   Push against the ball with an impulse force, at the onClicked location, and
   with a force direction originating from the controller (controller forward).
   */
  onItemPushImpulse(itemTag){
    return (clickedPos, source) =>{
      this.controllerRef.getControllerForwardAsync().then((forward)=>{
        var pushStrength = 3;
        var pushImpulse = [forward[0]*pushStrength, forward[1]*pushStrength, forward[2]*pushStrength];
        this.ball.getTransformAsync().then((transform) => {
          var pos = transform.position;
          var pushPosition = [clickedPos[0] - pos[0], clickedPos[1] - pos[1], clickedPos[2] - pos[2]];
          this.ball.applyImpulse(pushImpulse, pushPosition);
        });
      });
    }
  },

  /*
   Pull the ball with a constant force towards the camera.
   */
  onItemPullForce(itemTag){
    return (state, position, source) => {
      this.sceneRef.getCameraOrientationAsync().then((camTransform)=>{
        this.ball.getTransformAsync().then((ballTransform)=>{
          var ballPos = ballTransform.position;
          var camPos = camTransform.position;
          var pullVec = [camPos[0] - ballPos[0], camPos[1] - ballPos[1], camPos[2] - ballPos[2]];

          var pullStrength = 5; // Force multiplier.
          var pullStrengthVec = [pullVec[0]*pullStrength, pullVec[1]*pullStrength, pullVec[2]*pullStrength];  // Force in newtons
          if (state == 1) {
            var phyzProps = {force:{value:pullStrengthVec}, type:'Dynamic', mass:4, enabled:true, useGravity:true, shape:{type:'Sphere', params:[0.14]}, restitution:0.65};
          } else {
            var phyzProps = {type:'Dynamic', mass:4, enabled:true, useGravity:true, shape:{type:'Sphere', params:[0.14]}, restitution:0.65};
          }
          this.ball.setNativeProps({"physicsBody":phyzProps});
        });
      });
    }
  },

 _onFloorCollide(collidedTag, collidedPoint, collidedNormal){
    console.log("Viro box has collided on the " + collidedTag);
    if (collidedTag == "BallTag"){
      this.floorSurface.setNativeProps({materials:["ground_hit"]});
    }
  },

  _renderCubes(){
    var views = [];
    for (var i = 0; i < this.state.totalCubes; i ++){
      var cubeKey = "CubeTag_" + i;
      views.push((<ViroBox scale={[0.2, 0.2, 0.2]}
                        position={[-0.5, 1, -1.3]}
                        rotation={[0, 0, 0]}
                        physicsBody={{type:'Dynamic', mass:25, enabled:true, useGravity:true, restitution:0.35, friction:0.75}}
                        materials="cube_color"
                        key={cubeKey}
                        onClick={this.state.controllerConfig == CONTROLLER_PUSH ? this.onItemPushImpulse(cubeKey) : undefined}
                        onDrag={this.state.controllerConfig == CONTROLLER_GRIP ? ()=>{} : undefined}
                        />));
    }
    return views;
  },

  _addCube(){
    this.setState({totalCubes:this.state.totalCubes + 1});
  },
});

var styles = StyleSheet.create({
  hud_text: {
       fontSize: 18,
       fontFamily: 'Arial',
       color: '#0000ff',
       flex: 1,
  },
});

ViroMaterials.createMaterials({
  hud_text_bg: {
    diffuseColor: "#00ffff"
  },
  ground:{
    diffuseColor: "#007CB6E6"
  },
  ground_hit:{
    diffuseColor: "#008141E6"
  },
  cube_color:{
    diffuseColor: "#0021cbE6"
  }
 });

module.exports = BasicARPhysicsSample;
