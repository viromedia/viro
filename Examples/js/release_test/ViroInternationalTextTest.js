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
  ViroImage,
  ViroUtils,
  ViroText,
  ViroSkyBox,
} from 'react-viro';

import DeviceInfo from 'react-native-device-info';

var createReactClass = require('create-react-class');
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroInternationalTextTest = createReactClass({

  getInitialState() {
    return {
      fontSize:32,
    }
  },

  _platformUpdate: function(updates) {
    console.log("Platform updated! platform: " + updates.platform + ", headset: " + updates.headset + ", controller: " + updates.controller);
  },

  render: function() {
    return (
      <ViroScene reticleEnabled={true} onPlatformUpdate={this._platformUpdate} >
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
        <ViroSkyBox color="#AA1460" />
        <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

        <ViroNode position={[0, 0, -6]}>
           <ViroText fontSize={this.state.fontSize}
                     style={Platform.OS == "ios" ? styles.chineseFontiOS : styles.chineseFontAndroid}
                     position={[-5, -2, 0]} width={5} height={5}
                     text="人人生而自由,在尊严和权利上一律平等。他们赋 有理性和良心,并应以兄弟关系的精神互相对待。" />

           <ViroText fontSize={this.state.fontSize}
                     style={Platform.OS == "ios" ? styles.koreanFontiOS : styles.koreanFontAndroid}
                     position={[5, -2, 0]} width={5} height={5}
                     text="ㄱ, ㄴ, ㄷ, ㄹ, ㅁ, ㅂ, ㅅ, ㅇ, ㅈ, ㄲ, ㄸ, ㅃ, ㅆ, ㅉ, ㅊ, ㅋ, ㅌ, ㅍ, ㅎ" />

           <ViroText fontSize={this.state.fontSize}
                     style={Platform.OS == "ios" ? styles.japaneseFontiOS : styles.japaneseFontAndroid}
                     position={[-5, 2, 0]} width={5} height={5}
                     text="あ い う え お か き く け こ さ し す せ そ が ぎ ぐ げ ご ぱ ぴ ぷ ぺ ぽ" />

           <ViroText fontSize={this.state.fontSize}
                     style={Platform.OS == "ios" ? styles.hindiFontiOS : styles.hindiFontAndroid} position={[5, 2, 0]}
                     width={5} height={5}
                     text="अ आ इ ई उ ऊ ए ऐ ओ औ अं अः क ख ग घ ङ च छ ज झ ञ ट ठ ड ढ ण त थ द ध न प फ" />

           <ViroText fontSize={this.state.fontSize}
                     style={Platform.OS == "ios" ? styles.arabicFontiOS : styles.arabicFontAndroid} position={[0, 0, 0]}
                     width={5} height={5}
                     text="ان عدة الشهور عند الله اثنا عشر شهرا في كتاب الله يوم خلق السماوات والارض" />
       </ViroNode>

      </ViroScene>
    );
  },

  _showNext() {
     this.props.sceneNavigator.replace({scene:require('./Viro3DObjectTest')});
  },
});


var hindiFontAndroid = DeviceInfo.getBrand() == 'samsung' ? 'SECDevanagari' : 'NotoSansDevanagari'
var arabicFontAndroid = DeviceInfo.getBrand() == 'samsung' ? 'SECNaskhArabic' : 'NotoNaskhArabic'

var styles = StyleSheet.create({
    chineseFontiOS: {
         fontFamily: 'PingFang HK',
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },
    chineseFontAndroid: {
         fontFamily: 'NotoSansCJK',
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },

    koreanFontiOS: {
         fontFamily: 'Apple SD Gothic Neo',
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },
    koreanFontAndroid: {
         fontFamily: 'NotoSansCJK',
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },

    japaneseFontiOS: {
         fontFamily: 'Heiti TC',
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },
    japaneseFontAndroid: {
         fontFamily: 'NotoSansCJK',
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },

    hindiFontiOS: {
         fontFamily: 'Devanagari Sangam MN',
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },
    hindiFontAndroid: {
         fontFamily: hindiFontAndroid,
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },

    arabicFontiOS: {
         fontFamily: 'Geeza Pro',
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },
    arabicFontAndroid: {
         fontFamily: arabicFontAndroid,
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },
});

ViroMaterials.createMaterials({

});


module.exports = ViroInternationalTextTest;
