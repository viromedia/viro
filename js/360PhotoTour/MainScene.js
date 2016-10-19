/**
 * Copyright (c) 2015-present, Viro Media, Inc.
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
import {
    StyleSheet,
    ViroScene,
    ViroOmniLight,
    Viro360Photo,
    ViroImage,
    ViroAnimations,
    ViroAnimatedComponent,
    ViroView,
    ViroText
} from 'react-viro';

/**
 * Set all the images and assets required in this scene.
 */
var westLakeTowersScene = require('./WestLakeTowers');
var backgroundImage = require('./res/westlake_towers.jpg');
var weworkImage = require('./res/wework_title.png');

var MainScene = React.createClass({
    getInitialState() {
        return {
            runShowTitleAnimation:false,
        };
    },

    /**
     * Renders a scene with a 360 Photo background that contains a single WeWork ViroImage. This
     * image will be faded/scaled in with the "showTitleAnimation" when the scene first appears. Clicking on
     * the WeWork ViroImage will launch the user into the next scene (WestLakeTowers).
     */
    render: function() {
        return (
            <ViroScene style={styles.container}>
                <ViroOmniLight position={[0, 0, 0]} color="#ffffff"
                               attenuationStartDistance={40} attenuationEndDistance={50}/>
                <Viro360Photo source={backgroundImage} onLoadEnd={this._onBackgroundPhotoLoadEnd}/>
                <ViroAnimatedComponent animation="showTitleAnimation"
                                       run={this.state.runShowTitleAnimation} loop={false}>
                    <ViroImage position={[0, 0, -5]} source={weworkImage} scale={[.1, .1, .1]}
                               opacity={0.0} onTap={this._onTitleTapped}/>
                </ViroAnimatedComponent>
            </ViroScene>
        );
    },

    /**
     * Callback function for when image has finished loading in the Viro360Photo.
     * We then animate the WeWork ViroImage into the scene through the
     * setting of state runShowTitleAnimation.
     */
    _onBackgroundPhotoLoadEnd(){
        this.setState({
            runShowTitleAnimation:true
        });
    },

    /**
     * Callback function for when the user taps on the WeWork ViroImage
     * where we navigate into the second scene.
     */
    _onTitleTapped(){
        this.props.sceneNavigator.push({scene:westLakeTowersScene});
    },
});

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
 * Declare all your animations here to be referenced and played by the
 * ViroAnimatedComponents above.
 */
ViroAnimations.registerAnimations({
    showTitleAnimation: {properties:{scaleX:2, scaleY:2, scaleZ:2, opacity:1.0}, easing:"PowerDecel", duration:1000},
});

module.exports = MainScene;
