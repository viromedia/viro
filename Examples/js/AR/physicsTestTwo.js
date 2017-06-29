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
  ViroSkyBox,
  Viro360Video,
  ViroText,
  ViroSurface
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var testARScene = React.createClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      text : "not tapped",
      boxes: [],
      count : 0,
    }
  },
  render: function() {
    return (
        <ViroARScene ref="arscene" reticleEnabled={false}>

          <ViroVideo
            height={.2} width={.2} position={[0,.15,-.5]}
            onClick={()=>{this._onTap(); this._addOneBox()}}
            source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}}
            transformConstraints={"billboard"}
          />
        
          <ViroText style={styles.welcome} position={[0, .5, -1]} text={this.state.text} />


          {this._getARPlanes()}
          {this._getBoxes()}
        </ViroARScene>
    );
  },
  _getARPlanes() {
    let prefix = "plane"
    let planes = []
    for (var i = 0; i < 15; i++) {
      var color;
      if (i%3 == 0) {
        color = "blue"
      } else if (i%3 == 1) {
        color = "red"
      } else {
        color = "green"
      }
      planes.push((
        <ViroARPlane key={prefix + i} >
          <ViroBox width={.5} height={.1} length={.5} position={[0,0,0]} rotation={[0,0,0]}
            physicsBody={{
                      type:'static', restitution:0.8
                    }}
            onClick={this._addOneBox}
            materials={color}/>
        </ViroARPlane>
      ));
    }
    return planes;
  },
  _getBoxes() {
    let prefix = "box"
    let boxes = [];
    for (var i = 0; i < this.state.boxes.length; i++) {
      boxes.push((<ViroBox key={prefix + i} position={this.state.boxes[i]} width={.1} height={.1} length={.1}
              onDrag={()=>{}}
              physicsBody={{
                type:'dynamic',
                mass:1
              }} />));
    }
    return boxes;
  },
  _addOneBox() {
    console.log("kirby add one box");
    let position = this.refs["arscene"].getCameraPositionAsync().then((position) => {
        console.log("kirby tap box count: " + this.state.count + ", " + position[0] + ", " + position.x);
        this.state.boxes.push([position[0], 3, position[2] - 1])
        this.setState({
          //count : this.state.count++,
          boxes : this.state.boxes
        })
    });
  },
  _onTap() {
    console.log("kirby tapped video!!!!")
    this.setState({
      text : "tapped!"
    })
    this.setTimeout( () => {
      this.setState({
        text : "not tapped"
      });
    }, 1000);
  },
  _onLoadStart(component) {
    return () => {
      console.log("scene1 " + component + " load start");
    }
  },
  _onLoadEnd(component) {
    return () => {
      console.log("scene1 " + component + " load end");
    }
  },
  _onFinish(component) {
    return () => {
      console.log("scene1 " + component + " finished");
    }
  }
});

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#FFFFFF',
  },
  welcome: {
    fontSize: 13,
    textAlign: 'center',
    color: '#ffffff',
    margin: 2,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

ViroMaterials.createMaterials({
  blue: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#0000ff"
  },
  green: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#00ff00"
  },
  red: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#ff0000"
  },
  wework_title: {
    shininess: 1.0,
    lightingModel: "Constant",
    diffuseTexture: {"uri": "https://s3-us-west-2.amazonaws.com/viro/guadalupe_360.jpg"},
    diffuseTexture: require("../res/new_menu_screen.jpg"),
    fresnelExponent: .5,
  },
  box_texture: {
    diffuseTexture: require("../res/sun_2302.jpg"),
  }
});

module.exports = testARScene;
