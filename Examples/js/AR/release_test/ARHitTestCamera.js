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
  Viro3DObject,
  Viro360Video,
  ViroText,
  ViroSurface,
  ViroAmbientLight,
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var objSourceNoPlaneFound = require('./res/tracking_1.vrx')
var objSourcePlane = require('./res/tracking_2.vrx')

var ARHitTestCamera = createReactClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      surfaceSize : [0,0,0],
      center : [0,0,0],
      testWithRay : true,
      displayHitReticle: false,
      displayOnPlane: false,
      displayNothingFound: false,
      planeReticleLocation: [0,0,0],
    }
  },
  render: function() {
    return (
      <ViroARScene
         onAnchorFound={global.onAnchorFound}
         onAnchorRemoved={global.onAnchorRemoved}
         ref="arscene" onCameraARHitTest={this._onCameraARHitTest} >
        <ViroAmbientLight color="#ffffff" intensity={500}/>
        {this._getModel()}
      </ViroARScene>
    );
  },

  _onCameraARHitTest(results) {
      console.log("_onCameraARHitTest invoked!");
      if(results.hitTestResults.length > 0) {
        for (var i = 0; i < results.hitTestResults.length; i++) {
          let result = results.hitTestResults[i];
          if (result.type == "ExistingPlaneUsingExtent") {
            this.setState({
              planeReticleLocation : result.transform.position,
              displayHitReticle: true,
              displayOnPlane: true,
              displayNothingFound: false,
            });
            return;
          } else if(result.type == "FeaturePoint") {
              var position = results.cameraOrientation.position;
              var distance = Math.sqrt(((result.transform.position[0] - position[0]) * (result.transform.position[0] - position[0])) + ((result.transform.position[1] - position[1]) * (result.transform.position[1] - position[1])) + ((result.transform.position[2] - position[2]) * (result.transform.position[2] - position[2])));
              if(distance > .2 && distance < 10) {
              //  this.setState({
              //    planeReticleLocation : result.transform.position,
              //    displayHitReticle: true,
              //    displayOnPlane: true,
              //    displayNothingFound: false,
              //  });
              //  return;
              }
          }
        }
    }

    //else we made it here, so just forward vector with unmarked.
    let newPosition = [results.cameraOrientation.forward[0] * 1.5, results.cameraOrientation.forward[1]* 1.5, results.cameraOrientation.forward[2]* 1.5];
    newPosition[0] = results.cameraOrientation.position[0] + newPosition[0];
    newPosition[1] = results.cameraOrientation.position[1] + newPosition[1];
    newPosition[2] = results.cameraOrientation.position[2] + newPosition[2];
    this.setState({
      planeReticleLocation : newPosition,
      displayHitReticle: true,
      displayOnPlane: false,
      displayNothingFound: true,
    });
  },

  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARScene/Navigator", {scene:require("./ARSceneAndNavigatorTest")})
  },

  _getModel() {
    var modelArray = [];
    //if(!this.props.arSceneNavigator.viroAppProps.displayObject || this.props.arSceneNavigator.viroAppProps.displayObjectName === undefined) {
    //  return;
    //}
    if(this.state.displayHitReticle == false) {
      return;
    }

    let transformBehaviors = {};
    if (this.state.shouldBillboard) {
      transformBehaviors.transformBehaviors = this.state.shouldBillboard ? "billboardY" : [];
    }

     var bitMask = 4;
      modelArray.push(<ViroNode
        {...transformBehaviors}
        visible={this.state.displayHitReticle}
        position={this.state.planeReticleLocation}
        scale={[.3, .3, .3]}
        rotation={[0,0,0]}
        key={"test1"}>

        <Viro3DObject
            position={[0, .01, 0]}
            source={objSourcePlane}
            type = "VRX" visible={this.state.displayOnPlane}
          />

        <Viro3DObject
            position={[0, .01, 0]}
            source={objSourceNoPlaneFound}
            type = "VRX" visible={this.state.displayNothingFound}
          />
      </ViroNode>
    );
    return modelArray;
  },

  renderIf(condition, content) {
      if (condition) {
          return content;
      } else {
          return null;
      }
  },
});

ViroMaterials.createMaterials({
  transparent: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#00000055"
  },
});

var styles = StyleSheet.create({
  instructionText: {
      fontFamily: 'Arial',
      fontSize: 10,
      color: '#cccccc',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});

module.exports = ARHitTestCamera;
