/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import React, { Component } from 'react';
import { Platform } from 'react-native';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

import {
  ViroSceneNavigator,
  ViroScene,
  ViroMaterials,
  ViroNode,
  ViroCamera,
  ViroOrbitCamera,
  ViroImage,
  ViroUtils,
  ViroText,
  ViroSkyBox,
} from 'react-viro';

var createReactClass = require('create-react-class');
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroTextSpecialTest = createReactClass({

  getInitialState() {
    return {
      activeCamera:0,
      fontSize:44,
    }
  },

  render: function() {
    return (
      <ViroScene reticleEnabled={true} onPlatformUpdate={this._platformUpdate} onClick={this._toggleCamera}>
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
        <ViroSkyBox color="#AA1460" />
        <ViroImage source={require('./res/poi_dot.png')} position={[-2, -1, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

        <ViroCamera
            position={[0, 0, 1]}
            rotation={[0, 0, 0]}
            active={this.state.activeCamera == 0} />

        <ViroOrbitCamera focalPoint={[0, 0, -4]} active={this.state.activeCamera == 1}/>

        <ViroNode position={[0, 0, -4]}>

          <ViroText fontSize={this.state.fontSize}
                    style={styles.italicFont3D} position={[0, 4, 0]}
                    width={20} height={5}
                    outerStroke={{type:"Outline", width:8, color:'#FF0000'}}
                    text="Italic 3D Text (thick red outline)" />

          <ViroText fontSize={this.state.fontSize}
                    style={styles.boldFont3D} position={[0, 3, 0]}
                    width={20} height={5}
                    outerStroke={{type:"DropShadow", width:2, color:'#444444'}}
                    text="Bold 2D Text (grey drop shadow)" />

          <ViroText fontSize={this.state.fontSize}
                    style={styles.normalFont3D} position={[0, 2, 0]}
                    width={20} height={5}
                    outerStroke={{type:"Outline", width:2, color:'#0000FF'}}
                    text="2D Text (blue outline)" />

          <ViroText fontSize={this.state.fontSize}
                    style={styles.normalFont3D} position={[0, 1, 0]}
                    width={20} height={5} extrusionDepth={8}
                    text="3D Text (white)" />

           <ViroText fontSize={this.state.fontSize}
                     style={styles.boldFont3D} position={[0, 0, 0]}
                     width={20} height={5} extrusionDepth={8}
                     materials={["whiteMaterialText3D", "blueMaterialText3D", "redMaterialText3D"]}
                     text="Bold 3D Text (white, blue, red)" />

           <ViroText fontSize={this.state.fontSize}
                     style={styles.heavyFont3D} position={[0, -1, 0]}
                     width={20} height={5} extrusionDepth={8}
                     materials={["whiteMaterialText3D", "blueMaterialText3D", "redMaterialText3D"]}
                     text="Heavy 3D Text (white, blue red)" />

           <ViroText fontSize={this.state.fontSize}
                     style={styles.italicFont3D} position={[0, -2, 0]}
                     width={20} height={5} extrusionDepth={8}
                     materials={["blueMaterialText3D", "redMaterialText3D", "whiteMaterialText3D"]}
                     text="Italicized 3D Text (blue, red, and black)" />

           <ViroText fontSize={this.state.fontSize}
                     style={Platform.OS == "ios" ? styles.chineseFont3DiOS : styles.chineseFont3DAndroid}
                     position={[0, -3, 0]}
                     width={10} height={5} extrusionDepth={8}
                     materials={["whiteMaterialText3D", "blueMaterialText3D", "redMaterialText3D"]}
                     text="人人生而自由,在尊严和权利上一律平等。他们赋 有理性和" />

          <ViroText fontSize={this.state.fontSize}
                    style={styles.italicFont3D} position={[0, -4, 0]}
                    width={20} height={5} extrusionDepth={8}
                    materials={["blueMaterialText3D", "redMaterialText3D", "whiteMaterialText3D"]}
                    text="Click Background to switch Cameras. " />
       </ViroNode>

      </ViroScene>
    );
  },

  _showNext() {
     this.props.sceneNavigator.replace({scene:require('./Viro3DObjectTest')});
  },

    _toggleCamera(){
        var newCamera = this.state.activeCamera + 1;
        if (newCamera > 1) {
            newCamera = 0;
        }
        this.setState({
           activeCamera:newCamera
        });
    },
});

var styles = StyleSheet.create({
    normalFont3D: {
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },
    boldFont3D: {
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
         fontWeight: 'bold',
    },
    heavyFont3D: {
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
         fontWeight: '900',
    },
    italicFont3D: {
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
         fontStyle: 'italic',
    },
    chineseFont3DiOS: {
         fontFamily: 'PingFang HK',
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },
    chineseFont3DAndroid: {
         fontFamily: 'NotoSansCJK',
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },
    hindiFont3DiOS: {
         fontFamily: 'Devanagari Sangam MN',
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },
    hindiFont3DAndroid: {
         fontFamily: 'NotoSansDevanagari',
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },
});

ViroMaterials.createMaterials({
    whiteMaterialText3D: {
      diffuseColor: '#FFFFFF',
    },
    redMaterialText3D: {
      diffuseColor: '#FF0000',
    },
    blueMaterialText3D: {
      diffuseColor: '#0000FF',
    },
});

module.exports = ViroTextSpecialTest;
