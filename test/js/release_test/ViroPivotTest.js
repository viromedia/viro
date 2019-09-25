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
  ViroQuad,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var ReleaseMenu = require("./ReleaseMenu.js");

var ViroPivotTest = createReactClass({

  getInitialState() {
    return {
      rotationPivot:"Center",
      scalePivot:"Center",
    };
  },

  _getPivotArray(pivotLabel) {
    if (pivotLabel == "Center") {
      return [0, 0, 0];
    }
    else if (pivotLabel == "Lower Left") {
      return [-1, -1, 0];
    }
    else if (pivotLabel == "Lower Right") {
      return [1, -1, 0];
    }
    else if (pivotLabel == "Upper Right") {
      return [1, 1, 0];
    }
    else if (pivotLabel == "Upper Left") {
      return [-1, 1, 0];
    }
    else {
      return [0, 0, 0];
    }
  },

  render: function() {
    var rotationPivot = this._getPivotArray(this.state.rotationPivot);
    var scalePivot = this._getPivotArray(this.state.scalePivot);
    return (
      <ViroScene>
        <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
        <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>
        <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

          <ViroBox width={2} height={2} length={2}
                   position={[0, 0, -6]}
                   rotationPivot={rotationPivot}
                   scalePivot={scalePivot}
                   materials={["boxMaterial"]} animation={{name:"scaleAndRotate", delay:0, loop:true, run:true}} />

        <ViroNode position={[0,-4,-3]}>
          <ViroText style={styles.baseTextTwo} position={[-2, 1, 0]} width={2} height={2}
                    text={"Change Rotation Pivot [Current : " + this.state.rotationPivot + "]"}
                    onClick={this._toggleRotationPivot}/>
          <ViroText style={styles.baseTextTwo} position={[0, 1, 0]} width={2} height={2}
                    text={"Change Scale Pivot [Current : " + this.state.scalePivot + "]"}
                    onClick={this._toggleScalePivot}/>
        </ViroNode>
     </ViroScene>
    );
  },

  _showNext() {
     this.props.sceneNavigator.replace({scene:require('./ViroBoxTest')});
  },

  _getNextPivot(currentPivot) {
    if (currentPivot == "Center") {
      return "Lower Left";
    }
    else if (currentPivot == "Lower Left") {
      return "Lower Right";
    }
    else if (currentPivot == "Lower Right") {
      return "Upper Right";
    }
    else if (currentPivot == "Upper Right") {
      return "Upper Left";
    }
    else {
      return "Center";
    }
  },

  _toggleRotationPivot() {
    var newPivot = this._getNextPivot(this.state.rotationPivot);
    this.setState({
      rotationPivot:newPivot,
    });
  },

  _toggleScalePivot() {
    var newPivot = this._getNextPivot(this.state.scalePivot);
    this.setState({
      scalePivot:newPivot,
    });
  },
});

ViroAnimations.registerAnimations({
  scaleUp:{properties:{scaleX:"+=0.5", scaleY:"+=0.5", scaleZ:"+=0.5"}, duration:1000},
  scaleDown:{properties:{scaleX:"-=0.5", scaleY:"-=0.5", scaleZ:"-=0.5"}, duration:1000},
  rotate:{properties:{rotateZ:"+=360"}, duration:1000},
  scaleAndRotate:[
      ["rotate", "scaleUp", "scaleDown"],
  ],
});

var styles = StyleSheet.create({
  baseTextTwo: {
    fontFamily: 'Arial',
    fontSize: 18,
    color: '#ffffff',
    textAlignVertical: 'center',
    textAlign: 'center',
  }
});

ViroMaterials.createMaterials({
   boxMaterial: {
     shininess : 2.0,
     lightingModel: "Blinn",
     diffuseTexture: require('./res/360_waikiki.jpg'),
   },
});

module.exports = ViroPivotTest;
