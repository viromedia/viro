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
  ViroOmniLight,
  ViroAnimations,
  ViroDirectionalLight,
  ViroController,
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


var GroupTestDragEvents = createReactClass({
  getInitialState() {
    return {
        transformFlag: 1,
        transformBehaviors:["billboard"],
        currentDragType:"FixedDistance"
    };
  },
onHover(objectTag){return (source, isHovering)  => {
                         console.log("GroupTest: " + objectTag+ " onHover: " + isHovering);
                       }},
onClick(objectTag){return (source, objectTag) => {
                         console.log("GroupTest: " + objectTag+ " onClick");
                       }},
onClickState(objectTag){return (source, state, objectTag) => {
                              console.log("GroupTest: " + objectTag+ " onClickState: " + state);
                            }},
onTouch(objectTag){return (source, touchState, touchPos, objectTag) => {
                         console.log("GroupTest: " + objectTag+ " onTouch touchState " + touchState + ",  touchpos: " + touchPos[0] +"," + touchPos[1]);
                       }},
onScroll(objectTag){return (source, scrollPos, objectTag) => {
                          console.log("GroupTest: " + objectTag+ " onScroll scrollPos " + scrollPos[0] + "," + scrollPos[1]);
                        }},
onSwipe(objectTag){return (source, swipeState, objectTag) => {
                         console.log("GroupTest: " + objectTag+ " onSwipe swipeState" + swipeState);
                       }},
onDrag(objectTag){return (dragtoPos, source) => {
                        console.log("GroupTest: " + objectTag+ " onDrag dragtoPos" +
                        dragtoPos[0] +","+ dragtoPos[1]+","+ dragtoPos[2]);
                      }},
  render: function() {
    return (
                <ViroScene>

                <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />
                <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>


                <ViroText width={2} height={1} position={[0.8, -1.75, -3.5]}
                          text={"Toggle DragType" + this.state.currentDragType} onClick={this._toggleDragType} />

                <ViroNode position={[0.8 , 0, -3.5]} >


                    <Viro3DObject source={require('../res/heart.obj')}
                             scale={[1.8, 1.8, 1.8]}
                              position={[-3.2, 2.5, -4.5]}
                              materials={["heart"]}
                              type="OBJ"
                              dragType={this.state.currentDragType}
                              onDrag={this.onDrag("3dObject")}/>

                  <ViroBox
                      position={[-1, 1, 0]}
                      scale={[0.4, 0.4, 0.4]}
                      materials={["redColor","blue","redColor","blue","redColor","blue"]}
                      height={1}
                      width={1}
                      length={1}
                      dragType={this.state.currentDragType}
                      onDrag={this.onDrag("ViroBox")}
                      />

                  <ViroButton
                      position={[0, 1, 0]}
                      scale={[0.08, 0.08, 0.1]}
                      source={LocalButtonImage}
                      hoverSource={LocalButtonImage}
                      clickSource={LocalButtonImage}
                      dragType={this.state.currentDragType}
                      onDrag={this.onDrag("ViroButton")}
                      />

                  <ViroFlexView
                      position={[1, 1, 0]}
                      scale={[0.2, 0.2, 0.1]}
                      materials={["redColor"]}
                      width={3}
                      height={2}
                      dragType={this.state.currentDragType}
                      onDrag={this.onDrag("ViroFlexView")}/>
                  <ViroImage
                      width={1} height={1}
                      format="RGBA8" mipmap={true}
                      position={[-2, 0, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      source={{uri: "https://upload.wikimedia.org/wikipedia/commons/7/74/Earth_poster_large.jpg"}}
                      dragType={this.state.currentDragType}
                      onDrag={this.onDrag("ViroImage")}/>
                  <ViroNode
                    position={[-1, 0, 0]}
                    scale={[0.5, 0.5, 0.1]}
                    rotation={[0,0,0]}
                    dragType={this.state.currentDragType}
                      onDrag={this.onDrag("ViroNode")}>
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
                      dragType={this.state.currentDragType}
                      onDrag={this.onDrag("ViroSphere")}
                      />

                  <ViroSpinner
                      position={[1, 0, 0]}
                      scale={[0.3, 0.3, 0.1]}
                      dragType={this.state.currentDragType}
                      onDrag={this.onDrag("ViroSpinner")}/>

                  <ViroQuad
                      position={[-2, -1, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      materials={["redColor"]}
                      width={1}
                      height={1}
                      dragType={this.state.currentDragType}
                         onDrag={this.onDrag("ViroQuad")}/>

                  <ViroText
                      position={[-1, -1, 0]}
                      scale={[0.5 , 0.5, 0.1]}
                      style={styles.baseTextTwo}
                      text="This is a Viro Text"
                      dragType={this.state.currentDragType}
                         onDrag={this.onDrag("ViroText")}/>
                  <ViroVideo
                      position={[0 , -1,0]}
                      scale={[0.1, 0.1, 0.1]}
                      height={4} width={4}
                      onDrag={this.onDrag("ViroVideo")}
                      dragType={this.state.currentDragType}
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

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./ViroVideoTest')});
  },


    _toggleDragType() {
      var type = this.state.currentDragType == "FixedDistance" ? "FixedDistanceOrigin" : "FixedDistance";
      this.setState({currentDragType:type});
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

module.exports = GroupTestDragEvents;
