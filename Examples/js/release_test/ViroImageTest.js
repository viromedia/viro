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
   ViroSurface,
   ViroSkyBox,
   ViroSphere,
   Viro3DObject,
 } from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;


var Uri360Image = {uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/360_horseshoe.jpg"};
var Local360Image = require("./res/360_park.jpg");

var placeholderImage = require("./res/grid_bg.jpg");
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroImageTest = React.createClass({
   getInitialState() {
     return {
        widthAndHeight:1,
        color:"#098765",
        format:"RGBA8",
        showColorBackground:false,
        showUrlImage:false
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
                position={[0, 1, -3]}
                scale={[0.5, 0.5, 0.1]}
                placeholderSource={placeholderImage}
                source={image}
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
      </ViroNode>
        );
   },

   _showNext() {
     this.props.sceneNavigator.pop();
           this.props.sceneNavigator.push({scene:require('./ViroTextTest')});
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
            newState="RGBA4";
        } else if (this.state.format == "RGBA4"){
            newState="RGB565";
        } else if (this.state.format == "RGB565"){
            newState="RGBA8";
        }

        this.setState({
                    format:newState
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
