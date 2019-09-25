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
  View,
} from 'react-native';

import {
  ViroSceneNavigator,
  ViroScene,
  ViroARScene,
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
  ViroPortal,
  ViroPortalFrame,
  ViroSphere,
  Viro3DObject,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;

var createReactClass = require('create-react-class');

var lightMask1 = 2;              // 0010
var lightMask2 = 4;              // 0100
var lightMask3 = 8;              // 1000
var allLights =  15;             // 1111
var enviromentLight = 16;        // 10000

var ARShadowTest = createReactClass({
  getInitialState() {
    return {
      boxMask:lightMask1,
      shadowPlaneMask:lightMask1,
      castshadowLights:true,
      shadowClippingPlaneStart:0.1,
      shadowOpacity: 0.9,
      shadowMapSize:1024,
      shadowOrthographicSize:5,
      shadowOrthographicPositionX:0,
      shadowBias:0.005
    };
  },

  toggleProperty(num){

      return () => {
          let that = this;
          if (num == 1){
            this.setState({
             castshadowLights:!this.state.castshadowLights
            });

          } else if (num == 2){
            var newMask = this.getToggledMask(this.state.boxMask);
            this.setState({
             boxMask:newMask
            });

          } else if (num == 3){
            var newMask = this.getToggledMask(this.state.shadowPlaneMask);
            this.setState({
             shadowPlaneMask:newMask
            });

          } else if (num == 4){
            let shadowClippingPlaneStartCurrent = this.state.shadowClippingPlaneStart;
            shadowClippingPlaneStartCurrent ++;
            if (shadowClippingPlaneStartCurrent >= 9){
              shadowClippingPlaneStartCurrent = 0.1;
            }
            this.setState({
             shadowClippingPlaneStart:shadowClippingPlaneStartCurrent
            });

          } else if (num == 5){
            let shadowOpacityCurrent = this.state.shadowOpacity;
            shadowOpacityCurrent = shadowOpacityCurrent - 0.1;
            if (shadowOpacityCurrent < 0){
              shadowOpacityCurrent = 0.9;
            }
            this.setState({
             shadowOpacity:shadowOpacityCurrent
            });

          } else if (num == 6){
            let shadowMapSizeCurrent = this.state.shadowMapSize;
            shadowMapSizeCurrent = shadowMapSizeCurrent /2;
            if (shadowMapSizeCurrent < 8){
              shadowMapSizeCurrent = 1024;
            }
            this.setState({
             shadowMapSize:shadowMapSizeCurrent
            });

          } else if (num == 7){
            let shadowOrthographicSizeCurrent = this.state.shadowOrthographicSize;
            shadowOrthographicSizeCurrent = shadowOrthographicSizeCurrent + 1;
            if (shadowOrthographicSizeCurrent > 10) {
              shadowOrthographicSizeCurrent = 5;
            }
            this.setState({
             shadowOrthographicSize:shadowOrthographicSizeCurrent
            });

          } else if (num == 8){
            let shadowBiasCurrent = this.state.shadowBias;
            shadowBiasCurrent = shadowBiasCurrent + 0.01;
            if (shadowBiasCurrent > 0.155){
              shadowBiasCurrent = 0.005;
            }
            this.setState({
             shadowBias:shadowBiasCurrent
            });

          } else if (num == 9) {
            let shadowOrthographicPositionCurrent = this.state.shadowOrthographicPositionX;
            shadowOrthographicPositionCurrent = shadowOrthographicPositionCurrent - 1;
            if (shadowOrthographicPositionCurrent < -8) {
              shadowOrthographicPositionCurrent = 0;
            }
            this.setState({
              shadowOrthographicPositionX:shadowOrthographicPositionCurrent
            });
          }

      }
  },

  getToggledMask(maskToggle){
    if (maskToggle == lightMask1){
        return lightMask2;
    } else if (maskToggle == lightMask2){
        return lightMask3;
    } else if (maskToggle == lightMask3){
        return 15;
    } else if (maskToggle == 15){
        return lightMask1;
    }
    return lightMask1;
  },

  render: function() {
    if (this.state.reset){
      return (<ViroScene />);
    }

    return (
      <ViroARScene ref="scene1">

         <ViroNode position={[-3, 5, -10]} >
           <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
             position={[0, 0, 0]} width={6} height ={2} maxLines={2}
             text={"Toggle Cast Shadow " + this.state.castshadowLights }
             onClick={this.toggleProperty(1)}
             />
            <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0}
              position={[0,-1, 0]} width={6} height ={2} maxLines={2}
              text={"Toggle Box Mask " + this.state.boxMask }
              onClick={this.toggleProperty(2)}
              />
            <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0}
                  position={[0, -2, 0]} width={6} height ={2} maxLines={2}
                  text={"Toggle Shadow Plane Mask " + this.state.shadowPlaneMask}
                  onClick={this.toggleProperty(3)}
            />
            <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0}
                  position={[0, -3, 0]} width={6} height ={2} maxLines={2}
                  text={"Toggle Shadow NCP " + this.state.shadowClippingPlaneStart}
                  onClick={this.toggleProperty(4)}
            />
            <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0}
                  position={[0, -4, 0]} width={6} height ={2} maxLines={2}
                  text={"Toggle Shadow Opacity " + this.state.shadowOpacity}
                  onClick={this.toggleProperty(5)}
            />
            <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0}
                  position={[0, -5, 0]} width={6} height ={2} maxLines={2}
                  text={"Toggle Shadow Map Size " + this.state.shadowMapSize}
                  onClick={this.toggleProperty(6)}
            />
            <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0}
                  position={[0, -6, 0]} width={6} height ={2} maxLines={2}
                  text={"Toggle Shadow Ortho Size " + this.state.shadowOrthographicSize}
                  onClick={this.toggleProperty(7)}
            />
            <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0}
                  position={[0, -7, 0]} width={6} height ={2} maxLines={2}
                  text={"Toggle Shadow Bias " + this.state.shadowBias}
                  onClick={this.toggleProperty(8)}
            />
            <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0}
                  position={[0, -8, 0]} width={6} height ={2} maxLines={2}
                  text={"Toggle Shadow Ortho Position X " + this.state.shadowOrthographicPositionX}
                  onClick={this.toggleProperty(9)}
            />
          </ViroNode>

          <ViroNode position={[0, 0, -5]}>

            <ViroSpotLight
              innerAngle={45}
              outerAngle={90}
              attenuationStartDistance={0.1}
              attenuationEndDistance={22}
              castsShadow={this.state.castshadowLights}
              direction={[1, -1, 0]}
              position={[-3, 5, 0]}
              color="#ff0000"
              intensity={900}
              shadowBias={this.state.shadowBias}
              shadowMapSize={this.state.shadowMapSize}
              influenceBitMask={lightMask1}
              shadowNearZ={this.state.shadowClippingPlaneStart}
              shadowFarZ={this.state.shadowClippingPlaneStart + 20}
              shadowOpacity={this.state.shadowOpacity}
            />
            <ViroSpotLight
              innerAngle={45}
              outerAngle={90}
              shadowBias={this.state.shadowBias}
              castsShadow={this.state.castshadowLights}
              attenuationStartDistance={0.1}
              attenuationEndDistance={22}
              direction={[-1, -1, 0]}
              position={[3, 5, 0]}
              color="#00ff00"
              intensity={900}
              shadowMapSize={this.state.shadowMapSize}
              influenceBitMask={lightMask2}
              shadowNearZ={this.state.shadowClippingPlaneStart}
              shadowFarZ={this.state.shadowClippingPlaneStart + 20}
              shadowOpacity={this.state.shadowOpacity}
            />

            <ViroDirectionalLight
              castsShadow={this.state.castshadowLights}
              direction={[0, -1, 0]}
              color="#00a7f4"
              intensity={900}
              shadowBias={this.state.shadowBias}
              shadowMapSize={this.state.shadowMapSize}
              shadowOrthographicSize={this.state.shadowOrthographicSize}
              shadowOrthographicPosition={[this.state.shadowOrthographicPositionX, 5, 0]}
              influenceBitMask={lightMask3}
              shadowNearZ={this.state.shadowClippingPlaneStart}
              shadowFarZ={this.state.shadowClippingPlaneStart + 20}
              shadowOpacity={this.state.shadowOpacity}/>

          <ViroBox
             lightReceivingBitMask={this.state.boxMask}
             shadowCastingBitMask={this.state.boxMask}
             position={[0, 0, 0]}
             scale={[0.5, 0.5, 0.5]}
             materials={["red"]}/>

          <ViroQuad
              lightReceivingBitMask={this.state.shadowPlaneMask}
              arShadowReceiver={true}
              rotation={[-90, 0, 0]}
              position={[0, -2.5, 0]}
              scale={[1, 1, 1]}
              width={15}
              height={10} />

              { /* This brightens the surface a bit and lights up the Box */ }
               <ViroOmniLight
                   intensity={500}
                   influenceBitMask={15}
                   position={[0, -2, 2]}
                   color={"#ffffff"}
                   attenuationStartDistance={50}
                   attenuationEndDistance={50}/>
        </ViroNode>

        {/* Release Menu */}
        <ViroText position={polarToCartesian([6, 30, 0])} text={"Next test"}
          style={styles.instructionText} onClick={this._goToNextTest} transformBehaviors={["billboard"]}/>
        <ViroText position={polarToCartesian([6, 30, -15])} text={"Release Menu"}
          style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
          transformBehaviors={["billboard"]}/>

      </ViroARScene>
    );
  },
  _goToNextTest() {
    this.props.arSceneNavigator.replace("ARSceneAndNavigatorTest", {scene:require("./ARSceneAndNavigatorTest")})
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
    color: '#ffffff',
    flex: 1,
  },
  centeredText: {
     fontFamily: 'Arial',
     color: '#ffffff',
     flex: 1,
  },
});

ViroMaterials.createMaterials({
  white: {
    lightingModel: "Lambert",
    cullMode: "None",
    shininess: 2.0,
    diffuseColor: "#ff0000"
  },
  red: {
    lightingModel: "Blinn",
    cullMode: "None",
    shininess: 2.0,
    diffuseColor: "#ff1111"
  },
  shadowCatcher: {
    writesToDepthBuffer: false,
  },
  green: {
    cullMode: "None",
    shininess: 2.0,
    diffuseColor: "#33cc3399"
  },
 });

module.exports = ARShadowTest;
