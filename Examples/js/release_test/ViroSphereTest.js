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

 var ViroSpatialSoundTest = require('./ViroSpatialSoundTest');
 var ViroSpinnerTest = require('./ViroSpinnerTest');

 // **This test has not been done.  This is placeholder for scene and navigation arrows**

 var ViroSphereTest = React.createClass({

   getInitialState() {
     return {
        widthSegCount:5,
        heightSegCount:5,
        radius:1,
        facesOutward:true
     };
   },

   render: function() {
     return (
      <ViroScene>
         <ViroNode position={[0,0,-4]}>
                <ViroSphere
                      position={[0, 3, 0]}
                      scale={[0.5, 0.5, 0.5]}
                      widthSegmentCount={this.state.widthSegCount}
                      heightSegmentCount={this.state.heightSegCount}
                      radius={this.state.radius}
                      facesOutward={this.state.facesOutward}
                      materials={["redColor"]}
                      />
            <ViroText fontSize={this.state.fontSize}  position={[-2, 0, 0]} width={4} height ={2}
                text={"Toggle width Seg count: " + this.state.widthSegCount} />
            <ViroText fontSize={this.state.fontSize}  position={[-2, -1.5, 0]} width={4} height ={2}
                text={"Toggle height Seg count: " + this.state.heightSegCount} />
            <ViroText fontSize={this.state.fontSize}  position={[-2, -3, 0]} width={4} height ={2}
                text={"Toggle Radius: " + this.state.radius} />
            <ViroText fontSize={this.state.fontSize}  position={[-2, -4.5, 0]} width={4} height ={2}
                text={"Toggle faces outward: " + this.state.facesOutward} />
       </ViroNode>
      </ViroScene>

     );
   },

   _toggleWidSegCount() {
     var newWidthSegCount = this.state.widthSegCount + 1;
     if (newWidthSegCount > 15){
        newWidthSegCount = 2;
     }

     this.setState({
        widthSegCount:newWidthSegCount,
     });
   },

   _toggleHeightSegCount() {
     var newHeightSegCount = this.state.heightSegCount + 1;
     if (newHeightSegCount > 15){
        newHeightSegCount = 2;
     }

     this.setState({
        heightSegCount:newHeightSegCount,
     });
   },
   _toggleRadius() {
     var newRadius = this.state.radius + 0.5;
     if (newRadius > 5){
        newRadius = 0.5;
     }

     this.setState({
        radius:newRadius,
     });
   },
   _toggleFacesOutwards() {
     this.setState({
             facesOutward:!facesOutward,
     });
   },
 });


 module.exports = ViroSphereTest;
