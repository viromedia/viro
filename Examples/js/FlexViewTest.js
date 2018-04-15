/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
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
  ViroAnimations,
  ViroAnimatedComponent,
  ViroQuad,
  ViroSkyBox,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;

var createReactClass = require('create-react-class');

var FlexViewTest = createReactClass({
  getInitialState: function() {
    return {
      addLight: false,
      flexState : true,
    };
  },
  render: function() {
    return (
      <ViroScene reticleEnabled={true} onTap={this._addRemoveLight}>
      	{this._addGetLight()}
        <ViroBox materials="box_material" position={[-2, 0, -3]} scale={[2,2,2]}/>

        <ViroSkyBox color="#ff69b4" />

        <ViroVideo source={{uri: "https://s3.amazonaws.com/viro.video/Climber2Top.mp4"}} position={[0,-1, -1]} />

        <ViroFlexView style={styles.containerVertical} position={polarToCartesian([2, 0, 30])} width={3} height={2}
          onClick={()=>{this.setState({flexState : !this.state.flexState})}}>
          <ViroFlexView style={styles.containerInner} >
            <ViroQuad style={this.state.flexState ? {flex:1} : {flex : 2}} materials={["redColor"]} />
            <ViroImage style={{flex:1}} source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}}
              onLoadStart={this._onLoadStart("Image")} onLoadEnd={this._onLoadEnd("Image")} />
          </ViroFlexView>
          <ViroFlexView style={styles.containerInner} >
            <ViroImage style={{flex:1}} source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}}
              onLoadStart={this._onLoadStart("Image")} onLoadEnd={this._onLoadEnd("Image")} />
          </ViroFlexView>
        </ViroFlexView>

        <ViroFlexView style={styles.containerHorizontal} position={[2, 0, 0]} width={3} height={2} transformBehaviors={"billboard"} >
          <ViroImage style={{flex:1}} source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}} />
          <ViroImage style={{flex:1}} source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}} />
        </ViroFlexView>

        <ViroFlexView style={styles.containerHorizontal} position={[-2, 0, 0]} width={3} height={2} transformBehaviors={"billboard"} >
          <ViroFlexView style={{flex:1, backgroundColor: "#ff0000",}} />
          <ViroFlexView style={{flex:1}} materials={"sunTexture"} />
        </ViroFlexView>

        <ViroAnimatedComponent animation="rotateAndMovePicture" run={true} loop={false} onFinish={this._onAnimationFinished}>
          <ViroImage source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}} position={[1, -1, -4]} scale={[.5, .5, .5]} />
        </ViroAnimatedComponent>

      </ViroScene>
    );
  },
  /*
  <Viro360Image source={{uri: "http://cdn3-www.dogtime.com/assets/uploads/gallery/pembroke-welsh-corgi-dog-breed-pictures/prance-8.jpg"}} rotation={[-30,90,0]} />
  */
  _onLoadStart(component) {
    return () => {
      console.log("flexViewTest " + component + " load start");
    }
  },
  _onLoadEnd(component) {
    return () => {
      console.log("flexViewTest " + component + " load end");
    }
  },
  _onFinish(component) {
    return () => {
      console.log("flexViewTest " + component + " finished");
    }
  },
  _onAnimationFinished(){
    console.log("AnimationTest on Animation Finished!");
  },
  _addRemoveLight(component) {
    console.log("remove light called");
    this.setState({
      addLight: !this.state.addLight,
    });
  },
  _addGetLight(component){
    if (this.state.addLight) {
      return (<ViroAmbientLight color="#ffff00"/>);
    } else {
     return;
    }
  }
});

const styles = StyleSheet.create({
  containerVertical: {
    flexDirection: 'column',
  },
  containerInner: {
    flexDirection: 'row',
    flex: 1,
    padding: .1,
    backgroundColor: "#0000ff",
  },
  containerHorizontal: {
    flexDirection: 'row',
    padding: .1,
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    color: '#333333',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

ViroMaterials.createMaterials({
  sunTexture: {
    diffuseTexture: require("./res/sun_2302.jpg"),
  },
  redColor: {
    diffuseColor: "#ff0000"
  },
  cardpetite: {
    shininess : 1.0,
    lightingModel: "Constant",
    diffuseTexture: require('./res/card_petite_ansu.png'),
  },
  box_material: {
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseColor: "#ffffff"
  }
});

ViroAnimations.registerAnimations({
    moveRight:{properties:{positionX:"+=3"}, duration: 3000},
    moveLeft:{properties:{positionX:"-=3"}, duration: 3000},
    rotate:{properties:{rotateZ:"+=90"}, duration:3000},
    rotateY:{properties:{rotateY:"+=180"}, duration:1500},
    rotateAndMovePicture:[
        ["moveLeft", "moveRight"],
        ["moveRight", "moveLeft"],
        ["rotateY", "rotateY"],
        ["rotate"],

    ],
});

module.exports = FlexViewTest;
