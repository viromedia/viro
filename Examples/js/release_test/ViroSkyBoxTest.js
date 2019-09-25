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
 } from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;

var createReactClass = require('create-react-class');

var ReleaseMenu = require("./ReleaseMenu.js");

var Uri360Image = {uri:"https://lh3.googleusercontent.com/dB3Dvgf3VIglusoGJAfpNUAANhTXW8K9mvIsiIPkhJUAbAKGKJcEMPTf0mkSexzLM5o=w300"};
var Local360Image = require("../res/sun_2302.jpg");

 var ViroSkyBoxTest = createReactClass({
   getInitialState() {
     return {
        color:"#098765",
        format:"RGBA8",
        showColorBackground:false,
        showUrlImage:false
     };
   },

   render: function() {
     var image = this.state.showUrlImage ? Uri360Image : Local360Image;
     if (this.state.showColorBackground){
      return (
            <ViroScene>

            <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

             <ViroSkyBox
                 color={this.state.color}
                 format={this.state.format}
                 onLoadStart={this._onLoadStart}
                 onLoadEnd={this._onLoadEnd}/>
                 {this._getTestControls()}

            <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />


            </ViroScene>

           );
     } else {
      return (
            <ViroScene>
            <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

             <ViroSkyBox source={{nx:image,
              px:image,
              ny:image,
              py:image,
              nz:image,
              pz:image}} format={this.state.format} onLoadStart={this._onLoadStart} onLoadEnd={this._onLoadEnd}/>
              {this._getTestControls()}

              <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["Billboard"]} onClick={this._showNext} />

            </ViroScene>

           );
     }
   },

   _getTestControls(){
     return(
         <ViroNode position={[0,-0.5,-3]}>
             <ViroText style={styles.baseTextTwo}  position={[-2,0, 0]} width={2} height ={2}
                    text={"ToggleImageSource isUrlIamge: " + this.state.showUrlImage}
                    onClick={this._toggleImageSource}/>

             <ViroText style={styles.baseTextTwo}  position={[0,0, 0]} width={2} height ={2}
                      text={"ToggleImageColor with color: " + this.state.color} textLineBreakMode='Justify'
                                    onClick={this._toggleImageColor}/>

             <ViroText style={styles.baseTextTwo}  position={[2,0, 0]} width={2} height ={2}
                    text={"Toggle Format " + this.state.format}
                    onClick={this._toggleFormat}/>
          </ViroNode>
        );
   },

   _showNext() {
      this.props.sceneNavigator.replace({scene:require('./Viro360ImageTest')});
   },

   _toggleImageSource() {
      var newShowUrlImageFlag = !this.state.showUrlImage;
      this.setState({
          showUrlImage:newShowUrlImageFlag,
          showColorBackground:false
      });
   },

   _toggleImageColor() {
      var newColor = "#098765";
      if (this.state.color == newColor){
        newColor = 0xffff;
      }

      this.setState({
        color:newColor,
        showColorBackground:true
      });
   },

   _toggleFormat() {
        var newState;
        if (this.state.format == "RGBA8"){
            newState="RGB565";
        } else if (this.state.format == "RGB565"){
            newState="RGBA8";
        }
         this.setState({
                            format:newState
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
      fontSize: 10,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});

 module.exports = ViroSkyBoxTest;
