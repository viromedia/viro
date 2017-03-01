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
  ViroAnimatedComponent,
  ViroScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroImage,
  ViroVideo,
  ViroFlexView,
  ViroUtils,
  ViroText,
  ViroSurface,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
  ViroButton,
  ViroSpinner,
  ViroOmniLight,
  ViroAnimations,
  ViroDirectionalLight,
  ViroAmbientLight
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;

var UriImage = {uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/360_horseshoe.jpg"};
var LocalImage = require("./res/360_park.jpg");

var Uri360Video = {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf.mp4"};
var Local360Video = require("./res/360Asteroids.mp4");

var LocalButtonImage = require("./res/icon_live.jpg");

var TOGGLE_PLAY ="Toggle Play";
var TOGGLE_PAUSE ="Toggle Pause";


var GroupTestMaterials = React.createClass({
  getInitialState() {
    return {
        materialFlag: 1
    };
  },

  render: function() {
    var newMaterial;
    var transformText;
    if (this.state.materialFlag == 0){
        transformText = "None";
    } else if (this.state.materialFlag == 1){
        newMaterial="redColor";
        transformText = "redColor";
    } else if (this.state.materialFlag == 2){
        newMaterial="blueLambert";
        transformText = "blueLambert";
    } else if (this.state.materialFlag == 3){
        newMaterial="blueBlinn";
        transformText = "blueBlinn";
    } else if (this.state.materialFlag == 4){
        newMaterial="blueCullModeFront";
        transformText = "blueCullModeFront";
    }else {
        newMaterial="sun_texture";
        transformText = "sun_texture";
    }

    return (
                <ViroScene>
                 <ViroAmbientLight
                            color={"#ffffff"}/>

                <ViroNode position={[0.8 , 0, -3.5]} >

                    <Viro3DObject source={require('../res/heart.obj')}
                             scale={[1.8 , 1.8  , 1.8]}
                              position={[-2.5 , -4.3 , -1.15]}
                              materials={["heart",newMaterial]}/>
                  <ViroBox
                      position={[-1 , 1, 0]}
                      scale={[0.4 , 0.4 , 0.4]}
                      rotation={[0,45,0]}
                      materials={[newMaterial,"blueBlinn",newMaterial,"blueBlinn",newMaterial,"blueBlinn"]}
                      height={1}
                      width={1}
                      length={1}/>

                  <ViroButton
                      position={[0, 1, 0]}
                      materials={[newMaterial]}
                      scale={[0.08, 0.08, 0.1]}
                      source={LocalButtonImage}
                      gazeSource={LocalButtonImage}
                      tapSource={LocalButtonImage}
                      />

                  <ViroFlexView
                      position={[1, 1, 0]}
                      scale={[0.2, 0.2, 0.1]}
                      materials={[newMaterial]}
                      width={3}
                      height={2} />
                  <ViroImage
                      width={1} height={1}
                      format="RGBA8" mipmap={true}
                      position={[-2, 0, 0]}
                       materials={[newMaterial]}
                      scale={[0.5, 0.5, 0.1]}
                      source={{uri: "https://upload.wikimedia.org/wikipedia/commons/7/74/Earth_poster_large.jpg"}}
                      />
                  <ViroNode
                    position={[-1, 0, 0]}
                    scale={[0.5, 0.5, 0.1]}
                    rotation={[0,0,0]}>
                    <ViroText
                    materials={[newMaterial]}
                        style={styles.baseTextTwo}
                        text="This is a text in a ViroNode" />
                  </ViroNode>

                  <ViroSphere

                      position={[0, 0, 0]}
                      scale={[0.3, 0.3, 0.3]}
                      widthSegmentCount={5}
                      heightSegmentCount={5}
                      radius={1}
            materials={[newMaterial]}
                      />

                  <ViroSpinner
                  materials={[newMaterial]}
                      position={[1, 0, 0]}
                      scale={[0.3, 0.3, 0.1]}/>

                  <ViroSurface
                      position={[-2, -1, 0]}
                      scale={[0.5, 0.5, 0.1]}
                       materials={[newMaterial]}
                      width={1}
                      height={1}/>

                  <ViroText
                   materials={[newMaterial]}
                      position={[-1, -1, 0]}
                      scale={[0.5 , 0.5, 0.1]}
                      style={styles.baseTextTwo}
                      text="This is a Viro Text"/>
                  <ViroVideo
                   materials={[newMaterial]}
                      position={[0 , -1,0]}
                      scale={[0.1, 0.1, 0.1]}
                      height={4} width={4}
                      source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}} />

                </ViroNode>

                <ViroText style={styles.baseTextTwo} position={[0.8 , -2, -3.5]} width={2} height ={2} fontFamily={'Arial'}
                      text={"Toggle material: " + transformText} textLineBreakMode='justify' onClick={this._toggleTransform}/>

                   <ViroOmniLight
                                     position={[0, 0, 0]}
                                     color={"#ffffff"}
                                     attenuationStartDistance={30}
                                     attenuationEndDistance={40}/>

            </ViroScene>

    );
  },

  _toggleTransform(){
    var newtransformflag = this.state.materialFlag + 1;
    if (newtransformflag > 5){
        newtransformflag = 0;
    }
     this.setState({
            materialFlag:newtransformflag
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

  blueLambert: {
      shininess: 2.0,
      lightingModel: "Lambert",
      diffuseColor: "#0000ff"
  },

  blueBlinn: {
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseColor: "#0000ff"
  },

  blueCullModeFront: {
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseColor: "#0000ff"
  },

  heart: {
      lightingModel: "Constant",
      diffuseTexture: require('../res/heart_d.jpg'),
    },
  sun_texture: {
    diffuseTexture: require("../res/sun_2302.jpg"),
  }
 });


module.exports = GroupTestMaterials;
