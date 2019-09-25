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

let polarToCartesian = ViroUtils.polarToCartesian;
var LocalButtonImage = require("./res/icon_live.jpg");
var ReleaseMenu = require("./ReleaseMenu.js");

var createReactClass = require('create-react-class');

var GroupTestBasicEvents = createReactClass({
  getInitialState() {
    return {
        reticleVisibility:true,
        transformFlag: 1,
        transformBehaviors:["billboard"],
        showImage:true
    };
  },
  onHover(objectTag) {
    return (isHovering, source)  => {
      console.log("GroupTest: " + objectTag + " isHovering" + isHovering+ " onHover: " + source);
    }
  },
  onClick(objectTag) {
    return (source) => {
      console.log("GroupTest: " + objectTag + " onClick source:" + source);
    }
  },
  onClickState(objectTag) {
    return (state, source) => {
      console.log("GroupTest: " + objectTag + " source " + source+ " onClickState: " + state);
    }
  },
  onTouch(objectTag){
    return (touchState, touchPos, source) => {
      console.log("GroupTest:" + objectTag + " onTouch touchState " + touchState + ",  touchpos: " + touchPos[0] +"," + touchPos[1] + " source: " + source);
    }
  },
  onScroll(objectTag){
    return (scrollPos, source) => {
        console.log("GroupTest:" + objectTag + " onScroll scrollPos " + scrollPos[0] + "," + scrollPos[1] + " source:" + source);
    }
  },
  onSwipe(objectTag){
    return (swipeState, source) => {
        console.log("GroupTest:" + objectTag + " onSwipe swipeState" + swipeState + " source: " + source);
    }
  },
  onDrag(objectTag){
    return (dragtoPos, source) => {
      console.log("GroupTest:" + objectTag + " onDrag dragtoPos" +
      dragtoPos[0] +","+ dragtoPos[1]+","+ dragtoPos[2] + " source: " + source);
    }
  },
  onFuse(objectTag){
    return (source) => {
       console.log("GroupTest:" + objectTag + " onFuse");
    }
  },
  _getImage() {
    if (this.state.showImage) {
      return (
        <ViroImage position={[0.0, 2, -3.5]}
          transformBehaviors={["billboard"]}
          source={{uri: "https://images.sftcdn.net/images/t_optimized,f_auto/p/50d2f4ce-96d1-11e6-8a53-00163ed833e7/3980672443/google-earth-screenshot.jpg"}}
          onFuse={()=>{this.setState({showImage : false})}} />
      );
    } else {
      return;
    }
  },

  toggleReticle(){
    this.setState({reticleVisibility:!this.state.reticleVisibility});
  },

  render: function() {
    return (
            <ViroScene onHover={this.onHover("ViroScene")}
                       onClick={this.onClick("ViroScene")}
                       onClickState={this.onClickState("ViroScene")}
                       onTouch={this.onTouch("ViroScene")}
                       onScroll={this.onScroll("ViroScene")}
                       onFuse={this.onFuse("ViroScene")}
                       onSwipe={this.onSwipe("ViroScene")}>

              <ViroController
                    onClick={this.onClick("ViroController")}
                    onClickState={this.onClickState("ViroController")}
                    onTouch={this.onTouch("ViroController")}
                    onScroll={this.onScroll("ViroController")}
                    onFuse={this.onFuse("ViroController")}
                    onSwipe={this.onSwipe("ViroController")}
                    reticleVisibility={this.state.reticleVisibility} />

              <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />
              <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

              {this._getImage()}

              <ViroOmniLight position={[0, 0, 0]}
                             color={"#FFFFFF"}
                             attenuationStartDistance={30}
                             attenuationEndDistance={40}/>

              <ViroNode position={[0.8, 0, -3.5]}>
                  <Viro3DObject source={require('../res/heart.obj')}
                                scale={[1.8, 1.8, 1.8]}
                                position={[-3.2, 2.5, -4.5]}
                                materials={["heart"]}
                                type="OBJ"
                                onHover={this.onHover("3dObject")}
                                onClick={this.onClick("3dObject")}
                                onFuse={{callback:this.onFuse("3dObject"), timeToFuse:1000}}
                                onClickState={this.onClickState("3dObject")}
                                onTouch={this.onTouch("3dObject")}
                                onScroll={this.onScroll("3dObject")}
                                onSwipe={this.onSwipe("3dObject")}/>

                  <ViroBox
                      position={[-1, 1, 0]}
                      scale={[0.4 , 0.4 , 0.4]}
                      materials={["redColor","blue","redColor","blue","redColor","blue"]}
                      height={1}
                      width={1}
                      length={1}
                      onHover={this.onHover("ViroBox")}
                      onClick={this.onClick("ViroBox")}
                      onFuse={{callback:this.onFuse("ViroBox"), timeToFuse:1000}}
                      onClickState={this.onClickState("ViroBox")}
                      onTouch={this.onTouch("ViroBox")}
                      onScroll={this.onScroll("ViroBox")}
                      onSwipe={this.onSwipe("ViroBox")}/>

                  <ViroButton
                      position={[0, 1, 0]}
                      scale={[0.08, 0.08, 0.1]}
                      source={LocalButtonImage}
                      hoverSource={LocalButtonImage}
                      clickSource={LocalButtonImage}
                      onHover={this.onHover("ViroButton")}
                      onClick={this.onClick("ViroButton")}
                      onFuse={this.onFuse("ViroButton")}
                      onClickState={this.onClickState("ViroButton")}
                      onTouch={this.onTouch("ViroButton")}
                      onScroll={this.onScroll("ViroButton")}
                      onSwipe={this.onSwipe("ViroButton")}/>

                  <ViroFlexView
                      position={[1, 1, 0]}
                      scale={[0.2, 0.2, 0.1]}
                      materials={["redColor"]}
                      width={3}
                      height={2}
                      onHover={this.onHover("ViroFlexView")}
                      onClick={this.onClick("ViroFlexView")}
                      onFuse={this.onFuse("ViroFlexView")}
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
                      onFuse={{callback:this.onFuse("ViroImage"), timeToFuse:1000}}
                      onTouch={this.onTouch("ViroImage")}
                      onScroll={this.onScroll("ViroImage")}
                      onSwipe={this.onSwipe("ViroImage")}/>

                  <ViroNode
                      position={[-1, 0, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      rotation={[0,0,0]}
                      onHover={this.onHover("ViroNode")}
                      onClick={this.onClick("ViroNode")}
                      onFuse={{callback:this.onFuse("ViroImage"), timeToFuse:1000}}
                      onClickState={this.onClickState("ViroNode")}
                      onTouch={this.onTouch("ViroNode")}
                      onScroll={this.onScroll("ViroNode")}
                      onSwipe={this.onSwipe("ViroNode")}>

                      <ViroText
                        style={styles.baseTextTwo}
                        text="This is a text in a ViroNode"/>
                  </ViroNode>

                  <ViroSphere
                      position={[0, 0, 0]}
                      scale={[0.3, 0.3, 0.3]}
                      widthSegmentCount={5}
                      heightSegmentCount={5}
                      radius={1}
                      materials={["redColor"]}
                      onHover={this.onHover("ViroSphere")}
                      onFuse={this.onFuse("ViroSphere")}
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
                      onFuse={this.onFuse("ViroSpinner")}
                      onScroll={this.onScroll("ViroSpinner")}
                      onSwipe={this.onSwipe("ViroSpinner")}/>

                  <ViroQuad
                      position={[-2, -1, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      materials={["redColor"]}
                      width={1}
                      height={1}
                         onHover={this.onHover("ViroQuad")}
                         onClick={this.onClick("ViroQuad")}
                         onFuse={{callback:this.onFuse("ViroQuad"), timeToFuse:1000}}
                         onClickState={this.onClickState("ViroQuad")}
                         onTouch={this.onTouch("ViroQuad")}
                         onScroll={this.onScroll("ViroQuad")}
                         onSwipe={this.onSwipe("ViroQuad")}/>

                  <ViroText
                      position={[-1, -1, 0]}
                      scale={[0.5 , 0.5, 0.1]}
                      style={styles.baseTextTwo}
                      text="This is a Viro Text"
                      onHover={this.onHover("ViroText")}
                      onFuse={this.onFuse("ViroText")}
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
                      onFuse={this.onFuse("ViroVideo")}
                      onClick={this.onClick("ViroVideo")}
                      onClickState={this.onClickState("ViroVideo")}
                      onTouch={this.onTouch("ViroVideo")}
                      onScroll={this.onScroll("ViroVideo")}
                      onSwipe={this.onSwipe("ViroVideo")}
                      source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}} />

                  <ViroText
                          position={[0, -2, 0]}
                          scale={[0.5 , 0.5, 0.1]}
                          style={styles.baseTextTwo}
                          text="Toggle Reticle Visibility"
                          onClick={this.toggleReticle}/>

              </ViroNode>
            </ViroScene>

    );
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./GroupTestDragEvents')});
  },

  _toggleTransform() {
    var newtransformflag = this.state.transformFlag + 1;
    if (newtransformflag > 5) {
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
      lightingModel: "Constant",
      diffuseTexture: require('../res/heart_d.jpg'),
    },
 });

module.exports = GroupTestBasicEvents;
