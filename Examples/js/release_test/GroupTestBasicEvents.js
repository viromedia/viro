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
  ViroAnimatedComponent,
  ViroScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroImage,
  ViroVideo,
  ViroFlexView,
  ViroUtils,
  ViroText,
  ViroSurface,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
  ViroButton,
  ViroSpinner,
  ViroOmniLight,
  ViroAnimations,
  ViroDirectionalLight
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;

var UriImage = {uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/360_horseshoe.jpg"};
var LocalImage = require("./res/360_park.jpg");

var Uri360Video = {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf.mp4"};
var Local360Video = require("./res/360Asteroids.mp4");

var LocalButtonImage = require("./res/icon_live.jpg");

var TOGGLE_PLAY ="Toggle Play";
var TOGGLE_PAUSE ="Toggle Pause";


var GroupTestBasicEvents = React.createClass({
  getInitialState() {
    return {
        transformFlag: 1,
        transformBehaviors:["billboard"]
    };
  },
onHover(objectTag){return (source, isHovering)  => {
                         console.log("GroupTest: " + objectTag+ " onHover: " + isHovering);
                       }},
onClick(objectTag){return (source) => {
                         console.log("GroupTest: " + objectTag+ " onClick");
                       }},
onClickState(objectTag){return (source, state) => {
                              console.log("GroupTest: " + objectTag+ " onClickState: " + state);
                            }},
onTouch(objectTag){return (source, touchState, touchPos) => {
                         console.log("GroupTest: " + objectTag+ " onTouch touchState " + touchState + ",  touchpos: " + touchPos[0] +"," + touchPos[1]);
                       }},
onScroll(objectTag){return (source, scrollPos) => {
                          console.log("GroupTest: " + objectTag+ " onScroll scrollPos " + scrollPos[0] + "," + scrollPos[1]);
                        }},
onSwipe(objectTag){return (source, swipeState) => {
                         console.log("GroupTest: " + objectTag+ " onSwipe swipeState" + swipeState);
                       }},
onDrag(objectTag){return (source, dragtoPos) => {
                        console.log("GroupTest: " + objectTag+ " onDrag dragtoPos" +
                        dragtoPos[0] +","+ dragtoPos[1]+","+ dragtoPos[2]);
                      }},
  render: function() {
    return (
                <ViroScene onHover={this.onHover("ViroScene")}
                                                 onClick={this.onClick("ViroScene")}
                                                 onClickState={this.onClickState("ViroScene")}
                                                 onTouch={this.onTouch("ViroScene")}
                                                 onScroll={this.onScroll("ViroScene")}
                                                 onSwipe={this.onSwipe("ViroScene")}>

                <ViroNode position={[0.8 , 0, -3.5]} >


                    <Viro3DObject source={require('../res/heart.obj')}
                             scale={[1.8 , 1.8  , 1.8]}
                              position={[-2.5 , -4.3 , -1.15]}
                              materials={["heart"]}
                              onHover={this.onHover("3dObject")}
                              onClick={this.onClick("3dObject")}
                              onClickState={this.onClickState("3dObject")}
                              onTouch={this.onTouch("3dObject")}
                              onScroll={this.onScroll("3dObject")}
                              onSwipe={this.onSwipe("3dObject")}/>

                  <ViroBox
                      position={[-1 , 1, 0]}
                      scale={[0.4 , 0.4 , 0.4]}
                      materials={["redColor","blue","redColor","blue","redColor","blue"]}
                      height={1}
                      width={1}
                      length={1}
                      onHover={this.onHover("ViroBox")}
                      onClick={this.onClick("ViroBox")}
                      onClickState={this.onClickState("ViroBox")}
                      onTouch={this.onTouch("ViroBox")}
                      onScroll={this.onScroll("ViroBox")}
                      onSwipe={this.onSwipe("ViroBox")}
                      />

                  <ViroButton
                      position={[0, 1, 0]}
                      scale={[0.08, 0.08, 0.1]}
                      source={LocalButtonImage}
                      gazeSource={LocalButtonImage}
                      tapSource={LocalButtonImage}
                      onHover={this.onHover("ViroButton")}
                      onClick={this.onClick("ViroButton")}
                      onClickState={this.onClickState("ViroButton")}
                      onTouch={this.onTouch("ViroButton")}
                      onScroll={this.onScroll("ViroButton")}
                      onSwipe={this.onSwipe("ViroButton")}
                      />

                  <ViroFlexView
                      position={[1, 1, 0]}
                      scale={[0.2, 0.2, 0.1]}
                      materials={["redColor"]}
                      width={3}
                      height={2}
                      onHover={this.onHover("ViroFlexView")}
                      onClick={this.onClick("ViroFlexView")}
                      onClickState={this.onClickState("ViroFlexView")}
                      onTouch={this.onTouch("ViroFlexView")}
                      onScroll={this.onScroll("ViroFlexView")}
                      onSwipe={this.onSwipe("ViroFlexView")}/>
                  <ViroImage
                      width={1} height={1}
                      format="RGBA8" mipmap={true}
                      position={[-2, 0, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      source={{uri: "https://upload.wikimedia.org/wikipedia/commons/7/74/Earth_poster_large.jpg"}}
                      onHover={this.onHover("ViroImage")}
                      onClick={this.onClick("ViroImage")}
                      onClickState={this.onClickState("ViroImage")}
                      onTouch={this.onTouch("ViroImage")}
                      onScroll={this.onScroll("ViroImage")}
                      onSwipe={this.onSwipe("ViroImage")}/>
                  <ViroNode
                    position={[-1, 0, 0]}
                    scale={[0.5, 0.5, 0.1]}
                    rotation={[0,0,0]}
                      onHover={this.onHover("ViroNode")}
                      onClick={this.onClick("ViroNode")}
                      onClickState={this.onClickState("ViroNode")}
                      onTouch={this.onTouch("ViroNode")}
                      onScroll={this.onScroll("ViroNode")}
                      onSwipe={this.onSwipe("ViroNode")}>
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
                      materials={["redColor"]}
                      onHover={this.onHover("ViroSphere")}
                      onClick={this.onClick("ViroSphere")}
                      onClickState={this.onClickState("ViroSphere")}
                      onTouch={this.onTouch("ViroSphere")}
                      onScroll={this.onScroll("ViroSphere")}
                      onSwipe={this.onSwipe("ViroSphere")}
                      />

                  <ViroSpinner
                      position={[1, 0, 0]}
                      scale={[0.3, 0.3, 0.1]}
                      onHover={this.onHover("ViroSpinner")}
                      onClick={this.onClick("ViroSpinner")}
                      onClickState={this.onClickState("ViroSpinner")}
                      onTouch={this.onTouch("ViroSpinner")}
                      onScroll={this.onScroll("ViroSpinner")}
                      onSwipe={this.onSwipe("ViroSpinner")}/>

                  <ViroSurface
                      position={[-2, -1, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      materials={["redColor"]}
                      width={1}
                      height={1}
                         onHover={this.onHover("ViroSurface")}
                         onClick={this.onClick("ViroSurface")}
                         onClickState={this.onClickState("ViroSurface")}
                         onTouch={this.onTouch("ViroSurface")}
                         onScroll={this.onScroll("ViroSurface")}
                         onSwipe={this.onSwipe("ViroSurface")}/>

                  <ViroText
                      position={[-1, -1, 0]}
                      scale={[0.5 , 0.5, 0.1]}
                      style={styles.baseTextTwo}
                      text="This is a Viro Text"
                         onHover={this.onHover("ViroText")}
                         onClick={this.onClick("ViroText")}
                         onClickState={this.onClickState("ViroText")}
                         onTouch={this.onTouch("ViroText")}
                         onScroll={this.onScroll("ViroText")}
                         onSwipe={this.onSwipe("ViroText")}/>
                  <ViroVideo
                      position={[0 , -1,0]}
                      scale={[0.1, 0.1, 0.1]}
                      height={4} width={4}
                      onHover={this.onHover("ViroVideo")}
                      onClick={this.onClick("ViroVideo")}
                      onClickState={this.onClickState("ViroVideo")}
                      onTouch={this.onTouch("ViroVideo")}
                      onScroll={this.onScroll("ViroVideo")}
                      onSwipe={this.onSwipe("ViroVideo")}
                      source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}} />
                </ViroNode>

                   <ViroOmniLight
                                     position={[0, 0, 0]}
                                     color={"#ffffff"}
                                     attenuationStartDistance={30}
                                     attenuationEndDistance={40}/>

            </ViroScene>

    );
  },

  _toggleTransform(){
    var newtransformflag = this.state.transformFlag + 1;
    if (newtransformflag > 5){
        newtransformflag = 0;
    }
     this.setState({
            transformFlag:newtransformflag
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
  redColor: {
  fresnelExponent: .5,
   shininess: 2.0,
    diffuseColor: "#ff0000"
  },
  blue: {
      shininess: 2.0,
      lightingModel: "Lambert",
      diffuseColor: "#0000ff"
    },
  heart: {
      lightingModel: "Phong",
      diffuseTexture: require('../res/heart_d.jpg'),
    },
 });

module.exports = GroupTestBasicEvents;
