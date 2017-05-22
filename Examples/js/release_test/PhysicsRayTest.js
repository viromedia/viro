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
  ViroScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroUtils,
  ViroText,
  ViroSurface,
  ViroOmniLight,
} from 'react-viro';

var ReleaseMenu = require("./ReleaseMenu.js");
var PhysicsRayTest = React.createClass({
  onClickedBox(){
    this.refs["box"].applyForceImpulse([0,0,-300],[0,0,0]);
    //this.refs["box"].applyTorqueImpulse([0,0,-10]);
  },

  onCollidedBox(collidedTag, collidedPoint, collidedNormal){
    console.log("Viro onCollidedBox Javascript: (" + collidedTag + ")  collidedPoint:" + collidedPoint[0] +","+ collidedPoint [1]+"," + collidedPoint[2]);
  },

  shootRay(){
    // this.refs["scene1"].findCollisionsWithRayAsync([0,-9,0], [0,-9,-50.5], false, "testRay2").then((position) => {
    // console.log("Viro Finished"); }); // Should collide
    // this.refs["scene1"].findCollisionsWithShapeAsync([0,-9, -15.5], [0,-9, -15.5], "sphere", [2], "testSphereRay").then((collide) => {
    // console.log("Viro hasCollide? " + collide); });// should collide
    // this.refs["scene1"].findCollisionsWithShapeAsync([0,-9, -15.5], [0,-9, -15.5], "sphere", [0.1], "testSphereRay").then((collide) => {
    // console.log("Viro hasCollide? " + collide); }); // should not collide
    // this.refs["scene1"].findCollisionsWithShapeAsync([0,-9,0], [0,-9,-16.5], "sphere", [2], "testSphere2").then((collide) => {
    // console.log("Viro hasCollide? " + collide); }); // Should collide with closest
  },

  render: function() {
    return (
              <ViroScene ref={"scene1"}>
                <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
                <ViroNode position={[0 , -9, -8.5]}>
                <ViroBox
                    onClick={this.shootRay}
                    physicsBody={{
                      type:'static', restitution:0.8
                    }}
                    position={[0, -4, -14.5]}
                    scale={[1,1,1]}
                    materials={["box2"]}
                    rotation={[0,0,0]}
                    height={2}
                    width={40}
                    length={50}
                    ViroTag="GROUND"
                />

                  <ViroBox
                      ref="box"
                      onClick={this.onClickedBox}
                      position={[0, 0, -6]}
                      scale={[1, 1, 1]}
                      materials={["redColor","blue","redColor","blue","redColor","blue"]}
                      height={1} width={1} length={1}
                      physicsBody={{
                        type:'dynamic',
                        mass:1,
                        useGravity:false,
                        torque:[0,0,1]
                      }}
                      viroTag="box 1"
                      onCollided={this.onCollidedBox}
                      />

                      <ViroBox
                          ref="box2"
                          onClick={this.onClickedBox}
                          position={[0,0,-8]}
                          scale={[1, 1, 1]}
                          materials={["redColor","blue","redColor","blue","redColor","blue"]}
                          height={1} width={1} length={1}
                          physicsBody={{
                            type:'dynamic',
                            mass:1,
                            useGravity:false,
                            onCollided:this.onCollidedBox,
                            torque:[0,0,1],
                          }}
                          viroTag="box2"
                          onCollided={this.onCollidedBox}
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

ViroMaterials.createMaterials({
  redColor: {
    shininess: 2.0,
    diffuseColor: "#ff0000"
  },
  box2: {
    lightingModel: "Phong",
    shininess : 2.0,
    diffuseColor: "#3FBF3F",
  },
  blue: {
      cullMode: "None",
      shininess: 2.0,
      diffuseColor: "#8640ff22"

    },
 });

module.exports = PhysicsRayTest;
