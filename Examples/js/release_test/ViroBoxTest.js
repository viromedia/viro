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

let polarToCartesian = ViroUtils.polarToCartesian;

var ViroAnimatedComponentTest = require('./ViroAnimatedComponentTest');
var ViroButtonTest = require('./ViroButtonTest');

// **This test has not been done.  This is placeholder for scene and navigation arrows**

var ViroBoxTest = React.createClass({

  getInitialState() {
    return {
      runAnimation1:false,
      runAnimation2:false,
      runAnimation3:false,
      runAnimation4:false,
      toggleWHL:1,
    };
  },

  render: function() {
    return (
     <ViroScene>
     <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

      <ViroAnimatedComponent
      animation="loopRotate"
      run={this.state.runAnimation1}
      loop={true}>
      <ViroBox
        position={[-2, 2, -4]}
        scale={[1,1,1]}
        materials={["box1"]}
        rotation={[0,0,0]}
        height={1*this.state.toggleWHL}
        width={1*this.state.toggleWHL}
        length={1*this.state.toggleWHL}
        opacity={.5}
        transformBehaviors={["billboard"]}
        onHover={this._startAnimation1}
        />
      </ViroAnimatedComponent>

      <ViroAnimatedComponent
      animation="loopRotate"
      run={this.state.runAnimation2}
      loop={true}>
      <ViroBox
        position={[2, 2, -4]}
        scale={[1,1,1]}
        materials={["box1"]}
        rotation={[0,45,0]}
        height={1*this.state.toggleWHL}
        width={1*this.state.toggleWHL}
        length={1*this.state.toggleWHL}
        transformBehaviors={["billboardX"]}
        onClick={this._startAnimation2}
        />
      </ViroAnimatedComponent>



      <ViroAnimatedComponent
      animation="loopRotate"
      run={this.state.runAnimation3}
      loop={true}>
      <ViroBox
        position={[-2, -2, -4]}
        scale={[2,2,2]}
        materials={["box1"]}
        rotation={[0,0,0]}
        height={1*this.state.toggleWHL}
        width={1*this.state.toggleWHL}
        length={1*this.state.toggleWHL}
        transformBehaviors={["billboardY"]}
        onScroll={this._startAnimation3}
        />
      </ViroAnimatedComponent>
      
    <ViroText fontSize={this.state.fontSize} style={styles.centeredText} position={[0,-3, -3]} width={2} height ={2} fontFamily={'Arial'}
        text={"Toggle WHL: " + this.state.toggleWHL} textLineBreakMode='justify' onClick={this._toggleWHL}/>



      <ViroImage source={require('./res/icon_left_w.png')} position={[-2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showPrevious} />
      <ViroText text="ViroBox" position={[0, -5, -3]} transformBehaviors={["billboard"]} />
      <ViroImage source={require('./res/icon_right_w.png')} position={[2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showNext} />

     </ViroScene>

    );
  },
  _toggleWHL(){
    var newWhlScale = this.state.toggleWHL + 0.4;
    if (newWhlScale > 2){
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

  _showNext() {
    this.props.sceneNavigator.push({scene:ViroButtonTest});
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
     shininess : 2.0,
     lightingModel: "Blinn",
     diffuseTexture: require('./res/earth.png'),
   },

});


module.exports = ViroBoxTest;
