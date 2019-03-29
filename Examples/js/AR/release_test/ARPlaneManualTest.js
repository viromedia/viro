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
  componentDidMount() {
    this.numAnchors = 0;
    this.anchors = {};
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
      <ViroARScene onAnchorFound={this._onSceneAnchorFound}
                   onAnchorUpdated={this._onSceneAnchorUpdated}
                   onAnchorRemoved={this._onSceneAnchorRemoved}>

        {this._getPlanes()}

        <ViroText position={polarToCartesian([6, 30, 10])} text={!this.state.addPlane1 ? "Add 1st Plane" : "Remove 1st Plane"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._toggleFirstPlane}/>
        <ViroText position={polarToCartesian([6, 30, 0])} text={this._getSecondPlaneText()}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._toggleSecondPlane}/>
        <ViroText position={polarToCartesian([6, 30, -10])} text={!this.state.addPlane3 ? "Add Default Green Plane" : "Remove Default Green Plane"}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._toggleThirdPlane}/>

        <ViroText position={polarToCartesian([6, 0, 10])} text={this._getFoundText()}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, 0])} text={this._getUpdatedText()}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, -10])} text={this._getRemovedText()}
          style={styles.instructionText} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 0, -20])} text={"No Callback"}
          style={styles.instructionText} transformBehaviors={["billboard"]}
          ref={component=>{this._callbackText = component}}/>

        <ViroText position={polarToCartesian([6, 90, 10])} text={"Reset Tracking!"} rotation={[0, 90, 0]}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._resetStuff(true, false)}/>
        <ViroText position={polarToCartesian([6, 90, 0])} text={"Reset Anchors!"} rotation={[0, 90, 0]}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._resetStuff(false, true)}/>
        <ViroText position={polarToCartesian([6, 90, -10])} text={"Reset All!"} rotation={[0, 90, 0]}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._resetStuff(true, true)}/>

        <ViroText position={polarToCartesian([6, -90, 10])} text={"Reset Tracking!"} rotation={[0, -90, 0]}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._resetStuff(true, false)}/>
        <ViroText position={polarToCartesian([6, -90, 0])} text={"Reset Anchors!"} rotation={[0, -90, 0]}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._resetStuff(false, true)}/>
        <ViroText position={polarToCartesian([6, -90, -10])} text={"Reset All!"} rotation={[0, -90, 0]}
          style={styles.instructionText} transformBehaviors={["billboard"]} onClick={this._resetStuff(true, true)}/>

        {/* Release Menu */}
        <ViroText position={polarToCartesian([6, -30, 0])} text={"Next test"}
          style={styles.instructionText} onClick={this._goToNextTest} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, -30, -15])} text={"Release Menu"}
          style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
          transformBehaviors={["billboard"]}/>
      </ViroARScene>
    );
  },
  _resetStuff(resetTracking, removeAnchors) {
    return () => {
      this.props.arSceneNavigator.resetARSession(resetTracking, removeAnchors);
    }
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
  // anchor #1 is at this.anchors[1]... and so on.
  _onSceneAnchorFound(anchor) {
    console.log("ARPlaneManualTest - anchor found " + anchor.anchorId)
    if (anchor.type != "plane"){
      return;
    }

    this.numAnchors++;
    this.anchors[this.numAnchors] = anchor;
    this.setState({
      reloadFlag : !(this.state.reloadFlag)
    })
  },
  _onSceneAnchorUpdated(anchor) {
    console.log("ARPlaneManualTest - anchor updated " + anchor.anchorId);
  },
  _onSceneAnchorRemoved(anchor) {
    console.log("ARPlaneManualTest - anchor removed " + anchor.anchorId);

    if (anchor.type != "plane"){
      return;
    }
    for(var i = 1; i <= this.numAnchors; i++) {
      if (this.anchors[i].anchorId == anchor.anchorId) {
        this.anchors[i] = undefined;
        for (; i < this.numAnchors; i++) {
          this.anchors[i] = this.anchors[i+1]
        }
        break;
      }
    }
    this.numAnchors--;
    this.setState({
      reloadFlag : !(this.state.reloadFlag)
    })
  },
  _getPlanes() {
    let planes = []
    if(this.state.addPlane1 && this.anchors[1]) {
      planes.push((
        <ViroARPlane
          anchorId={this.anchors[1].anchorId}
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

        <ViroQuad
          rotation={[-90, 0, 0]}
          scale={[.5,.5,1]}
          materials={"blue_plane"}/>

      </ViroARPlane>));
    }

    // Show the 2nd plane if the it's to use the 1st plane ID and 1 plane has been found
    // OR if it should use the 2nd plane ID and a 2nd plane has been found.
    if ((this.state.secondPlaneFlag == 1 && this.anchors[1]) || (this.state.secondPlaneFlag == 2 && this.anchors[2])) {
      planes.push((
        <ViroARPlane
          anchorId={this.state.secondPlaneFlag == 2 ? this.anchors[2].anchorId : this.anchors[1].anchorId}
          key={"secondPlane"} >

          <ViroQuad
            rotation={[-90, 0, 0]}
            scale={[.5,.5,1]}
            materials={"red_plane"}/>

        </ViroARPlane>));
    }

    if (this.state.addPlane3) {
      planes.push((
        <ViroARPlane
          key={"thirdPlane"} >

          <ViroQuad
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
    return this.numAnchors == 0 ? "Plane NOT Found" : (this.numAnchors + " Planes Found");
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
  green_plane: {
    lightingModel: "Constant",
    diffuseColor: "#00ff0050"
  },
});

module.exports = testARScene;
