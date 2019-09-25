/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *//**
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
   ViroQuad,
   ViroSkyBox,
   ViroSphere,
   Viro3DObject,
   ViroSpinner,
 } from 'react-viro';

 let polarToCartesian = ViroUtils.polarToCartesian;

var createReactClass = require('create-react-class');

 // **This test has not been done.  This is placeholder for scene and navigation arrows**

 var ReleaseMenu = require("./ReleaseMenu.js");

 var ViroQuadFlexViewTest = createReactClass({

   getInitialState() {
     return {
        spinnerType:'Dark',
        fontSize:10,
        newWidth:1,
        newHeight:1,
        mirrored : false,
     };
   },

   render: function() {
     return (
      <ViroScene>
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

      <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>
      <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

        <ViroNode position={[0,0,-3]}>
                  <ViroFlexView
                      position={[1, 1, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      materials={["redColor"]}
                      width={this.state.newWidth}
                      height={this.state.newHeight} />

                  <ViroSpinner
                      type={this.state.spinnerType}
                      position={[0,1,0]}
                      scale={[0.3, 0.3, 0.3]}/>

                  <ViroQuad
                      position={[-1, 1, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      uvCoordinates={[0,0,2,2]}
                      materials={this.state.mirrored ? "grid_mirrored" : "grid"}
                      width={this.state.newWidth}
                      height={this.state.newHeight} />

                 <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[-2, 0, 0]} width={4} height ={2}
                                text={"Toggle Height / Width" + this.state.newHeight} onClick={this._toggleWidthHeight}/>
                 <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[0, 0, 0]} width={4} height ={2}
                                text={"Toggle Spinner theme: " + this.state.spinnerType} onClick={this._toggleSpinner}/>
                 <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[2, 0, 0]} width={4} height ={2}
                                text={"Toggle Grid Mirror: " + this.state.mirrored} onClick={this._toggleMirror}/>

         </ViroNode>
      </ViroScene>

     );
   },

   _showNext() {
     this.props.sceneNavigator.replace({scene:require('./ViroImageTest')});
   },

   _toggleSpinner(){
        var newSpinnerType = this.state.spinnerType == "Dark" ? "Light" : "Dark";
        this.setState({
                    spinnerType:newSpinnerType
                   });
   },

  _toggleMirror() {
    this.setState({
      mirrored : !this.state.mirrored
    })
  },

   _toggleWidthHeight(){
        var height = this.state.newHeight + 1;
        var width = this.state.newWidth + 1;

        if (width > 3){
            width = 1;
        }

        if (height > 3){
            height = 1;
        }

        this.setState({
                            newHeight:height,
                            newWidth:width
                           });
   }

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
  baseTextTwo: {
      fontFamily: 'Arial',
      fontSize: 20,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});
ViroMaterials.createMaterials({
  redColor: {
    fresnelExponent: .5,
    shininess: 2.0,
    diffuseColor: "#ff0000"
  },
  blue: {
      shininess: 2.0,
      lightingModel: "Lambert",
      diffuseColor: "#0000ff"
    },
  heart: {
    lightingModel: "Phong",
    diffuseTexture: require('../res/heart_d.jpg'),
  },
  grid: {
    diffuseTexture: require('./res/grid_bg.jpg'),
    wrapS: "Repeat",
    wrapT: "Repeat"
  },
  grid_mirrored: {
    diffuseTexture: require('./res/grid_bg.jpg'),
    wrapS: "Mirror",
    wrapT: "Mirror"
  }
 });
 module.exports = ViroQuadFlexViewTest;
