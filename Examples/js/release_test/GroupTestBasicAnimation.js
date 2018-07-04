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
  ViroVideo,
  ViroFlexView,
  ViroUtils,
  ViroText,
  ViroQuad,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
  ViroButton,
  ViroSpinner,
  ViroAnimations,
  ViroDirectionalLight
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var UriImage = {uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/360_horseshoe.jpg"};
var LocalImage = require("./res/360_park.jpg");

var Uri360Video = {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf.mp4"};
var Local360Video = require("./res/360Asteroids.mp4");

var LocalButtonImage = require("./res/icon_live.jpg");

var ReleaseMenu = require("./ReleaseMenu.js");

var TOGGLE_PLAY ="Toggle Play";
var TOGGLE_PAUSE ="Toggle Pause";


var GroupTestBasicAnimation = createReactClass({
  getInitialState() {
    return {
        loopText:"Loop off",
        playText:"Play",
        delayOn:0,
        willLoop:false,
        runAnimation:false
    };
  },

  render: function() {
    return (
            <ViroScene>
            <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

            <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

                <ViroNode position={[0.8 , 0, -3.5]}>

                  <Viro3DObject source={require('../res/heart.obj')}
                              scale={[1.8, 1.8, 1.8]}
                              position={[-3.2, 2.5, -4.5]}
                              materials={["heart"]}
                              type="OBJ"
                              onClick={this._elementClick(10)}
                              animation={{name:"testLoopRotate",
                                          loop:this.state.willLoop,
                                          run:this.state.runAnimation }} />



                  <ViroBox
                      position={[-1, 1, 0]}
                      scale={[0.4, 0.4, 0.4]}
                      rotation={[40, 45, 0]}
                      materials={["redColor", "blueColor","redColor", "blueColor","redColor", "blueColor"]}
                      height={1}
                      width={1}
                      length={1}
                       onClick={this._elementClick(0)}
                       animation={{name:"testLoopMove",
                                   loop:this.state.willLoop,
                                   run:this.state.runAnimation }}/>

                  <ViroButton
                      position={[0, 1, 0]}
                      scale={[0.08, 0.08, 0.1]}
                      source={LocalButtonImage}
                      hoverSource={LocalButtonImage}
                      clickSource={LocalButtonImage}
                      onClick={this._elementClick(1)}
                      animation={{name:"testLoopScale",
                                  loop:this.state.willLoop,
                                  run:this.state.runAnimation }}
                      />

                  <ViroFlexView
                      position={[1, 1, 0]}
                      scale={[0.2, 0.2, 0.1]}
                      materials={["redColor"]}
                      width={3}
                      height={2}
                      onClick={this._elementClick(2)}
                      animation={{name:"testLoopOpacity",
                                  loop:this.state.willLoop,
                                  run:this.state.runAnimation }}/>


                  <ViroImage
                      width={1} height={1}
                      format="RGBA8" mipmap={true}
                      position={[-2, 0, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      onClick={this._elementClick(3)}
                      animation={{name:"testLoopRotate",
                                  loop:this.state.willLoop,
                                  run:this.state.runAnimation }}
                      source={{uri: "https://upload.wikimedia.org/wikipedia/commons/7/74/Earth_poster_large.jpg"}}
                      />


                  <ViroNode
                    position={[-1, 0, 0]}
                    scale={[0.5, 0.5, 0.1]}
                    onClick={this._elementClick(4)}
                    rotation={[0,0,0]}
                          animation={{name:"testLoopMove",
                                      loop:this.state.willLoop,
                                      run:this.state.runAnimation }}>
                    <ViroText
                        style={styles.baseTextTwo}
                        text="This is a text in a ViroNode" />
                  </ViroNode>

                  <ViroSphere
                      position={[0, 0, 0]}
                      scale={[0.3, 0.3, 0.3]}
                      widthSegmentCount={5}
                      heightSegmentCount={5}
                      radius={1}
                      onClick={this._elementClick(5)}
                      materials={["redColor"]}
                      animation={{name:"testLoopScale",
                                  loop:this.state.willLoop,
                                  run:this.state.runAnimation }}
                      />



                  <ViroSpinner
                      position={[1, 0, 0]}
                      scale={[0.3, 0.3, 0.1]}
                      onClick={this._elementClick(6)}
                      animation={{name:"testLoopOpacity",
                                  loop:this.state.willLoop,
                                  run:this.state.runAnimation }}/>


                  <ViroQuad
                      position={[-2, -1, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      materials={["redColor"]}
                      width={1}
                      onClick={this._elementClick(7)}
                      height={1}
                      animation={{name:"testLoopRotate",
                                  loop:this.state.willLoop,
                                  run:this.state.runAnimation }}/>


                  <ViroText
                      position={[-1, -1, 0]}
                      scale={[0.5 , 0.5, 0.1]}
                      style={styles.baseTextTwo}
                      onClick={this._elementClick(8)}
                      text="This is a Viro Text"
                      animation={{name:"testLoopMove",
                                  loop:this.state.willLoop,
                                  run:this.state.runAnimation }}/>


                  <ViroVideo
                      position={[0 , -1,0]}
                      scale={[0.1, 0.1, 0.1]}
                      height={4} width={4}
                      onClick={this._elementClick(9)}
                      source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}}
                      animation={{name:"testLoopScale",
                                  loop:this.state.willLoop,
                                  run:this.state.runAnimation }}/>

                 <ViroText
                      visible={this.state.isPlayVisible}
                      position={[-1.5 , -2, 0]}
                      scale={[1, 1, 0.1]}
                      style={styles.baseTextTwo}
                      text={this.state.playText}
                      onClick={this._togglePlay}/>

                 <ViroText
                      visible={this.state.isPlayVisible}
                       position={[0 , -2, 0]}
                       scale={[1, 1, 0.1]}
                       style={styles.baseTextTwo}
                       text={this.state.loopText}
                       onClick={this._toggleLoop}/>
                </ViroNode>
            </ViroScene>

    );
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./GroupTestBasicBillboard')});
  },

  _elementClick(item){
    return () => {
        //No-op
    }
  },

    _togglePlay(){
        var newRunAnimation = !this.state.runAnimation;
        var newPlayText = "Pause";
        if (!newRunAnimation){
            newPlayText = "Play";
        }
       this.setState({
        playText:newPlayText,
        runAnimation:newRunAnimation
       });
    },

  _toggleLoop(){
    var loopText = "Loop On";
    if (this.state.willLoop){
        loopText = "Loop Off";
    }
     this.setState({
             loopText:loopText,
             willLoop:!this.state.willLoop
         });
  }
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



ViroMaterials.createMaterials({
  sunTexture: {
    diffuseTexture: require("../res/sun_2302.jpg"),
  },
  redColor: {
    diffuseColor: "#ff0000"
  },
  blueColor: {
    diffuseColor: "#0000ff"
  },
  heart: {
      lightingModel: "Constant",
      diffuseTexture: require('../res/heart_d.jpg'),
    },
 });



ViroAnimations.registerAnimations({
    testLoopRotate:{properties:{rotateZ:"+=45"}, duration:2000, easing:"EaseInEaseOut"},

    moveRight:{properties:{positionX:"+=0.3"}, duration: 2000, easing:"Linear"},
    moveLeft:{properties:{positionX:"-=0.3"}, duration: 2000, easing:"Linear"},
    testLoopMove:[
        ["moveRight", "moveLeft"]
    ],

    scaleUp:{properties:{scaleX:"+=0.2"}, duration: 2000, easing:"Bounce"},
    scaleDown:{properties:{scaleX:"-=0.2"}, duration: 2000, easing:"Bounce"},
    testLoopScale:[
        ["scaleUp", "scaleDown"]
    ],
    opacityUp:{properties:{opacity: 1}, duration: 2000, easing:"EaseIn"},
    opacityDown:{properties:{opacity: 0}, duration: 2000, easing:"EaseOut"},
        testLoopOpacity:[
            ["opacityDown", "opacityUp"]
        ],
});



module.exports = GroupTestBasicAnimation;
