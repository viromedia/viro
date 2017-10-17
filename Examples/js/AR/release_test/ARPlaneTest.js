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
  ViroARScene,
  ViroARPlane,
  ViroMaterials,
  ViroImage,
  ViroARPlaneSelector,
  ViroSurface,
  ViroConstants,
  ViroVideo,
  ViroText,
  ViroUtils
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

let polarToCartesian = ViroUtils.polarToCartesian;

var testARScene = React.createClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      addPlane : true,
      useFirstPlane : true,
      updateMap : {width:0, height:0, position:[0,0,0]},
      minValue : 0,
    }
  },
  render: function() {
    return (
      <ViroARScene position={[0,0,0]} reticleEnabled={false} >

        {this._getPlane()}

        <ViroText position={polarToCartesian([2, 30, 10])} text={!this.state.addPlane ? "Add Plane" : "Remove Plane"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._addRemovePlane}/>
        <ViroText position={polarToCartesian([2, 30, 0])} text={"Switch Plane"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._switchPlane}/>
        <ViroText position={polarToCartesian([2, 30, -10])} text={"Min Width/Height: " + this.state.minValue}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._increaseMinValue}/>

        <ViroText position={polarToCartesian([2, 0, 10])} text={this._getFoundText()}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, 0, 0])} text={this._getUpdatedText()}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, 0, -10])} text={this._getRemovedText()}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, 0, -20])} text={"No Callback"}
          style={styles.instructionText} transformBehaviors={["billboard"]}
          ref={component=>{this._callbackText = component}}/>

        {/* Release Menu */}
        <ViroText position={polarToCartesian([2, -30, 0])} text={"Next test"}
          style={styles.instructionText} onClick={this._goToNextTest} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([2, -30, -15])} text={"Release Menu"}
          style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
          transformBehaviors={["billboard"]}/>
      </ViroARScene>
    );
  },
  _getPlane() {
    let planes = []
    if(this.state.addPlane) {
      planes.push((
        <ViroARPlane
            minHeight={this.state.minValue}
            minWidth={this.state.minValue}
            key={this.state.useFirstPlane ? "firstPlane" : "secondPlane"}
            onClick={this._setTextNatively("onClick")}
            onHover={this._setTextNatively("onHover")}
            onDrag={this._setTextNatively("onDrag")}
            onPinch={this._setTextNatively("onPinch")}
            onRotate={this._setTextNatively("onRotate")}
            onAnchorFound={this._onAnchorFound}
            onAnchorUpdated={this._onAnchorUpdated}
            onAnchorRemoved={this._onAnchorRemoved} >

          <ViroSurface
            position={this.state.updateMap.center}
            rotation={[-90, 0, 0]}
            scale={[this.state.updateMap.width, this.state.updateMap.height, 1]}
            materials={this.state.useFirstPlane ? "blue_plane" : "red_plane"}/>

        </ViroARPlane>));
    }
    return planes;
  },
  _increaseMinValue() {
    this.setState({
      minValue : (this.state.minValue >= 3 ? 0 : this.state.minValue + .3),
    })
  },
  _onAnchorFound() {
    this.setState({
      found : true,
    });
  },
  _onAnchorUpdated(updateMap) {
    this.setState({
      updateMap
    })
  },
  _onAnchorRemoved() {
    this.setState({
      removed : true,
    })
  },
  _getFoundText() {
    return this.state.found ? "Plane Found" : "Plane NOT Found";
  },
  _getUpdatedText() {
    return this.state.updateMap ? ("Width: " + this.state.updateMap.width + ", Height: " + this.state.updateMap.height) : "No Updates";
  },
  _getRemovedText() {
    return this.state.removed ? "Plane Removed" : "Plane NOT Removed";
  },
  /*
   We set text natively because some events update very quickly
   */
  _setTextNatively(callbackType) {
    return ()=> {
      this._callbackText.setNativeProps({"text" : "Callback: " + callbackType})
    }
  },
  _addRemovePlane() {
    this.setState({
      addPlane : !this.state.addPlane
    });
  },
  _switchPlane() {
    this.setState({
      useFirstPlane : !this.state.useFirstPlane
    })
  },
  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARPlaneManualTest", {scene:require("./ARPlaneManualTest")})
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

ViroMaterials.createMaterials({
  blue_plane: {
    lightingModel: "Constant",
    diffuseColor: "#0000ff50"
  },
  red_plane: {
    lightingModel: "Constant",
    diffuseColor: "#ff000050"
  },
});

module.exports = testARScene;
