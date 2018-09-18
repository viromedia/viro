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
  ViroImage,
  ViroFlexView,
  ViroUtils,
  ViroText,
  ViroQuad,
  ViroSphere,
  ViroOmniLight,
  ViroAnimations,
  ViroDirectionalLight,
  ViroController,
} from 'react-viro';

var createReactClass = require('create-react-class');

var LocalButtonImage = require("./res/icon_live.jpg");
var ReleaseMenu = require("./ReleaseMenu.js");

var PhysicsScaleTest = createReactClass({
  getInitialState() {
    return {
      physicsEnabled:true,
      scale:1,
      rotation:0,
      trans:0,
      dimension: 0.5
    };
  },

  togglePhysicsBody(tag){
    return () => {
        console.log("Click numeric tag: " + tag);
        if (tag == 1){
          var scale = this.state.scale  + 0.2;
          if (scale > 2){
            scale = 0.2;
          }

          this.setState({
            scale:scale
          });
        } else if (tag ==2){
          var rotation = this.state.rotation + 10;
          if (rotation >= 360){
            rotation = 0;
          }

          this.setState({
            rotation:rotation
          });
        } else if (tag ==3){
          var trans = this.state.trans + 0.2;
          if (trans >= 1.5){
            trans = 0;
          }

          this.setState({
            trans:trans
          });
        } else if (tag ==4){
          var dimension = this.state.dimension + 0.2;
          if (dimension >= 1.5){
            dimension = 0.5;
          }


          this.setState({
            dimension:dimension
          });
        }
    }
  },


  render: function() {
    return (
                <ViroScene physicsWorld={{gravity:[0,-9.81,0], drawBounds:true}}>
                <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

                <ViroNode position={[1 , -2, -3]} transformBehaviors={["billboard"]}>
                  <ViroText fontSize={25}  style={styles.centeredText}
                    position={[-2, 0, 0]} width={4} height ={2} maxLines={3}
                    text={"Toggle Scale"} onClick={this.togglePhysicsBody(1)}/>

                  <ViroText fontSize={25}  style={styles.centeredText}
                      position={[0,0, 0]} width={4} height ={2} maxLines={3}
                      text={"Toggle Rotation"} onClick={this.togglePhysicsBody(2)}/>

                  <ViroText fontSize={25}  style={styles.centeredText}
                      position={[2,0, 0]} width={4} height ={2} maxLines={3}
                      text={"Toggle Translation "} onClick={this.togglePhysicsBody(3)}/>
                </ViroNode>


                <ViroNode position={[1 , 0, -3.5]} >
                  <ViroBox
                      rotation={[this.state.rotation, this.state.rotation, this.state.rotation]}
                      position={[this.state.trans -2, 0, 0]}
                      scale={[this.state.scale, this.state.scale, this.state.scale]}
                      materials={["blue","blue","blue","blue","blue","blue"]}
                      height={this.state.dimension}
                      width={this.state.dimension}
                      length={this.state.dimension}
                      physicsBody={this.state.physicsEnabled?
                        {type:'Dynamic', mass:1, enabled:true, useGravity:false}:undefined}
                      />

                  <ViroSphere
                      rotation={[this.state.rotation, this.state.rotation, this.state.rotation]}
                      position={[this.state.trans, 0, 0]}
                      scale={[this.state.scale, this.state.scale, this.state.scale]}
                      widthSegmentCount={5}
                      heightSegmentCount={5}
                      radius={this.state.dimension}
                            materials={["blue"]}
                      physicsBody={this.state.physicsEnabled?
                              {type:'Dynamic', mass:1, enabled:true, useGravity:false}:undefined}

                      />

                  <ViroQuad
                    rotation={[this.state.rotation, this.state.rotation, this.state.rotation]}
                    physicsBody={this.state.physicsEnabled?
                      {type:'Dynamic', mass:1, enabled:true, useGravity:false}:undefined}
                    position={[this.state.trans + 2, 0, 0]}
                      scale={[this.state.scale, this.state.scale, 0.1]}
                      materials={["blue"]}
                      width={this.state.dimension}
                      height={this.state.dimension}/>
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
       flex: 1,
  },

});



ViroMaterials.createMaterials({
  redColor: {
    shininess: 2.0,
    diffuseColor: "#ff0000"
  },
  box2: {
    shininess : 2.0,
    diffuseColor: "#F0F099",
  },
  blue: {
      shininess: 2.0,
      diffuseColor: "#0000ff"
    },
 });

module.exports = PhysicsScaleTest;
