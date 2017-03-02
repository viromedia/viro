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

var Uri360Video = {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf.mp4"};
var Local360Video = require("./res/360Asteroids.mp4");

var Viro360ImageTest = require('./Viro360ImageTest');
var ViroAmbientLightTest = require('./ViroAmbientLightTest');

var Viro360VideoTest = React.createClass({

  getInitialState() {
    return {
      get360Video:Uri360Video,
      videoMuted:false,
      videoPaused:false,
      volumeSet:1,
    };
  },

  render: function() {
    return (
     <ViroScene>
     <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

     <Viro360Video
      rotation={[0,0,0]}
      source={this.state.get360Video}
      onFinish={this._showOther}
      muted={this.state.videoMuted}
      paused={this.state.videoPaused}
      volume={this.state.volumeSet}
      onUpdateTime={this._updateTime}
      loop={true}
      />


     <ViroAnimatedComponent animation="fadeIn" run={true} loop={false}>
     <ViroImage source={require('./res/icon_left_w.png')} position={[-2, -4, -3]} scale={[0, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showPrevious} />
     </ViroAnimatedComponent>

     <ViroImage source={require('./res/poi_dot.png')} position={[-2, -2, -3]} transformBehaviors={["billboard"]} onClick={this._pauseVideo} />
     <ViroText text="Pause" position={[-2, -3, -3]} transformBehaviors={["billboard"]} />

     <ViroImage source={require('./res/poi_dot.png')} position={[0, -2, -3]} transformBehaviors={["billboard"]} onClick={this._muteVideo} />
     <ViroText text="Mute" position={[0, -3, -3]} transformBehaviors={["billboard"]} />

     <ViroImage source={require('./res/poi_dot.png')} position={[2, -2, -3]} transformBehaviors={["billboard"]} onClick={this._volumeUp} />
     <ViroText text="Volume Up" position={[2, -3, -3]} transformBehaviors={["billboard"]} />

     <ViroImage source={require('./res/poi_dot.png')} position={[4, -2, -3]} transformBehaviors={["billboard"]} onClick={this._volumeDown} />
     <ViroText text="Volume Down" position={[4, -3, -3]} transformBehaviors={["billboard"]} />

     <ViroImage source={require('./res/poi_dot.png')} position={[0, -4, -3]} transformBehaviors={["billboard"]} onClick={this._showOther} />
     <ViroText text="Viro360Video" position={[0, -5, -3]} transformBehaviors={["billboard"]} />

     <ViroAnimatedComponent animation="fadeIn" run={true} loop={false}>
     <ViroImage source={require('./res/icon_right_w.png')} position={[2, -4, -3]} scale={[0, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />
     </ViroAnimatedComponent>

     </ViroScene>

    );
  },


  _updateTime(current, total) {
    console.log("Viro360VideoTest On time update-> Current: " + current+ ", total: " + total);
  },

  _showOther() {
    this.setState({
        get360Video:Local360Video,
      });
  },


  _showPrevious() {
    this.props.sceneNavigator.pop();
  },

  _pauseVideo() {
    this.setState({
        videoPaused:!this.state.videoPaused,
      });
  },

  _muteVideo() {
    this.setState({
        videoMuted:!this.state.videoMuted,
      });
  },

  _volumeUp() {
    this.setState({
        volumeSet:.9,
      });
  },

  _volumeDown() {
    this.setState({
        volumeSet:.2,
      });
  },

  _showNext() {
    this.props.sceneNavigator.push({scene:ViroAmbientLightTest});
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
  elementText: {
    fontFamily: 'HelveticaNeue-Medium',
    fontSize: 30,
    color: '#ffffff',
    textAlign: 'center',
  },
});

module.exports = Viro360VideoTest;
