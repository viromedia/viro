/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 */

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
  ViroQuad,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
  ViroPolyline
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var ReleaseMenu = require("./ReleaseMenu.js");

var Uri360Image = {uri:"https://lh3.googleusercontent.com/dB3Dvgf3VIglusoGJAfpNUAANhTXW8K9mvIsiIPkhJUAbAKGKJcEMPTf0mkSexzLM5o=w300"};
var Local360Image = require("../res/sun_2302.jpg");

var points = [[[0,0,0]],
              [[0,0,0], [.5,.5,-.5]],
              [[0,0,0], [.5,.5,-.5], [1,0,0]],
              [[0,0,0], [.5,.5,-.5], [1,0,0], [1.5,1,.5]],
              [[0,0,0], [.5,.5,-.5], [1,0,0], [1.5,1,.5], [5,5,0]]];

var ViroPolylineTest = createReactClass({
  getInitialState() {
    return {
      materials:false,
      closed: false,
      pointSet:0,
      thickness: .05,
      orbit : false,
    };
  },
  render: function() {
    var image = this.state.showUrlImage ? Uri360Image : Local360Image;
    return (
      <ViroScene>
        <ViroCamera position={[0, 0, 0]} active={!this.state.orbit}/>
        <ViroOrbitCamera focalPoint={[0, -.25, -2]} active={this.state.orbit}/>
        <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
        <ViroPolyline position={[0,0,-2]} points={this._getPoints()} closed={this.state.closed} thickness={this.state.thickness}
          materials={this.state.materials ? "whitePolyline" : "redPolyline"} onClick={()=>{this.setState({orbit : !this.state.orbit})}}/>
        {this._getTestControls()}
      </ViroScene>
    );
  },

  _getTestControls(){
    return(
      <ViroNode position={[0,-0.5,-3]}>
        <ViroText style={styles.baseTextTwo}  position={[0, 0, 0]} width={2} height ={2}
                  text={"Toggle Points: " + (this.state.pointSet + 1)}
                  onClick={this._nextPoints}/>

        <ViroText style={styles.baseTextTwo}  position={[0, -.5, 0]} width={2} height ={2}
                  text={"Toggle Material : " + (this.state.materials ? " whitePolyline" : "redPolyline")} textLineBreakMode='Justify'
                  onClick={this._toggleMaterial}/>

        <ViroText style={styles.baseTextTwo}  position={[0, -1, 0]} width={2} height ={2}
                  text={"Toggle Thickness: " + this.state.thickness}
                  onClick={this._toggleThickness}/>

        <ViroText style={styles.baseTextTwo}  position={[2.1, -1, 0]} width={2} height ={2}
                  text={"Toggle Closed: " + this.state.closed}
                  onClick={this._toggleClosed}/>
      </ViroNode>
    );
  },

  _nextPoints() {
    this.setState({
      pointSet : this.state.pointSet == 4 ? 0 : ++this.state.pointSet
    })
  },

  _getPoints() {
    return points[this.state.pointSet];
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./Viro360ImageTest')});
  },

  _toggleMaterial() {
    this.setState({
      materials : !this.state.materials
    })
  },

  _toggleThickness() {
    this.setState({
      thickness : this.state.thickness >= .5 ? .05 : this.state.thickness + .05
    })
  },

  _toggleClosed() {
    this.setState({
      closed: !this.state.closed,
    });
  },
  _onLoadStart() {
    console.log("ViroSkyBoxtest: _onLoadStart");
  },

  _onLoadEnd() {
    console.log("ViroSkyBoxtest: _onLoadEnd");
  },
});

var styles = StyleSheet.create({
  baseTextTwo: {
      fontFamily: 'Arial',
      fontSize: 15,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});

ViroMaterials.createMaterials({
  whitePolyline: {
    diffuseColor: "#ffffff",
  },
  redPolyline: {
    diffuseColor: "#ff0000",
  },
});

module.exports = ViroPolylineTest;
