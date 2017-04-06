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
  Text,
  View
} from 'react-native';

import {
  ViroSceneNavigator,
} from 'react-viro';

var InitialScene = require('./js/release_test/ViroImageTest');

export default class ViroSample extends Component {
  render() {
    return (
      <ViroSceneNavigator apiKey="7EEDCB99-2C3B-4681-AE17-17BC165BF792"
        initialScene={{scene: InitialScene}}
        />
    );
  }
}

AppRegistry.registerComponent('ViroSample', () => ViroSample);


/**
 * ReactJS to ViroJS setup.
 */
/*
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
                <Text style={ MobileStyles.buttonText }>Go to next React Scene</Text>
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
                <Text style={ MobileStyles.buttonText }>Go to Viro Scene</Text>
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
        return (
            <ViroSceneNavigator
               onExitViro={this._navigate}
                apiKey="7EEDCB99-2C3B-4681-AE17-17BC165BF792"
                initialScene={{
                    scene: require('./js/release_test/ViroImageTest.js'),
                    passProps:{...this.props}}}/>
        );
    },

    _navigate(){
            // Pops the Viro Scene off.
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
*/