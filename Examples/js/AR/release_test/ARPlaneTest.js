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
  ViroQuad,
  ViroConstants,
  ViroVideo,
  ViroText,
  ViroUtils
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var testARScene = createReactClass({
  getInitialState: function() {
    return {
      pauseUpdates: false,
      addPlane : true,
      useFirstPlane : true,
      updateMap : {width:0, height:0, position:[0,0,0]}, // used to update render props
      displayMap : {width:0, height:0, position:[0,0,0]}, // used to display info
      minValue : 0,
      detectType : 0,
    }
  },
  render: function() {
    return (
      <ViroARScene position={[0,0,0]} reticleEnabled={false} anchorDetectionTypes={this._getDetectType()} >

        {this._getPlane()}

        <ViroText position={polarToCartesian([6, 30, 10])} text={!this.state.addPlane ? "Add Plane" : "Remove Plane"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._addRemovePlane}/>
        <ViroText position={polarToCartesian([6, 30, 0])} text={"Switch Plane"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._switchPlane}/>
        <ViroText position={polarToCartesian([6, 30, -10])} text={"Min Width/Height: " + this.state.minValue}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._increaseMinValue}/>
        <ViroText position={polarToCartesian([6, 30, -20])} text={this.state.pauseUpdates ? "Resume Updates" : "Pause Updates"}
          style={styles.instructionText} transformBehaviors={["billboard"]}
          onClick={()=>{this.setState({pauseUpdates : !this.state.pauseUpdates})}}/>
        <ViroText position={polarToCartesian([6, 30, -30])} text={"Detect Type: " + this._getDetectType()}
          style={styles.instructionText} transformBehaviors={["billboard"]}
          onClick={this._changeDetectType}/>

        <ViroText position={polarToCartesian([6, 0, 10])} text={this._getFoundText()}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, 0])} text={this._getUpdatedText()}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, -10])} text={this._getRemovedText()}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, -20])} text={"No Callback"}
          style={styles.instructionText} transformBehaviors={["billboard"]}
          ref={component=>{this._callbackText = component}}/>

        {/* Release Menu */}
        <ViroText position={polarToCartesian([6, -30, 0])} text={"Next test"}
          style={styles.instructionText} onClick={this._goToNextTest} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, -30, -15])} text={"Release Menu"}
          style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
          transformBehaviors={["billboard"]}/>
      </ViroARScene>
    );
  },
  _changeDetectType() {
    this.setState({
      detectType : (this.state.detectType == 2) ? 0 : this.state.detectType + 1
    })
  },
  _getDetectType() {
    if (this.state.detectType == 0) {
      return "PlanesHorizontal"
    } else if (this.state.detectType == 1) {
      return "PlanesVertical"
    } else {
      return "None"
    }
  },
  _getPlane() {
    let planes = []
    if(this.state.addPlane) {
      planes.push((
        <ViroARPlane
            minHeight={this.state.minValue}
            minWidth={this.state.minValue}
            alignment={this.state.detectType == 1 ? "Vertical" : "Horizontal"}
            pauseUpdates={this.state.pauseUpdates}
            key={this.state.useFirstPlane ? "firstPlane" : "secondPlane"}
            onClick={this._setTextNatively("onClick")}
            onHover={this._setTextNatively("onHover")}
            onDrag={this._setTextNatively("onDrag")}
            onPinch={this._setTextNatively("onPinch")}
            onRotate={this._setTextNatively("onRotate")}
            onAnchorFound={this._onAnchorFound}
            onAnchorUpdated={this._onAnchorUpdated}
            onAnchorRemoved={this._onAnchorRemoved} >

          <ViroQuad
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
    let newState = {};
    newState["displayMap"] = updateMap;
    if (!this.state.pauseUpdates) {
      newState["updateMap"] = updateMap;
    }
    this.setState(newState);
  },
  _onAnchorRemoved() {
    this.setState({
      removed : true,
    })
  },
  _getFoundText() {
    return "Plane Found: " + (this.state.found ? "true" : "false");
  },
  _getUpdatedText() {
    return this.state.displayMap ? ("Width: " + this.state.displayMap.width + ", Height: " + this.state.displayMap.height) : "No Updates";
  },
  _getRemovedText() {
    return "Plane Removed: " + (this.state.removed ? "true" : "false");
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
      fontSize: 25,
      width: 2,
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
