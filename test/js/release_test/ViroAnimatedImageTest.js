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
  ViroAnimatedImage
} from 'react-viro';

var createReactClass = require('create-react-class');
let polarToCartesian = ViroUtils.polarToCartesian;

var localSource1 = require("./res/testingGifplz2.gif");
var localSource2 = require("./res/testingGifplz3.gif");
var uriSource = {uri:"https://cdn.vox-cdn.com/thumbor/lv4CD8Y-BFnpagAg6vgGcBozr8Y=/1600x0/filters:no_upscale()/cdn.vox-cdn.com/uploads/chorus_asset/file/8692949/no_words_homer_into_brush.gif"};

var ReleaseMenu = require("./ReleaseMenu.js");
var placeholderImage = require("./res/grid_bg.jpg");

var ViroAnimatedImageTest = createReactClass({
  getInitialState() {
    return {
      videoPaused: false,
      loopVideo: true,
      showVideo: true,
      uriVideo:localSource1,
      lastLoadText:"",
      resizeMode:"ScaleToFill",
      clipMode:"ClipToBounds",
    }
  },
  render: function() {
    return (
      <ViroScene>
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
        <ViroOmniLight position={[0, 0, 0]} color="#ffffff"
          attenuationStartDistance={40} attenuationEndDistance={50} />
 {this._getViroVideo()}



        <ViroText position={[0, -0, -2]} text={this._getLoopingText()}
          style={styles.baseTextTwo} onClick={this._toggleLooping} />
        <ViroText position={[0, -0.3, -2]} text={"Toggle clipMode " + this.state.clipMode}
          style={styles.baseTextTwo} onClick={this._toggleImageClipMode}/>
        <ViroText position={[0, -0.6, -2]} text={"IsVideoPlaying: " + !this.state.videoPaused}
          style={styles.baseTextTwo} onClick={this._togglePauseVideo} />
        <ViroText position={[1.2, 0,  -2]} text={"OnLoadCallback: " + this.state.lastLoadText}
          style={styles.baseTextTwo} />
        <ViroText position={[1.2, -0.3, -2]} text="Change Source"
          style={styles.baseTextTwo} onClick={this._changeVideoSource} />
        <ViroText position={[1.2, -0.6, -2]} text={"Toggle resizeMode " + this.state.resizeMode}
          style={styles.baseTextTwo} onClick={this._toggleResizeMode}/>
          </ViroScene>
    );
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./ViroAnimatedComponentTest')});
  },

  _getViroVideo() {
      return (
        <ViroAnimatedImage
        resizeMode={this.state.resizeMode}
        imageClipMode={this.state.clipMode}
          width={1.7} height={0.95} position={[0, 1, -2.9]} scale={[1, 1, 1]}
          paused={this.state.videoPaused}
          source={this.state.uriVideo}
          transformBehaviors={["billboard"]}
          onLoadStart={this._onLoadStart}
          onLoadEnd={this._onLoadEnd}
          onError={this._onError}
          loop={this.state.loopVideo}
          placeholderSource={require("./res/grid_bg.jpg")} />
      );
  },
  _onLoadStart() {
    this.setState({
      lastLoadText: "_onLoadStart",
    });
    console.log("ViroAnimImageTest: _onLoadStart");
  },
  _onLoadEnd() {
    this.setState({
      lastLoadText: "_onLoadEnd",
    });
    console.log("ViroAnimImageTest: _onLoadEnd");
  },
  _onError(error){
    this.setState({
      lastLoadText: "_onLoadError",
    });
    console.log("ERROR: " + error);
  },

  _changeVideoSource() {
    var newVideoSource;
    if (this.state.uriVideo == uriSource) {
      newVideoSource = localSource1;
    } else if (this.state.uriVideo == localSource1) {
      newVideoSource = localSource2;
    } else {
      newVideoSource = uriSource;
    }

    this.setState({
      uriVideo: newVideoSource,
    });
  },
  _toggleResizeMode() {
       var newMode;
       if (this.state.resizeMode == "ScaleToFit") {
         newMode="ScaleToFill";
       } else if (this.state.resizeMode == "ScaleToFill") {
         newMode="StretchToFill";
       } else if (this.state.resizeMode == "StretchToFill") {
         newMode="ScaleToFit";
       }
       this.setState({
         resizeMode:newMode
       });
  },
  _toggleImageClipMode() {
       var mode;
       if (this.state.clipMode == "ClipToBounds") {
         mode = "None";
       } else if (this.state.clipMode == "None") {
         mode = "ClipToBounds";
       }
       this.setState({
         clipMode:mode
       });
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

module.exports = ViroAnimatedImageTest;
