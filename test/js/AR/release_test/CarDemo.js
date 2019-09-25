'use strict';

import React, { Component } from 'react';

import {StyleSheet} from 'react-native';

import {
  ViroARScene,
  ViroMaterials,
  ViroNode,
  ViroAmbientLight,
  ViroOmniLight,
  ViroImage,
  ViroText,
  ViroAnimations,
  Viro3DObject,
  ViroUtils,
  ViroLightingEnvironment,
  ViroARPlaneSelector,
  ViroARPlane,
  ViroSphere,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var CarDemo = createReactClass({
  getInitialState() {
    return {
      texture: "white",
      playAnim: false,
      findPlanes : true,
      selectedState : "NOT Selected",
      numberOfPlanes : 2,
    }
  },

  render: function() {
    return (
      <ViroARScene>
      <ViroLightingEnvironment source={require('./res/car/tesla/garage_1k.hdr')}/>

       <ViroOmniLight
            intensity={100}
            position={[3, 3, 1]}
            color={"#FFFFFF"}
            attenuationStartDistance={0}
            attenuationEndDistance={20} />
        <ViroARPlaneSelector>
        <ViroNode rotation={[0,180,0]} position={[0, 0, 0]} scale={[0,0,0]} transformBehavior={["billboardY"]} animation={{name:this.state.animName, run:this.state.playAnim,}}>
          <ViroSphere materials={["white_sphere"]}
              heightSegmentCount={20} widthSegmentCount={20} radius={.03}
              position={polarToCartesian([.3, -30, -15])}
              onClick={this._selectWhite}
              />
          <ViroSphere materials={["blue_sphere"]}
              heightSegmentCount={20} widthSegmentCount={20} radius={.03}
              position={polarToCartesian([.3, -15, -15])}
              onClick={this._selectBlue}
            />
          <ViroSphere materials={["grey_sphere"]}
              heightSegmentCount={20} widthSegmentCount={20} radius={.03}
              position={polarToCartesian([.3, 0, -15])}
              onClick={this._selectGrey}
            />
          <ViroSphere materials={["red_sphere"]}
              heightSegmentCount={20} widthSegmentCount={20} radius={.03}
              position={polarToCartesian([.3, 15, -15])}
              onClick={this._selectRed}
            />
          <ViroSphere materials={["yellow_sphere"]}
              heightSegmentCount={20} widthSegmentCount={20} radius={.03}
              position={polarToCartesian([.3, 30, -15])}
              onClick={this._selectYellow}
            />
        </ViroNode>
        <Viro3DObject
        scale={[.1, .1, .1]}
        source={require('./res/car/tesla/object_car.obj')}
        resources={[require('./res/car/tesla/object_car.mtl'),
                    ]}
        type="OBJ"
        animation={{name:"rotateCar", run:true, loop:true,}}
        materials={this.state.texture}
        onClick={this._toggleButtons}
          />
        </ViroARPlaneSelector>


      </ViroARScene>
    );
  },
  _toggleButtons() {
    this.setState({
      animName: (this.state.animName == "scaleUp" ? "scaleDown" : "scaleUp"),
      playAnim: true
    })
  },
  _selectWhite(){
    this.setState({
      texture : "white",
    })
  },
  _selectBlue(){
    this.setState({
      texture : "blue",
    })
  },
  _selectGrey(){
    this.setState({
      texture : "grey",
    })
  },
  _selectRed(){
    this.setState({
      texture : "red",
    })
  },
  _selectYellow(){
    this.setState({
      texture : "yellow",
    })
  },
});

ViroMaterials.createMaterials({
  white: {
    lightingModel: "PBR",
    diffuseTexture: require('./res/car/tesla/object_car_main_Base_Color.png'),
    metalnessTexture: require('./res/car/tesla/object_car_main_Metallic.png'),
    roughnessTexture: require('./res/car/tesla/object_car_main_Roughness.png'),
  },
  blue: {
    lightingModel: "PBR",
    diffuseTexture: require('./res/car/tesla/object_car_main_Base_Color_blue.png'),
    metalnessTexture: require('./res/car/tesla/object_car_main_Metallic.png'),
    roughnessTexture: require('./res/car/tesla/object_car_main_Roughness.png'),
  },
  grey: {
    lightingModel: "PBR",
    diffuseTexture: require('./res/car/tesla/object_car_main_Base_Color_grey.png'),
    metalnessTexture: require('./res/car/tesla/object_car_main_Metallic.png'),
    roughnessTexture: require('./res/car/tesla/object_car_main_Roughness.png'),
  },
  red: {
    lightingModel: "PBR",
    diffuseTexture: require('./res/car/tesla/object_car_main_Base_Color_red.png'),
    metalnessTexture: require('./res/car/tesla/object_car_main_Metallic.png'),
    roughnessTexture: require('./res/car/tesla/object_car_main_Roughness.png'),
  },
  yellow: {
    lightingModel: "PBR",
    diffuseTexture: require('./res/car/tesla/object_car_main_Base_Color_yellow.png'),
    metalnessTexture: require('./res/car/tesla/object_car_main_Metallic.png'),
    roughnessTexture: require('./res/car/tesla/object_car_main_Roughness.png'),
  },
  white_sphere: {
    lightingModel: "PBR",
    diffuseColor: "rgb(231,231,231)",
  },
  blue_sphere: {
    lightingModel: "PBR",
    diffuseColor: "rgb(19,42,143)",
  },
  grey_sphere: {
    lightingModel: "PBR",
    diffuseColor: "rgb(75,76,79)",
  },
  red_sphere: {
    lightingModel: "PBR",
    diffuseColor: "rgb(168,0,0)",
  },
  yellow_sphere: {
    lightingModel: "PBR",
    diffuseColor: "rgb(200,142,31)",
  },
});

ViroAnimations.registerAnimations({
    scaleUp:{properties:{scaleX:1, scaleY:1, scaleZ:1,},
                  duration: 500, easing: "bounce"},
    scaleDown:{properties:{scaleX:0, scaleY:0, scaleZ:0,},
                  duration: 200,},
    rotateCar:{properties:{rotateY:"+=30"}, duration:5000},
});

var styles = StyleSheet.create({
  elementText: {
    fontFamily: 'HelveticaNeue-Medium',
    fontSize: 30,
    color: '#ffffff',
    textAlign: 'center',
  },
});


module.exports = CarDemo;
