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
   ViroAnimatedComponent,
   ViroQuad,
   ViroSkyBox,
   ViroSphere,
   Viro3DObject,
 } from 'react-viro';

 let polarToCartesian = ViroUtils.polarToCartesian;

var createReactClass = require('create-react-class');

 // **This test has not been done.  This is placeholder for scene and navigation arrows**

 var ReleaseMenu = require("./ReleaseMenu.js");

 var ViroLightTest = createReactClass({

   getInitialState() {
     return {
        lightPosHeight:4,
        lightAttenuationDistanceStart:3.5,
        lightInnerAngle:5,
        lightOuterAngle:30,
        lightColor0:"#ff8888",
        lightColor1:"#88ff88",
        sceneColor:"#ffffff",
        lightDirection:[0,-1,0],
        showAbientLight:false,
        showDirectionalLight:false
     };
   },

   _renderDirectionalLight(){
      if (!this.state.showDirectionalLight){
          return;
      }
      return(
         <ViroDirectionalLight direction={[0, 0, -1]} color={this.state.sceneColor}/>
      )
   },
   _renderAmbientLight(){
       if (!this.state.showAbientLight){
           return;
       }
       return(
          <ViroAmbientLight color={this.state.sceneColor}/>
       )
   },

   _toggleColorLights(){
     var newLightColor0 = "#ff8888";
     var newLightColor1 = "#88ff88";
     if (this.state.lightColor0 == "#ff8888"){
         newLightColor0 = "#88ff88";
         newLightColor1 = "#ff8888"
     }
     this.setState({
       lightColor0: newLightColor0,
       lightColor1: newLightColor1,
     });
   },
   _toggleColorScene(){
        var newSceneColor = "#ffffff";
        if (this.state.sceneColor == "#ffffff"){
            newSceneColor = "#ff0000";
        }
         this.setState({
                 sceneColor: newSceneColor
               });
   },
   _toggleSpotLightPosition(){
     var newlightPosHeight = this.state.lightPosHeight + .5;
     if (newlightPosHeight > 8){
         newlightPosHeight = 4;
     }
     this.setState({
       lightPosHeight: newlightPosHeight,
      });
   },
   _toggleSpotLightAttenuation(){
      var newlightAttenuationDistanceStart = this.state.lightAttenuationDistanceStart + .5;
      if (newlightAttenuationDistanceStart > 6){
          newlightAttenuationDistanceStart = 1;
      }
      this.setState({
        lightAttenuationDistanceStart: newlightAttenuationDistanceStart,
      });
   },
   _toggleSpotLightInnerAngle(){
       var newlightInnerAngle = this.state.lightInnerAngle + 5;
       if (newlightInnerAngle > 30){
           newlightInnerAngle = 5;
       }
       this.setState({
         lightInnerAngle: newlightInnerAngle,
       });
   },
   _toggleSpotLightOuterAngle(){
       var newlightOuterAngle = this.state.lightOuterAngle + 5;
       if (newlightOuterAngle > 40){
         newlightOuterAngle = 5;
       }

       this.setState({
         lightOuterAngle: newlightOuterAngle,
       });
   },
   _toggleAmbientLight(){
       this.setState({
         showAbientLight:!this.state.showAbientLight
       });
   },
   _toggleDirectionalLight(){
       this.setState({
         showDirectionalLight:!this.state.showDirectionalLight
       });
   },

   _renderLightControls(){
     return(
        <ViroNode position={[-0,1,-6]}>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText} position={[-2, 1, 0]} width={2} height ={2} fontFamily={'Arial'}
              text={"Toggle Color Scene"} textLineBreakMode='Justify' onClick={this._toggleColorScene}/>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText} position={[0,1, 0]} width={2} height ={2} fontFamily={'Arial'}
              text={"Toggle Spot / Omni Height " + this.state.lightPosHeight} textLineBreakMode='Justify' onClick={this._toggleSpotLightPosition}/>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText} position={[2, 1, 0]} width={2} height ={2} fontFamily={'Arial'}
              text={"Toggle Spot lightAttenuation  " + this.state.lightAttenuationDistanceStart + " - " + (this.state.lightAttenuationDistanceStart + 3)} textLineBreakMode='justify' onClick={this._toggleSpotLightAttenuation}/>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText} position={[-2,0, 0]} width={2} height ={2} fontFamily={'Arial'}
              text={"Toggle Spot Inner Angle " + this.state.lightInnerAngle} textLineBreakMode='Justify' onClick={this._toggleSpotLightInnerAngle}/>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText} position={[0, 0, 0]} width={2} height ={2} fontFamily={'Arial'}
              text={"Toggle Spot Outer Angle " + this.state.lightOuterAngle} textLineBreakMode='Justify' onClick={this._toggleSpotLightOuterAngle}/>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText} position={[2, 0, 0]} width={2} height ={2} fontFamily={'Arial'}
              text={"Toggle Ambient Light " + this.state.showAbientLight} textLineBreakMode='Justify' onClick={this._toggleAmbientLight}/>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText} position={[0, -1, 0]} width={1.5} height ={2} fontFamily={'Arial'}
              text={"Toggle Color Spot & Omni light"} textLineBreakMode='Justify' onClick={this._toggleColorLights}/>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText} position={[2, -1, 0]} width={2} height ={2} fontFamily={'Arial'}
              text={"Toggle Directional Light " + this.state.showDirectionalLight} textLineBreakMode='Justify' onClick={this._toggleDirectionalLight}/>
        </ViroNode>
      );
   },

   render: function() {
     return (
      <ViroScene>
      <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

          {this._renderAmbientLight()}
          {this._renderDirectionalLight()}
          {this._renderLightControls()}

          {
           /*
            The box to the far left, has a Constant lighting material so should
            be unaffected by any light.
            */
          }
          <ViroBox
            materials={["whiteColorConst"]}
            position={[-9, -2, -5]}
            scale={[1, 1, 1]}
            height={2}
            width={2}
            length={2}/>

            {
             /*
              The box to the immediate left. This box is meant to test the spotlight.
              It is also lit by the global ambient and directional lights.
              */
            }
          <ViroNode position={[-5,-2,-7]}>
              <ViroSpotLight
                  innerAngle={this.state.lightInnerAngle}
                  outerAngle={this.state.lightOuterAngle}
                  direction={this.state.lightDirection}
                  position={[0, this.state.lightPosHeight, 0]}
                  color={this.state.lightColor1}
                  attenuationStartDistance={this.state.lightAttenuationDistanceStart}
                  attenuationEndDistance={this.state.lightAttenuationDistanceStart + 3}/>

              <ViroBox
                materials={["whiteColorBlinn"]}
                position={[0, 0, 0]}
                scale={[1, 1, 1]}
                height={2}
                width={3}
                length={3}/>
          </ViroNode>

          {
           /*
            The large box in the middle, lit by an omni light and the ambient +
            directional global lights.
            */
          }
          <ViroNode position={[-1,-2,-7]}>
              <ViroOmniLight
                  position={[0, this.state.lightPosHeight, 0]}
                  color={this.state.lightColor0}
                  attenuationStartDistance={this.state.lightAttenuationDistanceStart}
                  attenuationEndDistance={this.state.lightAttenuationDistanceStart + 3} />

              <ViroBox
                materials={["whiteColorLambert"]}
                position={[0, 0, 0]}
                scale={[1, 1, 1]}
                height={2}
                width={3}
                length={3}/>
          </ViroNode>

          {
           /*
            These are the 3 boxes to the right of the viewer. They are lit by
            the omni light below, and the global ambient/directional lights.
            */
           }
          <ViroNode position={[4, -2, -7]}>
              {
               /*
                The spotlight within this node currently has no effect, because
                lights only impact their child nodes.
                */
              }
              <ViroNode position={[0, 5, 0]}>
                <ViroSpotLight
                    position={[0, 0, 0]}
                    innerAngle={this.state.lightInnerAngle}
                    outerAngle={this.state.lightOuterAngle}
                    direction={this.state.lightDirection}
                    color={"#8888FF"}
                    attenuationStartDistance={3.5}
                    attenuationEndDistance={5}/>
              </ViroNode>
              <ViroOmniLight
                    position={[0, 1, 2]}
                    color={"#ffffff"}
                    attenuationStartDistance={this.state.lightAttenuationDistanceStart}
                    attenuationEndDistance={this.state.lightAttenuationDistanceStart + 3} />

              <ViroBox
                    materials={["whiteColorLambert"]}
                    position={[-1.2, 0, 0]}
                    scale={[1, 1, 1]}
                    height={1}
                    width={1}
                    length={1}/>
              <ViroBox
                    materials={["whiteColorLambert"]}
                    position={[0, 0, 0]}
                    scale={[1, 1, 1]}
                    height={1}
                    width={1}
                    length={1}/>
              <ViroBox
                    materials={["whiteColorLambert"]}
                    position={[1.2, 0, 0]}
                    scale={[1, 1, 1]}
                    height={1}
                    width={1}
                    length={1}/>
         </ViroNode>

      </ViroScene>
     );
   },
   _showNext() {
       this.props.sceneNavigator.replace({scene:require('./GroupTestBasicProps')});
    },
 });

const styles = StyleSheet.create({
   centeredText: {
        fontSize: 20,
        fontFamily: 'Arial',
        color: '#ffffff',
   },
});

ViroMaterials.createMaterials({
  whiteColorConst:{
    diffuseColor: "#ffffff"
  },
  whiteColorLambert: {
    lightingModel: "Lambert",
    diffuseColor: "#ffffff"
  },
  whiteColorBlinn: {
    lightingModel: "Blinn",
    diffuseColor: "#ffffff",
    specularTexture: require('./res/specular.png')
  },
 });

 module.exports = ViroLightTest;
