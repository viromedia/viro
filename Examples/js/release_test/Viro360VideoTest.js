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
  ViroSurface,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;
var VIDEO_REF = "videoref";
var Uri360Video = {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf.mp4"};
var Local360Video = require("./res/360Asteroids.mp4");
var ReleaseMenu = require("./ReleaseMenu.js");

var Viro360VideoTest = React.createClass({

  getInitialState() {
    return {
          get360Video:Local360Video,
          videoPaused: false,
          loopVideo: true,
          muteVideo: false,
          volume: 1,
          rotationY: 0,
          showVideo: true,
        }
  },

  render: function() {
    return (
     <ViroScene>
     <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
     <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>
     <Viro360Video
          ref={VIDEO_REF}
           rotation={[0, this.state.rotationY,0]}  width={1.7} height={0.95} position={[0, 1, -2.9]} scale={[1, 1, 1]} paused={this.state.videoPaused}
          source={this.state.get360Video} transformBehavior={["billboard"]}
          loop={this.state.loopVideo} muted={this.state.muteVideo} volume={this.state.volume}
          onFinish={this._onVideoFinished} onUpdateTime={this._onUpdateTime}/>

          <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />


<ViroText position={polarToCartesian([2, -10, 0])} text={this._getLoopingText()}
          style={styles.baseTextTwo} onClick={this._toggleLooping} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, -10, -5])} text="Restart"
          style={styles.baseTextTwo} onClick={this._restartVideo} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, -10, -10])} text={"IsPlaying: " + !this.state.videoPaused}
          style={styles.baseTextTwo} onClick={this._togglePauseVideo} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, -10, -15])} text={this._getMuteText()}
          style={styles.baseTextTwo} onClick={this._muteVideo} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, -10, -20])} text="Change Source"
          style={styles.baseTextTwo} onClick={this._changeVideoSource} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, 15, 0])} text={"Change Volume from: " + this.state.volume}
          style={styles.baseTextTwo} onClick={this._changeVolume} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, 15, -10])} text={"Toogle Rotate: " + this.state.rotationY}
                  style={styles.baseTextTwo} onClick={this._toggleRotation} transformBehaviors={["billboard"]}/>
          </ViroScene>
    );
  },

  _showNext() {
          this.props.sceneNavigator.push({scene:require('./ViroVideoTest')});
      },


    _toggleRotation() {
        var newRotation = this.state.rotationY + 10;
        if (newRotation == 360){
            newRotation = 0;
        }
        this.setState({
                          rotationY:newRotation
                });
    },
  _onVideoFinished(){
    var videoTag = this.state.get360Video == Uri360Video ? "Uri Video" : " Local Video";
    console.log("Viro on video Finished for: " + videoTag );
  },
  _onUpdateTime(current, total){
    console.log("Viro On time update-> Current: " + current+ ", total: " + total);
  },
  _changeVideoSource() {
    var newVideo = this.state.get360Video == Uri360Video ? Local360Video :Uri360Video;
    this.setState({
      get360Video: newVideo,
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

ViroAnimations.registerAnimations({
  fadeIn:{properties:{scaleX:1, scaleY:1, scaleZ:1}, duration: 5000},
});

var styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
baseTextTwo: {
      fontFamily: 'Arial',
      fontSize: 10,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});

module.exports = Viro360VideoTest;
