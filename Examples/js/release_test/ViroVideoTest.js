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
  ViroVideo,
  ViroOmniLight,
  ViroSceneNavigator,
  Materials,
  ViroText,
  ViroUtils,
  Viro360Photo,
  Viro360Video,
  ViroImage,
  ViroMaterials,
} from 'react-viro';

var createReactClass = require('create-react-class');
var VIDEO_REF = "videoref";
let polarToCartesian = ViroUtils.polarToCartesian;

var localVideo = require("./res/Titan_Touchdown.mp4");
var chromeVideo = require("./res/alphachica.mp4");
var localVideoStereo = require("./res/stereoVid.mp4");
var uriSource = {uri:'https://s3.amazonaws.com/viro.video/Climber1Bottom.mp4'};
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroVideoTest = createReactClass({
  getInitialState() {
    return {
      videoPaused: false,
      loopVideo: true,
      muteVideo: true,
      volume: 1,
      showVideo: true,
      uriVideo:localVideo,
    }
  },
  render: function() {
    return (
      <ViroScene>
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
        <ViroOmniLight position={[0, 0, 0]} color="#ffffff"
          attenuationStartDistance={40} attenuationEndDistance={50} />
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
          </ViroScene>
    );
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./ViroAnimatedComponentTest')});
  },

  _getViroVideo() {
      return (
        <ViroVideo ref={VIDEO_REF}  width={1.7} height={0.95} position={[0, 1, -2.9]} scale={[1, 1, 1]} paused={this.state.videoPaused}
          source={this.state.uriVideo} transformBehaviors={["billboard"]} onBufferStart={this._onBufferStart} onBufferEnd={this._onBufferEnd}
          stereoMode={this.state.uriVideo == localVideoStereo ? "LeftRight":"None"}
          loop={this.state.loopVideo} muted={this.state.muteVideo} volume={this.state.volume}
          onFinish={this._onVideoFinished} onUpdateTime={this._onUpdateTime} materials={["chromeKeyedVideo"]} />
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
    } else if (this.state.uriVideo == localVideo) {
      newVideoSource = localVideoStereo;
    } else if (this.state.uriVideo == localVideoStereo) {
        newVideoSource = chromeVideo;
    } else {
      newVideoSource = uriSource;
    }

    this.setState({
      uriVideo: newVideoSource,
    });
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
  _restartVideo() {
    // This tests seeking to a non integer number and seeking in general.
    this.refs[VIDEO_REF].seekToTime(shouldResetToHalfSecond ? .5 : 0);
    shouldResetToHalfSecond = !shouldResetToHalfSecond;
  },
});

var shouldResetToHalfSecond = false;

ViroMaterials.createMaterials({
  chromeKeyedVideo : {
    chromaKeyFilteringColor: "#00ff00"
  },
  normalVideo: {
    lightingModel: "Constant",
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

module.exports = ViroVideoTest;
