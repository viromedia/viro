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

var Uri360Image = {uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/360_horseshoe.jpg"};
var Local360Image = require("./res/360_park.jpg");

var placeholderImage = require("./res/grid_bg.jpg");
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroImageTest = createReactClass({
   getInitialState() {
     return {
        widthAndHeight:1,
        color:"#098765",
        format:"RGBA8",
        showColorBackground:false,
        showUrlImage:false,
        resizeMode:"ScaleToFill",
        clipMode:"ClipToBounds",
     };
   },

   render: function() {
     var image = this.state.showUrlImage ? Uri360Image : Local360Image;
      return (
            <ViroScene>
            <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

            <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

            <ViroImage
               width={this.state.widthAndHeight} height={this.state.widthAndHeight}
               format={this.state.format}
               mipmap={true}
               position={[-3, 4, -15]}
               scale={[6.5, 5.0, 0.1]}
               placeholderSource={placeholderImage}
               source={require("./res/stereotest.jpg")}
               stereoMode="LeftRight"
               onLoadStart={this._onLoadStart} onLoadEnd={this._onLoadEnd}
               />

             <ViroImage
                width={this.state.widthAndHeight} height={this.state.widthAndHeight}
                format={this.state.format}
                mipmap={true}
                position={[1, 1, -3]}
                scale={[0.5, 0.5, 0.1]}
                placeholderSource={placeholderImage}
                source={image}
                resizeMode={this.state.resizeMode}
                imageClipMode={this.state.clipMode}
                onLoadStart={this._onLoadStart} onLoadEnd={this._onLoadEnd}
                />
                {this._getTestControls()}

            </ViroScene>

           );
   },

   _getTestControls(){
    return(
     <ViroNode position={[0,-0.5,-3]}>
         <ViroText style={styles.baseTextTwo}  position={[-2,0, 0]} width={2} height ={2}
                text={"ToggleImageSource isUrlIamge: " + this.state.showUrlImage}
                onClick={this._toggleImageSource}/>

         <ViroText style={styles.baseTextTwo}  position={[0,0, 0]} width={2} height ={2}
                text={"ToggleImageSize " + this.state.widthAndHeight}
                onClick={this._toggleSize}/>

         <ViroText style={styles.baseTextTwo}  position={[2,0, 0]} width={2} height ={2}
                text={"Toggle Format " + this.state.format}
                onClick={this._toggleFormat}/>
         <ViroText style={styles.baseTextTwo}  position={[-2,-1, 0]} width={2} height ={2}
                text={"Toggle resizeMode " + this.state.resizeMode}
                onClick={this._toggleResizeMode}/>
        <ViroText style={styles.baseTextTwo}  position={[0,-1, 0]} width={2} height ={2}
                text={"Toggle clipMode " + this.state.clipMode}
                onClick={this._toggleImageClipMode}/>

      </ViroNode>
        );
   },

   _showNext() {
      this.props.sceneNavigator.replace({scene:require('./ViroTextTest')});
   },

   _toggleImageSource() {
        var newShowUrlImageFlag = !this.state.showUrlImage;
        this.setState({
                  showUrlImage:newShowUrlImageFlag,
        });
   },
   _toggleSize() {
        var size = this.state.widthAndHeight;
        size = size + 0.5;
        if (size > 4){
            size = 0.5;
        }

        this.setState({
            widthAndHeight:size
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
   _toggleResizeMode() {
        var newMode;
        if (this.state.resizeMode == "ScaleToFit") {
          newMode="ScaleToFill";
        } else if (this.state.resizeMode == "ScaleToFill") {
          newMode="StretchToFill";
        } else if (this.state.resizeMode == "StretchToFill") {
          newMode="ScaleToFit";
        }
        this.setState({
          resizeMode:newMode
        });
   },
   _toggleImageClipMode() {
        var mode;
        if (this.state.clipMode == "ClipToBounds") {
          mode = "None";
        } else if (this.state.clipMode == "None") {
          mode = "ClipToBounds";
        }
        this.setState({
          clipMode:mode
        });
   },
   _onLoadStart() {
        console.log("ViroImageTest: _onLoadStart");
   },
   _onLoadEnd() {
        console.log("ViroImageTest: _onLoadEnd");
   },

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
      fontSize: 10,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});
 module.exports = ViroImageTest;
