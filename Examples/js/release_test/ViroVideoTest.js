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
} from 'react-viro';

var VIDEO_REF = "videoref";
let polarToCartesian = ViroUtils.polarToCartesian;

var localVideo = require("./res/Titan_Touchdown.mp4");
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroVideoTest = React.createClass({
  getInitialState() {
    return {
      videoPaused: false,
      loopVideo: true,
      muteVideo: true,
      videoSource: {uri:'https://s3.amazonaws.com/viro.video/Climber2Top.mp4'},
      volume: 1,
      showVideo: true,
    }
  },
  render: function() {
    return (
      <ViroScene>
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
        <ViroOmniLight position={[0, 0, 0]} color="#ffffff"
          attenuationStartDistance={40} attenuationEndDistance={50} />
 {this._getViroVideo()}

<ViroText position={polarToCartesian([2, -10, 0])} text={this._getLoopingText()}
          style={styles.baseTextTwo} onClick={this._toggleLooping} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, -10, -5])} text="Restart"
          style={styles.baseTextTwo} onClick={this._restartVideo} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, -10, -10])} text={"IsVideoPlaying: " + this.state.videoPaused}
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
  _getViroVideo() {
    if (this.state.showVideo) {
      return (
        <ViroVideo ref={VIDEO_REF}  width={1.7} height={0.95} position={[0, 1, -2.9]} scale={[1, 1, 1]} paused={this.state.videoPaused}
          source={localVideo} transformBehavior={["billboard"]}
          loop={this.state.loopVideo} muted={this.state.muteVideo} volume={this.state.volume}
          onFinish={this._onVideoFinished} onUpdateTime={this._onUpdateTime}/>
      );
    } else {
      return; (
        <ViroVideo ref={VIDEO_REF} width={1.7} height={0.95} position={[0, 1, -2.9]} scale={[1, 1, 1]} paused={this.state.videoPaused}
          source={{uri:'https://s3.amazonaws.com/viro.video/Climber1Bottom.mp4'}} transformBehavior={["billboard"]}
          loop={this.state.loopVideo} muted={this.state.muteVideo} volume={this.state.volume}
          onFinish={this._onVideoFinished} onUpdateTime={this._onUpdateTime}/>
      );
    }
  },
  _onVideoFinished(){
    console.log("Viro on video Finished");
  },
  _onUpdateTime(current, total){
    console.log("Viro On time update-> Current: " + current+ ", total: " + total);
  },
  _changeVideoSource() {
    this.setState({
      videoSource: {uri:'https://s3.amazonaws.com/viro.video/Climber1Bottom.mp4'},
      showVideo: !this.state.showVideo,
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
  _restartVideo() {
    this.refs[VIDEO_REF].seekToTime(0);
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