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
  ViroAnimatedComponent,
  ViroQuad,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var ReleaseMenu = require("./ReleaseMenu.js");

var ViroAnimatedComponentTest = createReactClass({

  getInitialState() {
    return {
      animationName:"loopRotate",
      playAnim:false,
      durationSlowDown:1,
      animationDelay:0,
      animationComponentDelay:0,
      animLoop:false,
      animationType:1,
    };
  },

  render: function() {
    var currentAnim;
    if ( this.state.animationType == 1){
        currentAnim = "sequentialAnim";
    } else if ( this.state.animationType == 2) {
        currentAnim = "loopRotate";
    } else {
        currentAnim = "parallelAnim";
    }

    return (
     <ViroScene>
     <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

     <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

     <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

      <ViroAnimatedComponent
      animation={currentAnim}
      delay={this.state.animationComponentDelay}
      run={this.state.playAnim}
      loop={this.state.animLoop}
        onStart={this._onStart}
        onFinish={this._onFinish}>
      <ViroSphere
        heightSegmentCount={20}
        widthSegmentCount={20}
        radius={.5}
        position={[-1, 0, -3]}
        materials={["sphere1Animated"]}
        facesOutward={false}
        />
      </ViroAnimatedComponent>

      <ViroNode position={[0,-2,-3]}>
               <ViroText style={styles.baseTextTwo}  position={[-2,1, 0]} width={2} height ={2}
                      text={"isPlaying: " + this.state.playAnim}
                      onClick={this._togglePlay}/>

               <ViroText style={styles.baseTextTwo}  position={[0,1, 0]} width={2} height ={2}
                      text={"Toggle Speed: " + this.state.durationSlowDown + "X slower"}
                      onClick={this._toggleSpeed}/>

               <ViroText style={styles.baseTextTwo}  position={[2,1, 0]} width={2} height ={2}
                      text={"IsLooping: " + this.state.animLoop}
                      onClick={this._toggleLoop}/>

               <ViroText style={styles.baseTextTwo}  position={[-2,0, 0]} width={2} height ={2}
                      text={"Animation Type: " + currentAnim}
                      onClick={this._toggleAnimationType}/>

               <ViroText style={styles.baseTextTwo}  position={[0,0, 0]} width={2} height ={2}
                      text={"AnimComponent Delay: " + this.state.animationComponentDelay}
                      onClick={this._toggleDelayComponent}/>

               <ViroText style={styles.baseTextTwo}  position={[2,0, 0]} width={2} height ={2}
                      text={"Animation Delay: " + this.state.animationDelay}
                      onClick={this._toggleDelayAnimation}/>
            </ViroNode>
     </ViroScene>

    );
  },

  _showNext() {
     this.props.sceneNavigator.replace({scene:require('./ViroBoxTest')});
  },

  _togglePlay(){
  this.setState({
              playAnim:!this.state.playAnim
             });
  },

  _toggleSpeed(){
    var newSpeed = this.state.durationSlowDown + 1.5;
    if (newSpeed > 6){
        newSpeed = 1;
    }
    this._updateAnimation(newSpeed, this.state.animationDelay);
    this.setState({
            durationSlowDown:newSpeed
           });
  },

  _toggleLoop(){
        this.setState({
                    animLoop:!this.state.animLoop
                   });
  },

  _toggleDelayComponent(){
    var newDelay = this.state.animationComponentDelay + 2000;
    if (newDelay > 6000){
        newDelay = 0;
    }

     this.setState({
            animationComponentDelay:newDelay
           });
  },

  _toggleDelayAnimation(){
    var newDelay = this.state.animationDelay + 2000;
    if (newDelay > 6000){
        newDelay = 0;
    }
    this._updateAnimation(this.state.durationSlowDown, newDelay);
    this.setState({
            animationDelay:newDelay
           });
  },

  _toggleAnimationType(){
    var newType = this.state.animationType + 1;
        if (newType > 3){
            newType = 1;
        }
        this.setState({
                animationType:newType
               });
  },

  _updateAnimation(slowDownMultiplier, animDelay){
    ViroAnimations.registerAnimations({
        moveRight:{properties:{positionX:"+=3",}, duration:500*slowDownMultiplier, delay:animDelay},
        moveLeft:{properties:{positionX:"-=3",}, duration:500*slowDownMultiplier, delay:animDelay},
        loopRotate:{properties:{rotateY:"+=180"}, duration:500*slowDownMultiplier},
        sequentialAnim:[
              ["moveRight","moveLeft"]
        ],
        parallelAnim:[
              ["moveRight","moveLeft"],
              ["loopRotate"]
        ]
      });

  },
  _onStart(){
    console.log("ViroAnimatedComponent onStart();");
  },
  _onFinish(){
    console.log("ViroAnimatedComponent onFinish();");
  }

});

ViroAnimations.registerAnimations({
  moveRight:{properties:{positionX:"+=3",}, duration:500, delay:0},
  moveLeft:{properties:{positionX:"-=3",}, duration:500, delay:0},
  loopRotate:{properties:{rotateY:"+=180"}, duration:500},
  sequentialAnim:[
        ["moveRight","moveLeft"]
  ],
  parallelAnim:[
        ["moveRight","moveLeft"],
        ["loopRotate"]
  ],
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
        textAlignVertical: 'center',
        textAlign: 'center',
    }
});

ViroMaterials.createMaterials({

   sphere1Animated: {
     shininess : 2.0,
     lightingModel: "Blinn",
     diffuseTexture: require('./res/360_waikiki.jpg'),
   },

});


module.exports = ViroAnimatedComponentTest;
