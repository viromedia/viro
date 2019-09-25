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
   ViroButton,
   ViroAnimations,
   ViroQuad,
   ViroSkyBox,
   ViroPortal,
   ViroPortalFrame,
   ViroSphere,
   Viro3DObject,
   ViroSpinner
 } from 'react-viro';

var createReactClass = require('create-react-class');

var UriImage = {uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/360_horseshoe.jpg"};
var LocalImage = require("./res/360_park.jpg");

var Uri360Video = {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf.mp4"};
var Local360Video = require("./res/360Asteroids.mp4");

var LocalButtonImage = require("./res/icon_live.jpg");
var ReleaseMenu = require("./ReleaseMenu.js");


var lightMask1 = 2;              // 0010
var lightMask2 = 4;              // 0100
var lightMask3 = 8;              // 1000
var allLights =  15;             // 1111

var ViroShadowGroupTest = createReactClass({
  getInitialState() {
    return {
      boxMask:lightMask1,
      shadowPlaneMask:lightMask1,
      castshadowLights:true,
      shadowClippingPlaneStart:0.1,
      shadowOpacity: 0.9,
      shadowMapSize:1024,
      shadowOrthographicSize:5,
      shadowBias:0.005,
      runAnimation:false,
      isVisible:true
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
            shadowOrthographicSizeCurrent = shadowOrthographicSizeCurrent +1;
            if (shadowOrthographicSizeCurrent > 50){
              shadowOrthographicSizeCurrent = 10;
            }
            this.setState({
             shadowOrthographicSize:shadowOrthographicSizeCurrent
            });

          } else if (num == 8){
            let shadowBiasCurrent = this.state.shadowBias;
            shadowBiasCurrent = shadowBiasCurrent + 0.01;
            if (shadowBiasCurrent > 0.03){
              shadowBiasCurrent = 0.005;
            }
            this.setState({
             shadowBias:shadowBiasCurrent
            });
          } else if (num == 9){
            this.setState({
             runAnimation:!this.state.runAnimation
            });
          } else if (num == 10){
            this.setState({
             isVisible:!this.state.isVisible
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
              <ViroScene ref="scene1">

                 {/* Left half of the screen, tests for collision with ray shot in scene */}
                 <ViroNode position={[5 , 3, 0]} transformBehaviors={["billboard"]}>
                 <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0} // 0 to avoid influencing the test
                   position={[0,0, 0]} width={6} height ={2} maxLines={2}
                   text={"Toggle Cast Shadow " + this.state.castshadowLights }
                   onClick={this.toggleProperty(1)}
                   />
                  <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0}
                        position={[0,-1, 0]} width={6} height ={2} maxLines={2}
                        text={"Run animation " + this.state.runAnimation}
                        onClick={this.toggleProperty(9)}
                  />
                  <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0}
                        position={[0,-2, 0]} width={6} height ={2} maxLines={2}
                        text={"Toggle control visibility " + this.state.isVisible}
                        onClick={this.toggleProperty(10)}
                  />
                  <ViroText fontSize={35}  style={styles.centeredText} lightReceivingBitMask={0}
                        position={[0,-3, 0]} width={6} height ={2} maxLines={2}
                        text={"Toggle shadowBias " + this.state.shadowBias}
                        onClick={this.toggleProperty(8)}
                  />
                  <ReleaseMenu position={[0 , -4, 0.2]} sceneNavigator={this.props.sceneNavigator}/>

                </ViroNode>
                <ViroNode position={[-3,0,-3]}>
                  <ViroSpotLight
                    innerAngle={5}
                    outerAngle={90}
                    attenuationStartDistance={55}
                    attenuationEndDistance={60}
                    castsShadow={this.state.castshadowLights}
                    direction={[0,-1,-1]}
                    position={[1, 1, 4]}
                    color="#ffffff"
                    intensity={1000}
                    shadowBias={this.state.shadowBias}
                    shadowMapSize={this.state.shadowMapSize}
                    influenceBitMask={allLights}
                    shadowNearZ={this.state.shadowClippingPlaneStart}
                    shadowFarZ={this.state.shadowClippingPlaneStart + 10}
                    shadowOpacity={this.state.shadowOpacity}
                  />

                  <ViroQuad
                     lightReceivingBitMask={allLights}
                      materials={"ground"}
                      rotation={[0, 0, 0]}
                      scale={[1,1,1]}
                      width={6.2}
                      height={6.2} />
                </ViroNode>

                <ViroNode position={[-2 , 0, -2.5]}>




                  <ViroBox
                  lightReceivingBitMask={allLights}
                  shadowCastingBitMask={allLights}
                      position={[-1, 1, 0]}
                      scale={[0.4, 0.4, 0.4]}
                      rotation={[40, 45, 0]}
                      materials={["redColor", "blueColor","redColor", "blueColor","redColor", "blueColor"]}
                      height={1}
                      width={1}
                      length={1}
                      visible={this.state.isVisible}
                      onClick={this._elementClick(0)}
                      animation={{name:"testLoopMove",  loop:true, run:this.state.runAnimation}}  />

                  <ViroButton
                      lightReceivingBitMask={allLights}
                      shadowCastingBitMask={allLights}
                      position={[0, 1, 0]}
                      scale={[0.2, 0.2, 0.1]}
                      source={LocalButtonImage}
                      hoverSource={LocalButtonImage}
                      clickSource={LocalButtonImage}
                      visible={this.state.isVisible}
                      onClick={this._elementClick(1)}
                      animation={{name:"testLoopScale",  loop:true, run:this.state.runAnimation}}
                      />


                  <ViroFlexView
                      lightReceivingBitMask={allLights}
                      shadowCastingBitMask={allLights}
                      position={[1, 1, 0]}
                      scale={[0.2, 0.2, 0.1]}
                      materials={["redColor"]}
                      width={3}
                      height={2}
                      visible={this.state.isVisible}
                      onClick={this._elementClick(2)}
                      animation={{name:"testLoopMove",  loop:true, run:this.state.runAnimation}}
                  />


                  <ViroImage
                      lightReceivingBitMask={allLights}
                      shadowCastingBitMask={allLights}
                      width={1} height={1}
                      format="RGBA8" mipmap={true}
                      position={[-2, 0, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      onClick={this._elementClick(3)}
                      visible={this.state.isVisible}
                      source={{uri: "https://upload.wikimedia.org/wikipedia/commons/7/74/Earth_poster_large.jpg"}}
                      animation={{name:"testLoopRotate",  loop:true, run:this.state.runAnimation}}
                      />


                  <ViroNode
                    position={[-1, 0, 0]}
                    scale={[0.5, 0.5, 0.1]}
                    onClick={this._elementClick(4)}
                    rotation={[0,0,0]}>
                    <ViroText
                        lightReceivingBitMask={allLights}
                        shadowCastingBitMask={allLights}
                        style={styles.baseTextTwo}
                        visible={this.state.isVisible}
                        text="This is a text in a ViroNode"
                        animation={{name:"testLoopMove",  loop:true, run:this.state.runAnimation}}
                      />
                  </ViroNode>


                  <ViroSphere
                      lightReceivingBitMask={allLights}
                      shadowCastingBitMask={allLights}
                      position={[0, 0, 0]}
                      scale={[0.3, 0.3, 0.3]}
                      widthSegmentCount={5}
                      heightSegmentCount={5}
                      radius={1}
                      onClick={this._elementClick(5)}
                      materials={["redColor"]}
                      visible={this.state.isVisible}
                      animation={{name:"testLoopScale",  loop:true, run:this.state.runAnimation}}
                      />

                  <ViroSpinner
                      lightReceivingBitMask={allLights}
                      shadowCastingBitMask={allLights}
                      position={[1, 0, 0]}
                      scale={[0.3, 0.3, 0.1]}
                      onClick={this._elementClick(6)}
                      animation={{name:"testLoopMove",  loop:true, run:this.state.runAnimation}}/>


                  <ViroQuad
                      lightReceivingBitMask={allLights}
                      shadowCastingBitMask={allLights}
                      position={[-2, -1, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      materials={["redColor"]}
                      width={1}
                      onClick={this._elementClick(7)}
                      visible={this.state.isVisible}
                      height={1}
                      animation={{name:"testLoopRotate",  loop:true, run:this.state.runAnimation}}
                  />


                  <ViroText
                      lightReceivingBitMask={allLights}
                      shadowCastingBitMask={allLights}
                      position={[-1, -1, 0]}
                      scale={[0.5 , 0.5, 0.1]}
                      style={styles.baseTextTwo}
                      visible={this.state.isVisible}
                      onClick={this._elementClick(8)}
                      text="This is a Viro Text"
                      animation={{name:"testLoopMove",  loop:true, run:this.state.runAnimation}}
                  />


                  <ViroVideo
                      lightReceivingBitMask={allLights}
                      shadowCastingBitMask={allLights}
                      position={[0 , -1,0]}
                      scale={[0.1, 0.1, 0.1]}
                      height={4} width={4}
                      visible={this.state.isVisible}
                      onClick={this._elementClick(9)}
                      animation={{name:"testLoopScale",  loop:true, run:this.state.runAnimation}}
                      source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}} />


                  <Viro3DObject source={{"uri" : "https://s3-us-west-2.amazonaws.com/viro/heart.obj"}}
                               scale={[1.8, 1.8, 1.8]}
                               position={[-2, 1, .2]}
                               type="OBJ"
                               materials="heart"
                               animation={{name:"testLoopRotate",  loop:true, run:this.state.runAnimation}}
                    />

                </ViroNode>
            </ViroScene>

    );
  },
  _elementClick(item){
    return () => {
        //No-op
    }
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
  blue: {
    lightingModel: "Blinn",

      cullMode: "None",
      shininess: 2.0,
      diffuseColor: "#3399ff99"
    },
    red: {
      lightingModel: "Blinn",

        cullMode: "None",
        shininess: 2.0,
        diffuseColor: "#ff1111"
      },
    ground: {
      lightingModel: "Blinn",

        cullMode: "None",
        shininess: 2.0,
        diffuseColor: "#ffffff"
      },
    shadowCatcher: {
      writesToDepthBuffer: false,
    },
  green: {
        cullMode: "None",
        shininess: 2.0,
        diffuseColor: "#33cc3399"

      },
      sunTexture: {
        diffuseTexture: require("../res/sun_2302.jpg"),
      },
      redColor: {
        diffuseColor: "#ff0000"
      },
      blueColor: {
        diffuseColor: "#0000ff"
      },
      heart: {
        lightingModel: "Constant",
        diffuseTexture: { "uri" : "https://s3-us-west-2.amazonaws.com/viro/heart_d.jpg" },
      },
  grey: {
    shininess : 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('./res/grey.jpg'),
  },
 });


 ViroAnimations.registerAnimations({
     testLoopRotate:{properties:{rotateZ:"+=45"}, duration:2000, easing:"EaseInEaseOut"},

     moveRight:{properties:{positionX:"+=0.3"}, duration: 2000, easing:"Linear"},
     moveLeft:{properties:{positionX:"-=0.3"}, duration: 2000, easing:"Linear"},
     testLoopMove:[
         ["moveRight", "moveLeft"]
     ],

     scaleUp:{properties:{scaleX:"+=0.2"}, duration: 2000, easing:"Bounce"},
     scaleDown:{properties:{scaleX:"-=0.2"}, duration: 2000, easing:"Bounce"},
     testLoopScale:[
         ["scaleUp", "scaleDown"]
     ],
     opacityUp:{properties:{opacity: 1}, duration: 2000, easing:"EaseIn"},
     opacityDown:{properties:{opacity: 0}, duration: 2000, easing:"EaseOut"},
         testLoopOpacity:[
             ["opacityDown", "opacityUp"]
         ],
 });


module.exports = ViroShadowGroupTest;
