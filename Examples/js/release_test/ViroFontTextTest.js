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

var createReactClass = require('create-react-class');
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroFontTextTest = createReactClass({

  getInitialState() {
    return {
      fontSize:64,
    }
  },

  render: function() {
    return (
      <ViroScene reticleEnabled={true} onPlatformUpdate={this._platformUpdate} >
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
        <ViroSkyBox color="#AA1460" />
        <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

        <ViroNode position={[0, 0, -6]}>
           <ViroText fontSize={this.state.fontSize}
                     style={styles.normalFont} position={[0, -3, 0]}
                     width={5} height={5}
                     text="Normal Font" />

           <ViroText fontSize={this.state.fontSize}
                     style={styles.boldFont} position={[0, -2, 0]}
                     width={5} height={5}
                     text="Bold Font" />

           <ViroText fontSize={this.state.fontSize}
                     style={styles.thinFont} position={[0, -1, 0]}
                     width={5} height={5}
                     text="Thin Font" />

           <ViroText fontSize={this.state.fontSize}
                     style={styles.heavyFont} position={[0, 0, 0]}
                     width={5} height={5}
                     text="Heavy Font" />

           <ViroText fontSize={this.state.fontSize}
                     style={styles.italicFont} position={[0, 1, 0]}
                     width={5} height={5}
                     text="Italicized Font" />
       </ViroNode>

      </ViroScene>
    );
  },

  _showNext() {
     this.props.sceneNavigator.replace({scene:require('./Viro3DObjectTest')});
  },
});

var styles = StyleSheet.create({
    normalFont: {
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
    },
    thinFont: {
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
         fontWeight: '100',
    },
    boldFont: {
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
         fontWeight: 'bold',
    },
    heavyFont: {
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
         fontWeight: '900',
    },
    italicFont: {
         color: '#FFFFFF',
         flex: 1,
         textAlignVertical: 'center',
         textAlign: 'center',
         fontStyle: 'italic',
    },
});

ViroMaterials.createMaterials({

});


module.exports = ViroFontTextTest;
