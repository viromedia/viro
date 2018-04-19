'use strict';

import React, { Component } from 'react';

import {StyleSheet} from 'react-native';

import {
  ViroARScene,
  ViroMaterials,
  ViroNode,
  ViroAnimations,
  Viro3DObject,
  ViroLightingEnvironment,
  ViroARImageMarker,
  ViroARTrackingTargets,
  ViroSphere,
  ViroSpotLight,
  ViroQuad,
} from 'react-viro';

var createReactClass = require('create-react-class');


var ARCarDemo = createReactClass({
  getInitialState() {
    return {
      texture: "white",
      playAnim: false,
      animateCar: false,
      tapWhite: false,
      tapBlue: false,
      tapGrey: false,
      tapRed: false,
      tapYellow: false,
    }
  },

  render: function() {
    return (
      <ViroARScene>

        <ViroLightingEnvironment source={require('./res/tesla/garage_1k.hdr')}/>

        <ViroARImageMarker target={"logo"} onAnchorFound={this._onAnchorFound} pauseUpdates={this.state.pauseUpdates}>
          <ViroNode scale={[0, 0, 0]} transformBehaviors={["billboardY"]} animation={{name:this.state.animName, run:this.state.playAnim,}}>
            <ViroSphere materials={["white_sphere"]}
              heightSegmentCount={20} widthSegmentCount={20} radius={.03}
              position={[-.2, .25, 0]}
              onClick={this._selectWhite}
              animation={{name:"tapAnimation", run:this.state.tapWhite, onFinish:this._animateFinished}}
              shadowCastingBitMask={0} />

            <ViroSphere materials={["blue_sphere"]}
              heightSegmentCount={20} widthSegmentCount={20} radius={.03}
              position={[-.1, .25, 0]}
              onClick={this._selectBlue}
              animation={{name:"tapAnimation", run:this.state.tapBlue, onFinish:this._animateFinished}}
              shadowCastingBitMask={0} />

            <ViroSphere materials={["grey_sphere"]}
              heightSegmentCount={20} widthSegmentCount={20} radius={.03}
              position={[0, .25, 0]}
              onClick={this._selectGrey}
              animation={{name:"tapAnimation", run:this.state.tapGrey, onFinish:this._animateFinished}}
              shadowCastingBitMask={0} />

            <ViroSphere materials={["red_sphere"]}
              heightSegmentCount={20} widthSegmentCount={20} radius={.03}
              position={[.1, .25, 0]}
              onClick={this._selectRed}
              animation={{name:"tapAnimation", run:this.state.tapRed, onFinish:this._animateFinished}}
              shadowCastingBitMask={0} />

            <ViroSphere materials={["yellow_sphere"]}
              heightSegmentCount={20} widthSegmentCount={20} radius={.03}
              position={[.2, .25, 0]}
              onClick={this._selectYellow}
              animation={{name:"tapAnimation", run:this.state.tapYellow, onFinish:this._animateFinished}}
              shadowCastingBitMask={0}/>
          </ViroNode>

          <Viro3DObject
            scale={[0, 0, 0]}
            source={require('./res/tesla/object_car.obj')}
            resources={[require('./res/tesla/object_car.mtl'),
                        ]}
            type="OBJ"
            materials={this.state.texture}
            onClick={this._toggleButtons}
            animation={{name:"scaleCar", run:this.state.animateCar,}} />

          <ViroSpotLight
            innerAngle={5}
            outerAngle={25}
            direction={[0,-1,0]}
            position={[0, 5, 1]}
            color="#ffffff"
            castsShadow={true}
            shadowMapSize={2048}
            shadowNearZ={2}
            shadowFarZ={7}
            shadowOpacity={.7} />

          <ViroQuad
            rotation={[-90, 0, 0]}
            position={[0, -0.001, 0]}
            width={2.5} height={2.5}
            arShadowReceiver={true} />

        </ViroARImageMarker>
      </ViroARScene>
    );
  },
  _onAnchorFound() {
    this.setState({
      animateCar: true,
    })
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
      tapWhite: true
    })
  },
  _selectBlue(){
    this.setState({
      texture : "blue",
      tapBlue: true
    })
  },
  _selectGrey(){
    this.setState({
      texture : "grey",
      tapGrey: true
    })
  },
  _selectRed(){
    this.setState({
      texture : "red",
      tapRed: true
    })
  },
  _selectYellow(){
    this.setState({
      texture : "yellow",
      tapYellow: true
    })
  },
  _animateFinished(){
    this.setState({
      tapWhite: false,
      tapBlue: false,
      tapGrey: false,
      tapRed: false,
      tapYellow: false,
    })
  },
});

ViroMaterials.createMaterials({
  white: {
    lightingModel: "PBR",
    diffuseTexture: require('./res/tesla/object_car_main_Base_Color.png'),
    metalnessTexture: require('./res/tesla/object_car_main_Metallic.png'),
    roughnessTexture: require('./res/tesla/object_car_main_Roughness.png'),
  },
  blue: {
    lightingModel: "PBR",
    diffuseTexture: require('./res/tesla/object_car_main_Base_Color_blue.png'),
    metalnessTexture: require('./res/tesla/object_car_main_Metallic.png'),
    roughnessTexture: require('./res/tesla/object_car_main_Roughness.png'),
  },
  grey: {
    lightingModel: "PBR",
    diffuseTexture: require('./res/tesla/object_car_main_Base_Color_grey.png'),
    metalnessTexture: require('./res/tesla/object_car_main_Metallic.png'),
    roughnessTexture: require('./res/tesla/object_car_main_Roughness.png'),
  },
  red: {
    lightingModel: "PBR",
    diffuseTexture: require('./res/tesla/object_car_main_Base_Color_red.png'),
    metalnessTexture: require('./res/tesla/object_car_main_Metallic.png'),
    roughnessTexture: require('./res/tesla/object_car_main_Roughness.png'),
  },
  yellow: {
    lightingModel: "PBR",
    diffuseTexture: require('./res/tesla/object_car_main_Base_Color_yellow.png'),
    metalnessTexture: require('./res/tesla/object_car_main_Metallic.png'),
    roughnessTexture: require('./res/tesla/object_car_main_Roughness.png'),
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

ViroARTrackingTargets.createTargets({
  logo : {
    source : require('./res/logo.png'),
    orientation : "Up",
    physicalWidth : 0.165 // real world width in meters
  }
});

ViroAnimations.registerAnimations({
    scaleUp:{properties:{scaleX:1, scaleY:1, scaleZ:1,},
                  duration: 500, easing: "bounce"},
    scaleDown:{properties:{scaleX:0, scaleY:0, scaleZ:0,},
                  duration: 200,},
    scaleCar:{properties:{scaleX:.09, scaleY:.09, scaleZ:.09,},
                  duration: 500, easing: "bounce"},
    scaleSphereUp:{properties:{scaleX:.8, scaleY:.8, scaleZ:.8,},
                  duration: 50, easing: "easeineaseout"},
    scaleSphereDown:{properties:{scaleX:1, scaleY:1, scaleZ:1,},
                  duration: 50, easing: "easeineaseout"},
    tapAnimation:[["scaleSphereUp", "scaleSphereDown"],]
});

module.exports = ARCarDemo;
