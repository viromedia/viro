/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

import React, { Component } from 'react';
import Config from 'react-native-config'
import {
  AppRegistry,
  Text,
  View,
  StyleSheet,
  PixelRatio,
  TouchableHighlight,
} from 'react-native';

import {
  ViroSceneNavigator,
  ViroARSceneNavigator
} from 'react-viro';

var InitialVRScene = require('./js/release_test/ViroSoundTest');
var InitialARScene = require('./js/AR/release_test/ARSceneAndNavigatorTest');

var UNSET = "UNSET";
var VR_NAVIGATOR_TYPE = "VR";
var AR_NAVIGATOR_TYPE = "AR";

export default class ViroExperienceSelector extends Component {
  constructor() {
    super();

    var sharedProps = {
      apiKey:"7EEDCB99-2C3B-4681-AE17-17BC165BF792",
    }
    console.log("Config.FLAVOR - " + Config.VR_PLATFORM);
    this.state = {
      navigatorType : Config.VR_PLATFORM == "OVR_MOBILE" ? VR_NAVIGATOR_TYPE : UNSET,
      sharedProps : sharedProps,
      vrMode : Config.FLAVOR == "ovr" ? true : UNSET
    }
    this._getSelectionButtons = this._getSelectionButtons.bind(this);
    this._getARNavigator = this._getARNavigator.bind(this);
    this._getVRNavigator = this._getVRNavigator.bind(this);
    this._getButtonPress = this._getButtonPress.bind(this);
  }
  render() {
    if (this.state.navigatorType == UNSET) {
      return this._getSelectionButtons();
    } else if (this.state.navigatorType == VR_NAVIGATOR_TYPE) {
      if (this.state.vrMode == UNSET) {
        return this._getSelectionButtons();
      } else {
        return this._getVRNavigator();
      }
    } else if (this.state.navigatorType == AR_NAVIGATOR_TYPE) {
      return this._getARNavigator();
    }
  }
  _getSelectionButtons() {
    return (
      <View style={localStyles.outer} >
        <View style={localStyles.inner} >

          <Text style={localStyles.titleText}>
            {this.state.navigatorType == UNSET ? "Choose your desired experience:" : "VR Mode or 360?"}
          </Text>

          <TouchableHighlight style={this.state.navigatorType == UNSET ? localStyles.buttons : localStyles.vrModeButtons}
            onPress={this._getButtonPress(1)}
            underlayColor={'#68a0ff'} >

            <Text style={localStyles.buttonText}>
              {this.state.navigatorType == UNSET ? "AR" : "VR"}
            </Text>
          </TouchableHighlight>

          <TouchableHighlight style={this.state.navigatorType == UNSET ? localStyles.buttons : localStyles.vrModeButtons}
            onPress={this._getButtonPress(2)}
            underlayColor={'#68a0ff'} >

            <Text style={localStyles.buttonText}>
              {this.state.navigatorType == UNSET ? "VR" : "360"}
            </Text>
          </TouchableHighlight>
        </View>
      </View>
    );
  }
  _getARNavigator() {
    return (
      <View style={localStyles.viroContainer} >
        <ViroARSceneNavigator
          {...this.state.sharedProps}
          initialScene={{scene: InitialARScene}} />

        <View style={{position: 'absolute',  left: 0, right: 0, bottom: 20, alignItems: 'center'}}>
          <TouchableHighlight style={localStyles.exitButton}
            onPress={()=>{this.setState({navigatorType : UNSET, vrMode : UNSET})}}
            underlayColor={'#00000000'} >
            <Text style={localStyles.buttonText}>Exit</Text>
          </TouchableHighlight>
        </View>
      </View>
    );
  }

  _getVRNavigator() {
    return (
      <View style={localStyles.viroContainer} >
        <ViroSceneNavigator {...this.state.sharedProps}
          initialScene={{scene: InitialVRScene}}
          vrModeEnabled={this.state.vrMode}
          onExitViro={()=>{this.setState({navigatorType : UNSET, vrMode : UNSET})}}/>

        <View style={{position: 'absolute',  left: 0, right: 0, bottom: 20, alignItems: 'center'}}>
          <TouchableHighlight style={localStyles.exitButton}
            onPress={()=>{this.setState({navigatorType : UNSET, vrMode : UNSET})}}
            underlayColor={'#00000000'} >
            <Text style={localStyles.buttonText}>Exit</Text>
          </TouchableHighlight>
        </View>
      </View>
    );
  }
  _getButtonPress(buttonNum) {
    if (buttonNum == 1) {
      if (this.state.navigatorType == UNSET) {
        return ()=>{this.setState({navigatorType : AR_NAVIGATOR_TYPE})}
      } else {
        return ()=>{this.setState({vrMode : true})}
      }
    } else {
      if (this.state.navigatorType == UNSET) {
        return ()=>{this.setState({navigatorType : VR_NAVIGATOR_TYPE})}
      } else {
        return ()=>{this.setState({vrMode : false})}
      }
    }

    return () => {
      this.setState({
        navigatorType : navigatorType
      })
    }
  }
}

var localStyles = StyleSheet.create({
  viroContainer : {
    flex : 1,
    backgroundColor: "black",
  },
  outer : {
    flex : 1,
    flexDirection: 'row',
    alignItems:'center',
    backgroundColor: "black",
  },
  inner: {
    flex : 1,
    flexDirection: 'column',
    alignItems:'center',
    backgroundColor: "black",
  },
  titleText: {
    paddingTop: 30,
    paddingBottom: 20,
    color:'#fff',
    textAlign:'center',
    fontSize : 25
  },
  buttonText: {
    color:'#fff',
    textAlign:'center',
    fontSize : 20
  },
  buttons : {
    height: 80,
    width: 150,
    paddingTop:20,
    paddingBottom:20,
    marginTop: 10,
    marginBottom: 10,
    backgroundColor:'#68a0cf',
    borderRadius: 10,
    borderWidth: 1,
    borderColor: '#fff',
  },
  vrModeButtons : {
    height: 80,
    width: 150,
    paddingTop:20,
    paddingBottom:20,
    marginTop: 10,
    marginBottom: 10,
    backgroundColor:'#1111aa',
    borderRadius: 10,
    borderWidth: 1,
    borderColor: '#fff',
  },
  exitButton : {
    height: 50,
    width: 100,
    paddingTop:10,
    paddingBottom:10,
    marginTop: 10,
    marginBottom: 10,
    backgroundColor:'#68a0cf',
    borderRadius: 10,
    borderWidth: 1,
    borderColor: '#fff',
  }
});

AppRegistry.registerComponent('ViroSample', () => ViroExperienceSelector);

module.exports = ViroExperienceSelector;
