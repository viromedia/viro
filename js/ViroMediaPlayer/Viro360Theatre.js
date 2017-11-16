
'use strict';

/**
 * Pull in all imports required for the controls within this scene.
 */
import React, { Component } from 'react';
import {StyleSheet} from 'react-native';

import {
  AppRegistry,
  ViroScene,
  ViroVideo,
  ViroSceneNavigator,
  ViroMaterials,
  Viro360Video,
  ViroButton,
  ViroImage,
  ViroNode,
  ViroAnimations,
} from 'react-viro';

var createReactClass = require('create-react-class');

/**
 * Set all the image and asset references required in this scene.
 */
var buttonSize = 0.25;
var VIDEO_REF = "videoref";

/**
 * Several references to video sources (wether it be local or on AWS) stored in an array.
 */
var videos = [
  {uri:'https://s3-us-west-2.amazonaws.com/viro/MediaDemo360_1.mp4'},
  {uri:'https://s3-us-west-2.amazonaws.com/viro/MediaDemo360_2.mp4'}
];

var Viro360Theatre = createReactClass({
  getInitialState() {
    return {
      videoControlsAnimation:"fadeIn",
      videoPaused: false,
      loopVideo: true,
      videoIndex: 0,
      runAnimation: false,
    }
  },

  /**
   * Renders a scene that contains a 360 video and Video Controls.
   */
  render: function() {
    return (
        <ViroScene onClick={this._onVideoTapped} reticleEnabled={this.state.videoControlsAnimation=="fadeIn"}>
          <Viro360Video ref={VIDEO_REF} source={videos[this.state.videoIndex]} volume={1.0}
            loop={this.state.loopVideo} paused={this.state.videoPaused} />

          {this._renderVideoControl()}

        </ViroScene>
    );
  },

  _onVideoTapped(){
    var videoControlsAnimationState = this.state.videoControlsAnimation;
    if (videoControlsAnimationState=="fadeIn"){
      videoControlsAnimationState="fadeOut";
    } else {
      videoControlsAnimationState="fadeIn";
    }

    this.setState({
      videoControlsAnimation:videoControlsAnimationState,
      runAnimation: true,
    });
  },
  /**
   * Render a set of Video UI Controls. This includes (in the order displayed from left to right):
   * Restart, Previous Video, Play/Pause, Next Video, Volume.
   */
  _renderVideoControl(){
    return(
      <ViroNode position={[0,-0.8,0]} opacity={1.0}
        animation={{name : this.state.videoControlsAnimation, run : this.state.runAnimation, loop : false}} >

        <ViroImage
          scale={[1.4, 1.2, 1]}
          position={[0, -0.27,-2.1]}
          source={require("./res/player_controls_container.png")} />

        <ViroButton
          position={[-buttonSize-0.1,0,-2]}
          scale={[1, 1, 1]}
          width={buttonSize}
          height={buttonSize}
          source={require("./res/previous.png")}
          gazeSource={require("./res/previous_hover.png")}
          tapSource={require("./res/previous_hover.png")}
          onClick={this._playPreviousVideo} />

        {this._renderPlayControl()}

        <ViroButton
          position={[buttonSize+0.1, 0,-2]}
          scale={[1, 1, 1]}
          width={buttonSize}
          height={buttonSize}
          source={require("./res/skip.png")}
          gazeSource={require("./res/skip_hover.png")}
          tapSource={require("./res/skip_hover.png")}
          onClick={this._playNextVideo} />

        <ViroButton
          position={[-0.3, -0.4 ,-2]}
          scale={[1, 1, 1]}
          width={0.5}
          height={0.5}
          source={require("./res/icon_2D.png")}
          gazeSource={require("./res/icon_2D_hover.png")}
          tapSource={require("./res/icon_2D_hover.png")}
          onClick={this._launchTheatreScene} />

        <ViroButton
          position={[0.3, -0.4 ,-2]}
          scale={[1, 1, 1]}
          width={0.5}
          height={0.5}
          source={require("./res/icon_360_hover.png")}
          gazeSource={require("./res/icon_360_hover.png")}
          tapSource={require("./res/icon_360_hover.png")} />

      </ViroNode>
    );
  },

  /**
   * Renders either the play or pause icon depending on video state.
   */
  _renderPlayControl(){
    if (this.state.videoPaused){
      return (
          <ViroButton
              position={[0,0,-2]}
              scale={[1, 1, 1]}
              width={buttonSize}
              height={buttonSize}
              source={require("./res/play.png")}
              gazeSource={require("./res/play_hover.png")}
              tapSource={require("./res/play_hover.png")}
              transformBehaviors={["billboard"]}
              onClick={this._togglePauseVideo}/>
      );
    } else {
      return (
          <ViroButton
              position={[0,0,-2]}
              scale={[1, 1, 1]}
              width={buttonSize}
              height={buttonSize}
              source={require("./res/pause.png")}
              gazeSource={require("./res/pause_hover.png")}
              tapSource={require("./res/pause_hover.png")}
              transformBehaviors={["billboard"]}
              onClick={this._togglePauseVideo}/>
      );
    }
  },

  _launchTheatreScene(){
    this.props.sceneNavigator.jump("ViroTheatre", {scene:require('./ViroTheatre')});
  },
  _togglePauseVideo() {
    this.setState({
      videoPaused: !this.state.videoPaused,
    })
  },

  /**
   * Play the previous video by setting the videoIndex.
   */
  _playPreviousVideo(){
    var currentVideo = this.state.videoIndex;
    if (currentVideo - 1 > -1){
      this.setState({
        videoIndex: (currentVideo - 1),
        videoPaused: false
      });
    }
  },

  /**
   * Play the next video by setting the videoIndex.
   */
  _playNextVideo(){
    var currentVideo = this.state.videoIndex;
    if (currentVideo + 1 < videos.length){
      this.setState({
        videoIndex: (currentVideo + 1),
        videoPaused: false
      });
    }
  },

});

ViroAnimations.registerAnimations({
  fadeOut:{properties:{opacity: 0.0}, duration: 500},
  fadeIn:{properties:{opacity: 1.0}, duration: 500},
});

module.exports = Viro360Theatre;
