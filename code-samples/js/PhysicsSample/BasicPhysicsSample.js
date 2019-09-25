'use strict';

import React, { Component } from 'react';
import {StyleSheet} from 'react-native';

import {
  ViroScene,
  ViroText,
  Viro360Image,
  ViroMaterials,
  ViroBox,
  Viro3DObject,
  ViroOmniLight,
  ViroController
} from 'react-viro';

var createReactClass = require('create-react-class');

/*
 Basic Physics example demonstrating how to apply constant forces, and
 as well how to apply an impulse force, inreference to a ViroController
 in response to user actions.
 */
var BasicPhysicsSample = createReactClass({
  getInitialState() {
    return {
      boxConstantForce:[0,0,0]
    };
  },

  componentDidMount: function() {
    this.disEngageBoosters();
  },

  engageBoosters(){
    this.setState({
      boxConstantForce:[0,20,0]
    });

    let that = this;
    setTimeout(
      () => {
        that.disEngageBoosters()},
      4000
    );
  },

  disEngageBoosters(){
    this.setState({
      boxConstantForce:[0,0,0]
    });

    let that = this;
    setTimeout(
      () => { that.engageBoosters()},
      4000
    );
  },

  render: function() {
    return (
     <ViroScene>
      <ViroController ref={this._setControllerNodeRef} />
       <ViroBox
           physicsBody={{
             type:'Static'
           }}
           position={[0, -2, -3]}
           materials={["ground"]}
           height={1} width={50} length={50}
           viroTag="Ground"
           onCollision={this.onGroundCollide}
           onClick={this.spawnBox}
       />

       <ViroBox
         ref={this._setBoxRef}
         position={[0, 6, -3]}
         materials={'box'}
         physicsBody={{
           type:'Dynamic',
           mass: 2,
           force:{value:this.state.boxConstantForce}
         }}
         viroTag="Box"
         onCollision={this.onBoxCollide}
         onClick={this.onBoxClicked}
         onDrag ={this.onDragg}
         />

     </ViroScene>
    );
  },

  _setControllerNodeRef(component) {
    this.controllerRef = component;
  },

  _setBoxRef(component) {
    this.boxRef = component;
  },

  /*
   Push against the box with an impulse force, at the onClicked location, and
   with a force direction originating from the controller (controller forward).
   */
  onBoxClicked(clickedPos, source){
    this.controllerRef.getControllerForwardAsync().then((forward)=>{
      var pushStrength = 1.5;
      var pushImpulse = [forward[0]*pushStrength, forward[1]*pushStrength, forward[2]*pushStrength];
      this.boxRef.getTransformAsync().then((transform) => {
        var pos = transform.position;
        var pushPosition = [clickedPos[0] - pos[0], clickedPos[1] - pos[1], clickedPos[2] - pos[2]];
        this.boxRef.applyImpulse(pushImpulse, pushPosition);
      });
    });
  },

  onBoxCollide(collidedTag, collidedPoint, collidedNormal){
    console.log("Viro box has collided on the " + collidedTag);
  }

});

ViroMaterials.createMaterials({
  box: {
      cullMode: "None",
      shininess: 2.0,
      diffuseTexture: require('./res/rocket.png'),
    },

  ground:{
    cullMode: "None",
    shininess: 2.0,
    diffuseColor: "#ff9999"
  }
 });

module.exports = BasicPhysicsSample;
