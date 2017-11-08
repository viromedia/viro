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
  ViroSurface,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;
var UriImage = {uri:"https://lh3.googleusercontent.com/dB3Dvgf3VIglusoGJAfpNUAANhTXW8K9mvIsiIPkhJUAbAKGKJcEMPTf0mkSexzLM5o=w300"};
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroBoxTest = createReactClass({

  getInitialState() {
    return {
      runAnimation1:false,
      runAnimation2:false,
      runAnimation3:false,
      runAnimation4:false,
      toggleWHL:1,
      usingMaterialArray:false,
      currentMaterials:["box1"],
    };
  },

  render: function() {
    return (
      <ViroScene>
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
      <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

      <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

      <ViroAnimatedComponent animation="loopRotate" run={this.state.runAnimation1} loop={true}>
         <ViroBox
            position={[-2, 2, -4]}
            scale={[1,1,1]}
            materials={this.state.currentMaterials}
            rotation={[0,0,0]}
            height={1*this.state.toggleWHL}
            width={1*this.state.toggleWHL}
            length={1*this.state.toggleWHL}
            opacity={.5}
            transformBehaviors={["billboard"]}
            onHover={this._startAnimation1}
          />
      </ViroAnimatedComponent>

      <ViroAnimatedComponent animation="loopRotate" run={this.state.runAnimation2} loop={true}>
        <ViroBox
            position={[2, 2, -4]}
            scale={[1,1,1]}
            materials={this.state.currentMaterials}
            rotation={[0,45,0]}
            height={1*this.state.toggleWHL}
            width={1*this.state.toggleWHL}
            length={1*this.state.toggleWHL}
            transformBehaviors={["billboardX"]}
            onClick={this._startAnimation2}
        />
      </ViroAnimatedComponent>

      <ViroAnimatedComponent animation="loopRotate" run={this.state.runAnimation3} loop={true}>
        <ViroBox
            position={[-3, -3, -4]}
            scale={[2,2,2]}
            materials={this.state.currentMaterials}
            rotation={[0,0,0]}
            height={1*this.state.toggleWHL}
            width={1*this.state.toggleWHL}
            length={1*this.state.toggleWHL}
            transformBehaviors={["billboardY"]}
            onScroll={this._startAnimation3}
        />
      </ViroAnimatedComponent>

      <ViroAnimatedComponent animation="loopRotate" run={this.state.runAnimation4} loop={true}>
        <ViroBox
            position={[2, -1.3, -4]}
            scale={[1,1,1]}
            materials={this.state.currentMaterials}
            rotation={[0,0,0]}
            height={2*this.state.toggleWHL}
            width={4*this.state.toggleWHL}
            length={3*this.state.toggleWHL}
            transformBehaviors={["billboard"]}
            onSwipe={this._startAnimation4}
          />
      </ViroAnimatedComponent>

      <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />
      <ViroText fontSize={this.state.fontSize} style={styles.centeredText} position={[0,-4, -3]} width={2} height ={2} fontFamily={'Arial'}
          text={"Toggle WHL: " + this.state.toggleWHL} textLineBreakMode='justify' onClick={this._toggleWHL}/>

      <ViroText fontSize={this.state.fontSize} style={styles.centeredText} position={[0,-5, -3]} width={2} height ={2} fontFamily={'Arial'}
          text={"Toggle materials" } textLineBreakMode='justify' onClick={this._toggleMaterials}/>

     </ViroScene>
    );
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./ViroSphereTest')});
  },

  _toggleMaterials() {
    var newMat = !this.state.usingMaterialArray ? ["box2", "box2", "box2", "box1", "box1", "box1"] : ["box1"];
    var newUsingMaterialArray = !this.state.usingMaterialArray;
    this.setState({
        currentMaterials: newMat,
        usingMaterialArray: newUsingMaterialArray,
    });
  },

  _toggleWHL(){
    var newWhlScale = this.state.toggleWHL + 0.1;
    if (newWhlScale > 1.3){
        newWhlScale = 1;
    }
    this.setState({
        toggleWHL: newWhlScale,
    });
  },

  _showPrevious() {
    this.props.sceneNavigator.pop();
  },

  _startAnimation1() {
    this.setState({
        runAnimation1:true,
      });
  },

  _startAnimation2() {
    this.setState({
        runAnimation2:true,
      });
  },

  _startAnimation3() {
    this.setState({
        runAnimation3:true,
      });
  },

  _startAnimation4() {
    this.setState({
        runAnimation4:true,
      });
  },
});

ViroAnimations.registerAnimations({
  loopRotate:{properties:{rotateY:"+=10"}, duration:250},
});

var styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  elementText: {
    fontFamily: 'HelveticaNeue-Medium',
    fontSize: 30,
    color: '#ffffff',
    textAlign: 'center',
  },
  centeredText: {
    fontSize: 20,
    fontFamily: 'Arial',
    color: '#ffffff',
  },
});

ViroMaterials.createMaterials({
   box1: {
     bloomThreshold : 1.0,
     shininess : 2.0,
     lightingModel: "Blinn",
     diffuseTexture: require('./res/earth.png'),
   },
   box2: {
     shininess : 2.0,
     lightingModel: "Blinn",
     diffuseColor: "#0000FF",
   },
});


module.exports = ViroBoxTest;
