/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

'use strict';

/**
 * Pull in all imports required for the controls within this scene.
 */
import React, { Component } from 'react';
import {StyleSheet} from 'react-native';

import {
  ViroScene,
  Viro360Image,
  ViroAnimations,
  ViroNode,
  ViroImage,
  ViroUtils,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;

/**
 * Set all the images and assets required in this scene.
 */
var backgroundImage = require('./res/westlake_towers.jpg');
var monorailInfoCard = require('./res/infocard_monorail.png');
var statueWindowCard = require('./res/infocard_statue.png');
var slutWindowCard = require('./res/infocard_slut.png');
var backImage = require('./res/icon_back.png');

/**
 * Grab our created custom controls used within this scene.
 */
var LoadingSpinner = require('./custom_controls/LoadingSpinner');
var InfoElement = require('./custom_controls/InfoElement');

export default class OfficeTourSplashScene extends Component {
  constructor() {
    super();

    // set initial state
    this.state = {
      showSceneItems : false,
    }

    // bind `this` to functions
    this._getInfoControls = this._getInfoControls.bind(this);
    this._onBackClick = this._onBackClick.bind(this);
    this._onBackgroundPhotoLoadEnd = this._onBackgroundPhotoLoadEnd.bind(this);
  }

    /**
     * Renders a scene with a 360 Photo background that contains a few toggleable Info UI Elements
     * featuring iconic items like the SLUT, monorail and statue.
     */
  render() {
    return (
      <ViroScene style={styles.container}>
        <Viro360Image source={backgroundImage} onLoadEnd={this._onBackgroundPhotoLoadEnd}/>

        {
          /*
           * Display a spinner icon while the background image is being loaded.
           * Once loaded, hide spinner and show the Info UI Elements.
           */
        }
        <LoadingSpinner visible={!this.state.showSceneItems} position={[0, 0, -5]}/>

        {this._getInfoControls()}

      </ViroScene>
    );
 }

  /**
   * Displays a set of InfoElement controls representing several POI locations
   * within this scene, and as well as a back button at the bottom of the scene.
   */
  _getInfoControls() {
    return (
      <ViroNode
        opacity={0.0}
        animation={{
          name : "fadeIn",
          run : this.state.showSceneItems,
          loop : false,
        }} >

        <InfoElement content={slutWindowCard} contentCardScale={[3.67,4,1]} position={polarToCartesian([-5, 0, 0])}/>
        <InfoElement content={monorailInfoCard} contentCardScale={[3.67,4,1]} position={polarToCartesian([-5, 77, -10])}/>
        <InfoElement content={statueWindowCard} contentCardScale={[4,3.95,2]} position={polarToCartesian([-5, 277, 0])}/>
        <ViroImage
          scale={[1, 1, 1]}
          position={[0, -3.5, 0]}
          rotation={[-90, 0, 0]}
          source={backImage}
          onClick={this._onBackClick}/>
      </ViroNode>
    );
  }

  /**
   * Callback function for when image has finished loading in the Viro360Photo.
   * We then animate the main info elements into the scene through the
   * setting of state showSceneItems.
   */
  _onBackgroundPhotoLoadEnd() {
    this.setState({
      showSceneItems:true
    });
  }

  /**
   * Callback function for when the user taps on back button located at the
   * bottom of the scene. This pops the current scene to the previous one.
   */
  _onBackClick(){
    this.props.sceneNavigator.pop();
  }
}

/**
 * Declare all custom flex box styles here to be reference by the
 * controls above.
 */
var styles = StyleSheet.create({
    container: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
    },
});

/**
 * Declare all your animations here. They'll be referenced by the animation props.
 */
ViroAnimations.registerAnimations({
    fadeIn:{properties:{opacity: 1.0}, duration: 1000},
});

module.exports = OfficeTourSplashScene;
