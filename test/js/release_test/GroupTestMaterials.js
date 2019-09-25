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
  ViroScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroImage,
  ViroVideo,
  ViroFlexView,
  ViroUtils,
  ViroText,
  ViroQuad,
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

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;
var ReleaseMenu = require("./ReleaseMenu.js");
var LocalButtonImage = require("./res/icon_live.jpg");

var TOGGLE_PLAY ="Toggle Play";
var TOGGLE_PAUSE ="Toggle Pause";

var GroupTestMaterials = createReactClass({
  getInitialState() {
    return {
        materialFlag: 1,
        createMaterials : true,
    };
  },

  render: function() {
    var newMaterial;
    var transformText;
    var heartMat;
    // TODO: this is NOT the right way to unset materials, either
    // set the array to empty array or "remove" the material property
    // setting material to "none" will simply invoke a lookup of the
    // material "none" and cause a red screen/crash usually (that's why
    // "none" has been registered as a material below!). Heart is set up
    // correctly though.
    if (this.state.materialFlag == 0) {
        newMaterial = "none";
        transformText = "none";
        heartMat="none";
    } else if (this.state.materialFlag == 1){
        newMaterial="redColor";
        transformText = "redColor";
        heartMat="heartRed";
    } else if (this.state.materialFlag == 2){
        newMaterial="blueLambert";
        transformText = "blueLambert";
        heartMat="heartBlueLambert";
    } else if (this.state.materialFlag == 3){
        newMaterial="blueBlinn";
        transformText = "blueBlinn";
        heartMat="heartBlueBlinn";
    } else if (this.state.materialFlag == 4){
        newMaterial="whiteCullModeBack";
        transformText = "whiteCullModeBack";
        heartMat="heartCullModeBack";
    } else if (this.state.materialFlag == 5){
        newMaterial="whiteCullModeFront";
        transformText = "whiteCullModeFront";
        heartMat="heartCullModeFront";
    } else {
        newMaterial="sun_texture";
        transformText = "sun_texture";
        heartMat="heartSun";
    }

    // this is how we "properly" unset materials.
    let heartMaterial = {};
    if (heartMat != "none") {
      heartMaterial["materials"] = [heartMat];
    }

    return (
        <ViroScene>
            <ViroAmbientLight color={"#666666"} />
            <ViroDirectionalLight direction={[0, 0, -1]} color={"#888888"} />
            <ViroDirectionalLight direction={[0, 0,  1]} color={"#FF0000"} />

            <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />
            <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

            <ViroNode position={[0.8, 0, -3.5]} >
                <Viro3DObject source={require('../res/heart.obj')}
                              scale={[1.8, 1.8, 1.8]}
                              position={[-3.2, 2.5, -4.5]}
                              type="OBJ"
                              {...heartMaterial}/>

                <ViroBox position={[-1, 1, 0]}
                         scale={[0.4, 0.4, 0.4]}
                         rotation={[0, 45, 0]}
                         materials={[newMaterial,"blueBlinn",newMaterial,"blueBlinn",newMaterial,"blueBlinn"]}
                         height={1}
                         width={1}
                         length={1}/>

                <ViroButton
                    position={[0, 1, 0]}
                    materials={[newMaterial]}
                    scale={[0.08, 0.08, 0.1]}
                    source={LocalButtonImage}
                    hoverSource={LocalButtonImage}
                    clickSource={LocalButtonImage}/>

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
                    source={{uri: "https://upload.wikimedia.org/wikipedia/commons/7/74/Earth_poster_large.jpg"}}/>

                <ViroNode position={[-1, 0, 0]} scale={[0.5, 0.5, 0.1]} rotation={[0,0,0]}>
                  <ViroText materials={[newMaterial]}
                            style={styles.baseTextTwo}
                            text="This is a text in a ViroNode" />
                </ViroNode>

                <ViroSphere position={[0, 0, 0]}
                            scale={[0.3, 0.3, 0.3]}
                            widthSegmentCount={5}
                            heightSegmentCount={5}
                            radius={1}
                            materials={[newMaterial]}/>

                <ViroSpinner materials={[newMaterial]}
                             position={[1, 0, 0]}
                            scale={[0.3, 0.3, 0.1]}/>

                <ViroQuad position={[-2, -1, 0]} scale={[0.5, 0.5, 0.1]}
                             materials={[newMaterial]}
                             width={1}
                             height={1}/>

                <ViroText materials={[newMaterial]}
                          position={[-1, -1, 0]}
                          scale={[0.5 , 0.5, 0.1]}
                          style={styles.baseTextTwo}
                          text="This is a Viro Text"/>
            </ViroNode>

            <ViroText style={styles.baseTextTwo} position={[0.8 , -2, -3.5]} width={2} height ={2} fontFamily={'Arial'}
                  text={"Toggle material: " + transformText} textLineBreakMode='Justify' onClick={this._toggleTransform}/>

            <ViroText style={styles.baseTextTwo} position={[0.8 , -2.5, -3.5]} width={2} height ={2} fontFamily={'Arial'}
              text={(this.state.createMaterials ? "Create" : "Delete") + " New Material"} textLineBreakMode='Justify' onClick={this._createOrDeleteMaterial}/>

        </ViroScene>
    );
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./GroupTestDragEvents')});
  },

  _toggleTransform(){
    var newtransformflag = this.state.materialFlag + 1;
    if (newtransformflag > 6){
      newtransformflag = 0;
    }
    this.setState({
      materialFlag:newtransformflag
    });
  },

  _createOrDeleteMaterial() {
    if (this.state.createMaterials) {
      ViroMaterials.createMaterials({
        testMaterialOne : {
          diffuseTexture : require("./res/iron-rusted4-basecolor.png")
        },
        testMaterialTwo : {
          diffuseTexture : require("./res/iron-rusted4-normal.png")
        }
      })
    } else {
      ViroMaterials.deleteMaterials(["testMaterialOne", "testMaterialTwo"]);
    }
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
    diffuseColor: "#0000ff",
    specularTexture: require('./res/specular.png'),
  },

  whiteCullModeBack: {
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseColor: "#FFFFFF",
    cullMode:'Back'
  },

  whiteCullModeFront: {
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseColor: "#FFFFFF",
    cullMode:'Front'
  },

  heartRed: {
    fresnelExponent: .5,
    shininess: 2.0,
    diffuseColor: "#ff0000",
    diffuseTexture: require('../res/heart_d.jpg'),
  },

  heartBlueLambert: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#0000ff",
    diffuseTexture: require('../res/heart_d.jpg'),
  },

  heartBlueBlinn: {
    diffuseColor: "#0000ff",
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('../res/heart_d.jpg'),
    specularTexture: require('./res/specular.png'),
  },

  heartCullModeBack: {
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseColor: "#FFFFFF",
    cullMode:'Back',
    diffuseTexture: require('../res/heart_d.jpg'),
  },

  heartCullModeFront: {
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseColor: "#FFFFFF",
    cullMode:'Front',
    diffuseTexture: require('../res/heart_d.jpg'),
  },

  heartSun: {
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('../res/sun_2302.jpg'),
  },

  sun_texture: {
    diffuseTexture: require("../res/sun_2302.jpg"),
  },

  none: {
    diffuseTexture: require("../res/sun_2302.jpg"),
  }
});

module.exports = GroupTestMaterials;
