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
  ViroMaterialVideo,
  ViroSphere,
  ViroOmniLight,
  ViroSceneNavigator,
  ViroMaterials,
  ViroText,
  ViroUtils,
  ViroImage,
} from 'react-viro';

var createReactClass = require('create-react-class');
var VIDEO_REF = "videoref";
let polarToCartesian = ViroUtils.polarToCartesian;

var localVideo = require("./res/Titan_Touchdown.mp4");
var localVideoStereo = require("./res/stereoVid.mp4");
var uriSource = {uri:'https://s3.amazonaws.com/viro.video/Climber1Bottom.mp4'};
var ReleaseMenu = require("./ReleaseMenu.js");
var materialsToTest = ["redColor", "titanTouchDown", "climberVid"];

var ViroMaterialVideoTest = createReactClass({
  getInitialState() {
    return {
      videoPaused: false,
      loopVideo: true,
      muteVideo: true,
      volume: 1,
      showVideo: true,
      uriVideo:localVideo,
      materialIndex: 0,
    }
  },
  render: function() {
    return (
      <ViroScene>
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
        <ViroOmniLight position={[0, 0, 0]} color="#ffffff"
          attenuationStartDistance={40} attenuationEndDistance={50} />
          <ViroSphere
                position={[0, 1, -3]}
                scale={[0.3, 0.3, 0.3]}
                widthSegmentCount={20}
                heightSegmentCount={20}
                radius={1}
                materials={materialsToTest[this.state.materialIndex]}
                />
 {this._getViroVideo()}

 <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />


        <ViroText position={polarToCartesian([2, -10, 0])} text={this._getLoopingText()}
          style={styles.baseTextTwo} onClick={this._toggleLooping} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, -10, -5])} text="Restart"
          style={styles.baseTextTwo} onClick={this._restartVideo} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, -10, -10])} text={"IsVideoPlaying: " + !this.state.videoPaused}
          style={styles.baseTextTwo} onClick={this._togglePauseVideo} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, -10, -15])} text={this._getMuteText()}
          style={styles.baseTextTwo} onClick={this._muteVideo} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, -10, -20])} text="Change Source"
          style={styles.baseTextTwo} onClick={this._changeVideoSource} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, 15, 0])} text={"Change Volume from: " + this.state.volume}
          style={styles.baseTextTwo} onClick={this._changeVolume} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, 15, -5])} text={"Change Material"}
            style={styles.baseTextTwo} onClick={this._changeMaterial} transformBehaviors={["billboard"]}/>

          </ViroScene>
    );
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./ViroAnimatedComponentTest')});
  },

  _getViroVideo() {
      return (
        <ViroMaterialVideo ref={VIDEO_REF} material={this._getMaterialVideoControl()} paused={this.state.videoPaused}
           onBufferStart={this._onBufferStart} onBufferEnd={this._onBufferEnd}
          loop={this.state.loopVideo} muted={this.state.muteVideo} volume={this.state.volume}
          onFinish={this._onVideoFinished} onUpdateTime={this._onUpdateTime} />
      );
  },
  _onBufferStart() {
    console.log("ViroVideoTest onBufferStart");
  },
  _onBufferEnd() {
    console.log("ViroVideoTest onBufferEnd");
  },
  _onVideoFinished(){
    console.log("Viro on video Finished");
  },
  _onUpdateTime(current, total){
    console.log("Viro On time update-> Current: " + current+ ", total: " + total);
  },
  _changeVideoSource() {
    var newVideoSource;
    if (this.state.uriVideo == uriSource) {
      newVideoSource = localVideo;
    } else if (this.state.uriVideo == localVideo){
      newVideoSource = localVideoStereo;
    } else {
      newVideoSource = uriSource;
    }

    this.setState({
      uriVideo: newVideoSource,
    })
  },
  _getMuteText() {
    return this.state.muteVideo ? "IsMute: True" : "IsMute: False";
  },
  _muteVideo() {
    this.setState({
      muteVideo: !this.state.muteVideo,
    })
  },
  _togglePauseVideo() {
    this.setState({
      videoPaused: !this.state.videoPaused,
    })
  },
  _getLoopingText() {
    return this.state.loopVideo ? "IsLooping: True" : "IsLooping: False";
  },
  _toggleLooping() {
    this.setState({
      loopVideo: !this.state.loopVideo,
    });
  },
  _pauseVideo() {
    console.log("pausing video!");
    this.setState({
      videoPaused: true,
    })
  },
  _playVideo() {
    console.log("playing video!");
    this.setState({
      videoPaused: false,
    })
  },
  _changeVolume() {
    this.setState({
      volume: Math.random(),
    });
  },
  _getMaterialVideoControl() {
    if(materialsToTest[this.state.materialIndex] == "redColor") {
        return "titanTouchDown"
    }
    return materialsToTest[this.state.materialIndex];
  },

  _changeMaterial() {
    if(this.state.materialIndex >= 2) {
      this.setState({
        materialIndex:0,
      });
    } else {
        this.setState({
          materialIndex: this.state.materialIndex +1,
        });
      }
  },
  _restartVideo() {
    // This tests seeking to a non integer number and seeking in general.
    this.refs[VIDEO_REF].seekToTime(shouldResetToHalfSecond ? .5 : 0);
    shouldResetToHalfSecond = !shouldResetToHalfSecond;
  },
});

var shouldResetToHalfSecond = false;

ViroMaterials.createMaterials({
  redColor: {
    fresnelExponent: .5,
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#ff0000",
  },
  titanTouchDown: {
    fresnelExponent: .5,
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/Titan_Touchdown.mp4"),
  },
  climberVid: {
    fresnelExponent: .5,
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseTexture: {uri:'https://s3.amazonaws.com/viro.video/Climber1Bottom.mp4'},
  },
});

var styles = StyleSheet.create({
  baseTextTwo: {

      fontFamily: 'Arial',
      fontSize: 10,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});


module.exports = ViroMaterialVideoTest;
