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

 // **This test has not been done.  This is placeholder for scene and navigation arrows**

 var ReleaseMenu = require("./ReleaseMenu.js");

 var ViroSphereTest = createReactClass({

   getInitialState() {
     return {
        widthSegCount:5,
        heightSegCount:5,
        fontSize:18,
        radius:1,
        facesOutward:true
     };
   },

   render: function() {
     return (
      <ViroScene>
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

         <ViroNode position={[0,0,-4]}>

         <ViroImage source={require('./res/poi_dot.png')} position={[0, -3, -2]} transformBehaviors={["billboard"]} onClick={this._showNext} />

           <ViroOmniLight
                             position={[0, 1, 2]}
                             color={"#ffffff"}
                             attenuationStartDistance={30}
                             attenuationEndDistance={40} />


                <ViroSphere
                      position={[0, 3, 0]}
                      scale={[0.5, 0.5, 0.5]}
                      widthSegmentCount={this.state.widthSegCount}
                      heightSegmentCount={this.state.heightSegCount}
                      radius={this.state.radius}
                      facesOutward={this.state.facesOutward}
                      materials={["redColor"]}
                      />
            <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[-2, 0, 0]} width={4} height ={2}
                text={"Toggle width Seg count: " + this.state.widthSegCount} onClick={this._toggleWidSegCount}/>
            <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[0, 0, 0]} width={4} height ={2}
                text={"Toggle height Seg count: " + this.state.heightSegCount} onClick={this._toggleHeightSegCount}/>
            <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[2, 0, 0]} width={4} height ={2}
                text={"Toggle Radius: " + this.state.radius} onClick={this._toggleRadius}/>
            <ViroText style={styles.baseTextTwo} fontSize={this.state.fontSize}  position={[0, -1, 0]} width={4} height ={2}
                text={"Toggle faces outward: " + this.state.facesOutward} onClick={this._toggleFacesOutwards}/>
       </ViroNode>


      </ViroScene>

     );
   },

   _showNext() {
     this.props.sceneNavigator.replace({scene:require('./ViroButtonTest')});
   },

   _toggleWidSegCount() {
     var newWidthSegCount = this.state.widthSegCount + 1;
     if (newWidthSegCount > 35){
        newWidthSegCount = 2;
     }

     this.setState({
        widthSegCount:newWidthSegCount,
     });
   },

   _toggleHeightSegCount() {
     var newHeightSegCount = this.state.heightSegCount + 1;
     if (newHeightSegCount > 35){
        newHeightSegCount = 2;
     }

     this.setState({
        heightSegCount:newHeightSegCount,
     });
   },
   _toggleRadius() {
     var newRadius = this.state.radius + 5;
     if (newRadius > 20){
        newRadius = 0.5;
     }

     this.setState({
        radius:newRadius,
     });
   },
   _toggleFacesOutwards() {
     this.setState({
             facesOutward:!this.state.facesOutward,
     });
   },
 });


ViroMaterials.createMaterials({
  redColor: {
  fresnelExponent: .5,
   shininess: 2.0,
   lightingModel: "Lambert",
    diffuseColor: "#ff0000"
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

 module.exports = ViroSphereTest;
