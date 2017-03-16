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
    View,
    Navigator,
    TouchableHighlight
} from 'react-native';

import {
  ViroSceneNavigator,
} from 'react-viro';

var ViroSample = React.createClass({
  render() {
    return (
      <Navigator
      	style={{ flex:1 }}
        initialRoute={{ id: 'Main' }}
        renderScene={ this.renderScene } />
    );
  },

  renderScene(route, navigator) {
      switch (route.id) {
        case 'Main':
          return (<Main navigator={navigator} {...route.passProps} title="Main" />);
        case 'Main2':
                  return (<Main2 navigator={navigator} {...route.passProps} title="Main2" />);

        case 'MainVR':
          return (<MainVR navigator={navigator} {...route.passProps} title="MainVR" />);
      }
  }
});

var Main = React.createClass({
    render() {
        return (
          <View style={ MobileStyles.container }>
            <Text style={ MobileStyles.heading }>Hello from Main</Text>
            <TouchableHighlight style={ MobileStyles.button } onPress={ () =>
                    this._navigate('Launch into Main2') } >
                <Text style={ MobileStyles.buttonText }>GO GO GO</Text>
            </TouchableHighlight>
          </View>
        );
    },

    _navigate(){
        this.props.navigator.push({ id: 'Main2',
              passProps: {
                name: "Test"
              }
            });
    }
});

var Main2 = React.createClass({
    render() {
        return (
          <View style={ MobileStyles.container }>
            <Text style={ MobileStyles.heading }>Hello from Main2</Text>
            <TouchableHighlight style={ MobileStyles.button } onPress={ () =>
                    this._navigate('Launch into VR') } >
                <Text style={ MobileStyles.buttonText }>GO GO GO</Text>
            </TouchableHighlight>
            <TouchableHighlight style={ MobileStyles.button } onPress={ () =>
                                this._navigateBack() } >
                <Text style={ MobileStyles.buttonText }>Return</Text>
            </TouchableHighlight>
          </View>
        );
    },

    _navigate(){
        this.props.navigator.push({ id: 'MainVR',
              passProps: {
                name: "Test123"
              }
            });
    },
    _navigateBack(){
        this.props.navigator.pop();
    }
});

var MainVR = React.createClass({
    render() {
        console.log("Daniel MainVR props passed: " + this.props.name);
        return (
        <View style={ MobileStyles.vrContainer }>
            <ViroSceneNavigator
                style={ MobileStyles.button }
                apiKey="ANDROID_DOESNT_YET_NEED_A_KEY"
                initialScene={{
                    scene: require('./js/release_test/ViroVideoTest.js'),
                    passProps:{...this.props}}} />
                <TouchableHighlight style={ MobileStyles.button } onPress={ () =>
                    this._navigate('Launch into VR') } >
                    <Text style={ MobileStyles.buttonText }>return</Text>
                </TouchableHighlight>
        </View>
        );
    },

    _navigate(){
            this.props.navigator.pop();
        }
});

var MobileStyles = StyleSheet.create({
  vrContainer:{
      height:200,
      width:200
  },
  container: {
    flex: 1,
   	marginTop: 80
  },
   heading: {
  	fontSize:22,
    marginBottom:10
  },
  button: {
  	height:60,
    justifyContent: 'center',
    backgroundColor: '#efefef',
    alignItems: 'center',
    justifyContent: 'center'
  },
  buttonText: {
  	fontSize:20
  }
});


AppRegistry.registerComponent('ViroSample', () => ViroSample);
