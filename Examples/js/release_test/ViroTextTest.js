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
  ViroQuad,
  ViroSkyBox,
} from 'react-viro';

var createReactClass = require('create-react-class');
let polarToCartesian = ViroUtils.polarToCartesian;
var longTextTag ="I'm testing this to see if it works, this is the best most tremendous text engine. You won't be disappointed, believe me!";
var longTextTag2 = "Changed text should see lots of #: ########### ########### ############### ########## ###### ########## ########## ###### End of #."

var ReleaseMenu = require("./ReleaseMenu.js");

var ViroTextTest = createReactClass({

  getInitialState() {
    return {
      color:1,
      fontSize:20,
      longText:longTextTag
    }
  },

  _platformUpdate: function(updates) {
    console.log("Platform updated! platform: " + updates.platform + ", headset: " + updates.headset + ", controller: " + updates.controller);
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

  _toggleColor(){
    var newColor = this.state.color == 1 ? 0 : 1;
    this.setState({
              color: newColor,
            });
  },

  _toggleTextValue(){
    var newText = this.state.longText == longTextTag ? longTextTag2 : longTextTag;
    this.setState({
              longText: newText,
            });

  },

  render: function() {
    var newStyle;
     if (this.state.color == 1) { newStyle = styles.centeredText} else {
      newStyle = styles.centeredTextBlue;
      }
    return (
      <ViroScene reticleEnabled={true} onPlatformUpdate={this._platformUpdate} >
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

        <ViroSkyBox color="#ff69b4" />

        <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

        <ViroNode  position={[0,0,-6]}>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColorText" position={[-4,2,0]} onClick={this._toggleFontSize}>
          <ViroText fontSize={this.state.fontSize} style={newStyle}  text={ "Toggle Font Size: " + this.state.fontSize} />
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColorText" position={[-4,1,0]} onClick={this._toggleColor}>
          <ViroText fontSize={this.state.fontSize} style={newStyle}  text="Toogle Font Color!" />
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColorText" position={[-4,0,0]} onClick={this._toggleTextValue}>
          <ViroText fontSize={this.state.fontSize} style={newStyle}  text="Toggle text value" />
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColorText" position={[-1, 2, 0]}>
          <ViroText fontSize={this.state.fontSize} style={newStyle}  text="Vertical Céntéréd Text Top!" textAlignVertical={"top"}/>
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColorText" position={[-1, 1, 0]}>
          <ViroText fontSize={this.state.fontSize} style={newStyle}  text="Vertical Céntéréd Text Center!" textAlignVertical={"center"}/>
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColorText" position={[-1, 0, 0]}>
          <ViroText fontSize={this.state.fontSize} style={newStyle}  text="Vertical Céntéréd Text Bottom!" textAlignVertical={"bottom"}/>
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColorText" position={[2, 2, 0]}>
          <ViroText fontSize={this.state.fontSize} style={newStyle} width={2} height={0.8} text="Horizontal Céntéréd Text Left!"  textAlign={"left"}/>
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColorText" position={[2, 1, 0]}>
          <ViroText fontSize={this.state.fontSize} style={newStyle} width={2} height={0.8} text="Horizontal Céntéréd Text Center!" textAlign={"center"}/>
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical} width={2} height={0.8} materials="redColorText" position={[2, 0, 0]}>
          <ViroText fontSize={this.state.fontSize} style={newStyle} width={2} height={0.8} text="Horizontal Céntéréd Text Right!" textAlign={"right"}/>
        </ViroFlexView>

        <ViroFlexView style={styles.containerVertical}  width={2} height={1}  materials="redColorText" position={[-4,-1, 0]}>
            <ViroText fontSize={this.state.fontSize} style={newStyle}  text={"TEXT IN FLEX BOX TEST: This text is supposed to be larger than the flex box? " + this.state.longText}/>
        </ViroFlexView>

       <ViroText fontSize={this.state.fontSize}  style={newStyle} position={[-1,-2, 0]} width={2} height ={2} maxLines={3}
       text={"MAX LINE TEST See a max of three lines for this text view only three lines if not it is a BUG do you see the end of this sentence go beyond three lines? " + this.state.longText} />

       <ViroText fontSize={this.state.fontSize}  style={newStyle}  position={[-4,-4, 0]} width={2} height ={2}
       text={"Text should be clipped to bounds ." + this.state.longText + this.state.longText} textClipMode={"clipToBounds"}/>

       <ViroText fontSize={this.state.fontSize} style={newStyle}  position={[2, -2, 0]} width={2} height ={2} fontFamily={'Arial'}
        text={"TextLineBreak for a word-wrap Test!" + this.state.longText} textLineBreakMode='WordWrap'/>

       <ViroText fontSize={this.state.fontSize} style={newStyle} position={[-1,-3, 0]} width={2} height ={2} fontFamily={'Arial'}
        text={"TextLineBreak for a charWrap test!" + this.state.longText} textLineBreakMode='CharWrap'/>

       <ViroText fontSize={this.state.fontSize} style={newStyle} position={[2,-4, 0]} width={2} height ={2} fontFamily={'Arial'}
        text={"TextLineBreak for a justify test!" + this.state.longText} textLineBreakMode='Justify'/>
       </ViroNode>

      </ViroScene>
    );
  },

  _showNext() {
     this.props.sceneNavigator.replace({scene:require('./Viro3DObjectTest')});
  },
});


var styles = StyleSheet.create({
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

      centeredTextBlue: {
           fontFamily: 'Arial',
           color: '#0000ff',
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

ViroMaterials.createMaterials({
  redColorText: {
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


module.exports = ViroTextTest;
