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

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var testARScene = createReactClass({
  mixins: [TimerMixin],
  componentWillMount() {
    if (!global.anchors) {
      global.anchors = {}
      global.anchors.keys = []
    }
  },
  getInitialState: function() {
    return {
      addPlane1 : false,
      secondPlaneFlag : 0,
      addPlane3 : true,
      minValue : 0,
    }
  },
  render: function() {
    return (
      <ViroARScene onAnchorFound={this._onSceneAnchorFound} onAnchorRemoved={global.onAnchorRemoved} >

        {this._getPlanes()}

        <ViroText position={polarToCartesian([2, 30, 10])} text={!this.state.addPlane1 ? "Add 1st Plane" : "Remove 1st Plane"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._toggleFirstPlane}/>
        <ViroText position={polarToCartesian([2, 30, 0])} text={this._getSecondPlaneText()}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._toggleSecondPlane}/>
        <ViroText position={polarToCartesian([2, 30, -10])} text={!this.state.addPlane3 ? "Add Default Green Plane" : "Remove Default Green Plane"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._toggleThirdPlane}/>

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
  _getSecondPlaneText() {
    if (this.state.secondPlaneFlag == 0) {
      return "Add 2nd Plane w/ 1st ID";
    } else if (this.state.secondPlaneFlag == 1) {
      return "Add 2nd Plane w/ 2nd ID";
    } else if (this.state.secondPlaneFlag == 2) {
      return "Remove Second Plane";
    }
  },
  _getThirdPlaneText() {

  },
  // anchor #1 is at global.anchors.keys[0] and so on
  _onSceneAnchorFound(anchor) {
    global.onAnchorFound(anchor);
    this.setState({
      reloadFlag : !(this.state.reloadFlag)
    })
  },
  _getPlanes() {
    let planes = []
    if(this.state.addPlane1 && global.anchors.keys[0]) {
      planes.push((
        <ViroARPlane
          anchorId={global.anchors[global.anchors.keys[0]].anchorId}
          minHeight={this.state.minValue}
          minWidth={this.state.minValue}
          key={"firstPlane"}
          onClick={this._setTextNatively("onClick")}
          onHover={this._setTextNatively("onHover")}
          onDrag={this._setTextNatively("onDrag")}
          onPinch={this._setTextNatively("onPinch")}
          onRotate={this._setTextNatively("onRotate")}
          onAnchorFound={this._onAnchorFound}
          onAnchorUpdated={this._onAnchorUpdated}
          onAnchorRemoved={this._onAnchorRemoved} >

        <ViroSurface
          rotation={[-90, 0, 0]}
          scale={[.5,.5,1]}
          materials={"blue_plane"}/>

      </ViroARPlane>));
    }

    if (this.state.secondPlaneFlag > 0 && global.anchors.keys[1]) {
      planes.push((
        <ViroARPlane
          anchorId={this.state.secondPlaneFlag == 2 ? global.anchors[global.anchors.keys[1]].anchorId : global.anchors[global.anchors.keys[0]].anchorId}
          key={"secondPlane"} >

          <ViroSurface
            rotation={[-90, 0, 0]}
            scale={[.5,.5,1]}
            materials={"red_plane"}/>

        </ViroARPlane>));
    }

    if (this.state.addPlane3) {
      planes.push((
        <ViroARPlane
          key={"thirdPlane"} >

          <ViroSurface
            rotation={[-90, 0, 0]}
            scale={[.5,.5,1]}
            materials={"green_plane"}/>

        </ViroARPlane>));
    }

    return planes;
  },
  _toggleFirstPlane() {
    this.setState({
      addPlane1 : !this.state.addPlane1
    });
  },
  _toggleSecondPlane() {
    this.setState({
      secondPlaneFlag : this.state.secondPlaneFlag == 2 ? 0 : (this.state.secondPlaneFlag + 1)
    })
  },
  _toggleThirdPlane() {
    this.setState({
      addPlane3 : !this.state.addPlane3
    });
  },
  _onAnchorFound(anchor) {
    this.setState({
      found : true,
      updateMap : anchor
    });
  },
  _onAnchorUpdated(anchor) {
    this.setState({
      updateMap : anchor
    })
  },
  _onAnchorRemoved() {
    this.setState({
      removed : true,
    })
  },
  _getFoundText() {
    return global.anchors.keys.length == 0 ? "Plane NOT Found" : (global.anchors.keys.length + " Planes Found");
  },
  _getUpdatedText() {
    if (this.state.updateMap) {
      return "#1 Width: " + this.state.updateMap.width + ", Height: " + this.state.updateMap.height;
    } else {
      return "No Updates"
    }
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
  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARHitTest", {scene:require("./ARHitTest")})
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
  green_plane: {
    lightingModel: "Constant",
    diffuseColor: "#00ff0050"
  },
});

module.exports = testARScene;
