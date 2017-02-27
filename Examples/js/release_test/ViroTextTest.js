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
  ViroText,
  ViroAnimations,
  ViroAnimatedComponent,
  ViroSurface,
  ViroSkyBox,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;
var longText ="I'm testing this to see if it works, this is the best most tremendous text engine. You won't be disappointed, believe me!";
var ViroTextTest = React.createClass({

  getInitialState() {
    return {
      fontSize:20
    }
  },

  _platformUpdate: function(updates) {
    console.log("Platform updated! platform: " + updates.vrPlatform + ", headset: " + updates.headset + ", controller: " + updates.controller);
  },
  _toggleFontSize(){
    var newSize;
    if (this.state.fontSize > 60){
        newSize = 15;
    } else {
        newSize = this.state.fontSize + 5;
    }

    this.setState({
          fontSize: newSize,
        });
  },

  render: function() {
    return (
      <ViroScene reticleEnabled={true} onPlatformUpdate={this._platformUpdate} >
        <ViroSkyBox color="#ff69b4" />

        <ViroNode  position={[0,0,-6]}>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColor" position={[-4,2,0]} onClick={this._toggleFontSize}>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText}  text={ "Toggle Font Size: " + this.state.fontSize} />
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColor" position={[-4,1,0]}>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText}  text="Toogle Font Color!" />
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColor" position={[-4,0,0]}>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText}  text="Toggle something else" />
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColor" position={[-1, 2, 0]}>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText}  text="Vertical Centered Text Top!" textAlignVertical={"top"}/>
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColor" position={[-1, 1, 0]}>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText}  text="Vertical Centered Text Center!" textAlignVertical={"center"}/>
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColor" position={[-1, 0, 0]}>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText}  text="Vertical Centered Text Bottom!" textAlignVertical={"bottom"}/>
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColor" position={[2, 2, 0]}>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText}  text="Horizontal Centered Text Left!"  textAlign={"left"}/>
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColor" position={[2, 1, 0]}>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText}  text="Horizontal Centered Text Center!" textAlign={"center"}/>
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColor" position={[2, 0, 0]}>
          <ViroText fontSize={this.state.fontSize} style={styles.centeredText}  text="Horizontal Centered Text Right!" textAlign={"right"}/>
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical}  width={2} height={1}  materials="redColor"position={[-4,-1, 0]}>
            <ViroText fontSize={this.state.fontSize} style={styles.baseTextTwo}  text={"TEXT IN FLEX BOX TEST: This text is supposed to be larger than the flex box? " + longText}/>
        </ViroFlexView>

       <ViroText fontSize={this.state.fontSize} color="#000000" position={[-1,-2, 0]} width={2} height ={2} maxLines={3}
       text={"MAX LINE TEST See a max of three lines for this text view only three lines if not it is a BUG do you see the end of this sentence go beyond three lines? " + longText} />

       <ViroText fontSize={this.state.fontSize}  position={[-4,-4, 0]} width={2} height ={2}
       text={"Text should be clipped to bounds ." + longText} textClipMode={"cliptobounds"}/>

       <ViroText fontSize={this.state.fontSize} position={[2, -2, 0]} width={2} height ={2} fontFamily={'Arial'}
        text={"TextLineBreak for a word-wrap Test!" + longText} textLineBreakMode='wordwrap'/>

       <ViroText fontSize={this.state.fontSize} position={[-1,-3, 0]} width={2} height ={2} fontFamily={'Arial'}
        text={"TextLineBreak for a charWrap test!" + longText} textLineBreakMode='charwrap'/>

       <ViroText fontSize={this.state.fontSize} position={[2,-4, 0]} width={2} height ={2} fontFamily={'Arial'}
        text={"TextLineBreak for a justify test!" + longText} textLineBreakMode='justify'/>
       </ViroNode>

      </ViroScene>
    );
  },
});

ViroMaterials.createMaterials({
  redColor: {
    diffuseColor: "#ff0000"
  },
  cardpetite: {
    shininess : 1.0,
    lightingModel: "Constant",
    diffuseTexture: require('../res/card_petite_ansu.png'),
  },
  box_material: {
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseColor: "#ffffff"
  }
});
const styles = StyleSheet.create({
  containerVertical: {
    flexDirection: 'column',
  },
    containerHorizontal: {
      flexDirection: 'row',
    },

   centeredText: {
        fontFamily: 'Arial',
        color: '#ffffff',
        flex: 1,
   },

   horizontalCenteredText: {
        fontFamily: 'Arial',
        color: '#ffffff',
        flex: 1,
        textAlignVertical: 'top',
        textAlign: 'center',
   },

      verticallyCenteredText: {
           fontFamily: 'Arial',
           color: '#ffffff',
           flex: 1,
           textAlignVertical: 'center',
           textAlign: 'left',
      },

  containerHorizontal: {
    flexDirection: 'row',
    padding: .1,
  },

  containerVertical: {
    flexDirection: 'column',
  },
  containerVerticalTwo: {
    flexDirection: 'column',
    justifyContent: 'flex-start'
  },
  containerInner: {
    flexDirection: 'row',
    flex: 1,
    padding: .1,
    backgroundColor: "#0000ff",
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
  baseTextThree: {
     fontFamily: 'Courier',
     fontSize: 20,
     color: '#ffffff',
     flex: 1,
     textAlign: 'center',
 },
});


module.exports = ViroTextTest;
