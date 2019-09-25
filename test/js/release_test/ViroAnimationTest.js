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
  ViroButton,
  ViroUtils,
  ViroText,
  ViroAnimations,
  ViroQuad,
  ViroSpinner,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;
var LocalButtonImage = require("./res/icon_live.jpg");

var ReleaseMenu = require("./ReleaseMenu.js");

var ViroAnimationTest = createReactClass({

  getInitialState() {
    return {
      animationName:"loopRotate",
      playAnim:false,
      durationSlowDown:1,
      animationDelay:0,
      animationComponentDelay:0,
      animLoop:false,
      animationType:1,
      interruptible: false,
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

     <Viro3DObject source={require('./res/object_basketball.vrx')}
                   resources={[require('./res/object_basketball_diffuse.png'),
                               require('./res/object_basketball_normal.png'),
                               require('./res/object_basketball_specular.png')]}
                   position={[-1, 6, -3]}
                   rotation={[0, 0, 0]}
                   scale={[1, 1, 1]}
                   type="VRX"
                   animation={{name:currentAnim,
                               delay:this.state.animationComponentDelay,
                               loop:this.state.animLoop,
                               run:this.state.playAnim,
                               onStart:this._onStart,
                               onFinish:this._onFinish,
                               interruptible:this.state.interruptible }}
     />

     <ViroButton
         position={[-1, 5, -3]}
         materials={["sphere1_waikiki"]}
         scale={[0.8, 0.8, 1.0]}
         source={LocalButtonImage}
         hoverSource={LocalButtonImage}
         clickSource={LocalButtonImage}
         animation={{name:currentAnim,
                     delay:this.state.animationComponentDelay,
                     loop:this.state.animLoop,
                     run:this.state.playAnim,
                     onStart:this._onStart,
                     onFinish:this._onFinish,
                     interruptible:this.state.interruptible }}/>

     <ViroImage width={1} height={1}
                format="RGBA8" mipmap={true}
                position={[-1, 4, -3]}
                materials={["sphere1_waikiki"]}
                scale={[0.5, 0.5, 0.1]}
                source={{uri: "https://upload.wikimedia.org/wikipedia/commons/7/74/Earth_poster_large.jpg"}}
                animation={{name:currentAnim,
                            delay:this.state.animationComponentDelay,
                            loop:this.state.animLoop,
                            run:this.state.playAnim,
                            onStart:this._onStart,
                            onFinish:this._onFinish,
                            interruptible:this.state.interruptible }}/>

     <ViroSpinner materials={["sphere1_waikiki"]}
                  position={[-1, 3, -3]}
                  scale={[0.9, 0.9, 0.3]}
                  animation={{name:currentAnim,
                              delay:this.state.animationComponentDelay,
                              loop:this.state.animLoop,
                              run:this.state.playAnim,
                              onStart:this._onStart,
                              onFinish:this._onFinish,
                              interruptible:this.state.interruptible}} />

     <ViroQuad position={[-1, 2, -3]}
                  scale={[0.5, 0.5, 0.1]}
                  materials={["sphere1_waikiki"]}
                  width={1}
                  height={1}
                  animation={{name:currentAnim,
                              delay:this.state.animationComponentDelay,
                              loop:this.state.animLoop,
                              run:this.state.playAnim,
                              onStart:this._onStart,
                              onFinish:this._onFinish,
                              interruptible:this.state.interruptible}} />

     <ViroText fontSize={24}
               text={"Sample Text"}
               position={[-1, 1, -3]}
               animation={{name:currentAnim,
                           delay:this.state.animationComponentDelay,
                           loop:this.state.animLoop,
                           run:this.state.playAnim,
                           onStart:this._onStart,
                           onFinish:this._onFinish,
                           interruptible:this.state.interruptible}}/>

     <ViroBox
        width={1} height={1}
        position={[-1, 0, -3]}
        materials={["sphere1_waikiki"]}
        animation={{name:currentAnim,
                    delay:this.state.animationComponentDelay,
                    loop:this.state.animLoop,
                    run:this.state.playAnim,
                    onStart:this._onStart,
                    onFinish:this._onFinish,
                    interruptible:this.state.interruptible}}
      />

     <ViroSphere
        heightSegmentCount={20}
        widthSegmentCount={20}
        radius={.5}
        position={[-1, -1, -3]}
        materials={["sphere1_waikiki"]}
        facesOutward={false}
        animation={{name:currentAnim,
                    delay:this.state.animationComponentDelay,
                    loop:this.state.animLoop,
                    run:this.state.playAnim,
                    onStart:this._onStart,
                    onFinish:this._onFinish,
                    interruptible:this.state.interruptible}}
        />

      <ViroNode position={[0,-3,-3]}>
              <ViroText style={styles.baseTextTwo}  position={[-2,-1, 0]} width={2} height ={2}
                    text={"Interruptible: "+ this.state.interruptible}
                    onClick={this._toggleInterruptible}/>

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

  _toggleInterruptible() {
    this.setState({
        interruptible: !this.state.interruptible,
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
    console.log("Animation onStart();");
  },
  _onFinish(){
    console.log("Animation onFinish();");
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

   sphere1_waikiki: {
     shininess : 2.0,
     lightingModel: "Blinn",
     diffuseTexture: require('./res/360_waikiki.jpg'),
   },

});


module.exports = ViroAnimationTest;
