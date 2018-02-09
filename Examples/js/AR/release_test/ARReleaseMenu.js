/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

import {
  ViroScene,
  ViroARScene,
  ViroARPlane,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroImage,
  ViroVideo,
  ViroUtils,
  ViroSkyBox,
  Viro360Video,
  ViroText,
  ViroSurface
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';
var createReactClass = require('create-react-class');
let polarToCartesian = ViroUtils.polarToCartesian;

// Tweak this value to get max rows:
var maxRows = 4

var testFiles = [
  ["ARScene/Navigator", require("./ARSceneAndNavigatorTest")],
  ["ARDragTest", require("./ARDragTest")],
  ["ARPlaneSelectorTest", require("./ARPlaneSelectorTest")],
  ["ARPlaneTest", require("./ARPlaneTest")],
  ["ARPlaneManualTest", require("./ARPlaneManualTest")],
  ["ARHitTest", require("./ARHitTest")],
  ["ARHitTestCamera", require("./ARHitTestCamera")],
  ["ViroPortalTestAR", require("./ViroPortalTestAR")],
  ["ViroPostProcessTestAR", require("./ViroPostProcessTestAR")],
  ["ARImageMarkerTest", require("./ARImageMarkerTest")],
  ["ViroPbrTestBasic", require("./ViroPbrTestAR")],
  ["ViroPbrTestSurface", require("./ViroPbrTestSurfacesAR")],
]

var ARReleaseMenu = createReactClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      surfaceSize : [0,0,0],
      testWithRay : true,
    }
  },
  render: function() {
    return (
      <ViroARScene>
        <ViroText style={styles.centeredText}  position={[0, 1, -4]} width={1.5} height ={1.5}
          text={"AR Release Test Menu"} />
        {this._getText()}
      </ViroARScene>
    );
  },
  _getText() {
    let text = []
    for (var i = 0; i < testFiles.length; i++) {
      let file = testFiles[i];
      let column = parseInt(i / maxRows);
      let row = parseInt(i % maxRows);
      text.push((
        <ViroText text={file[0]} position={[2 * column - 2, -.5 * row, -3]}
          onClick={this._getOnClick(file)} key={file[0]} />
      ))
    }
    return text;
  },
  _getOnClick(file) {
    return ()=>{
      this.props.arSceneNavigator.replace(file[0], {scene:file[1]})
    }
  }
});

var styles = StyleSheet.create({
  centeredText: {
    fontSize: 25,
    fontFamily: 'Arial',
    color: '#bbbbbb',
  },
});

module.exports = ARReleaseMenu;
