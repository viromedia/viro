/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import React, { Component } from 'react';
import PropTypes from 'prop-types';
import {
  AppRegistry,
    StyleSheet,
    Text,
    View,
    Navigator,
    TouchableHighlight,
    NavigatorIOS,
} from 'react-native';

import {
  ViroSceneNavigator,
} from 'react-viro';

var createReactClass = require('create-react-class');

class MyScene extends Component {
  static propTypes = {
    title: PropTypes.string.isRequired,
    navigator: PropTypes.object.isRequired,
  }
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
    }

    _navigate(){
      this.props.navigator.push({
          component: MySceneTwo,
          title: 'Bar That',
        });
    }
}

class MySceneTwo extends Component {
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
    }

    _navigate(){
      this.props.navigator.push({
          component: MainVR,
          title: 'Bar That',
        });
    }

    _navigateBack(){
        this.props.navigator.pop();
    }
};

class MainVR extends Component {
    render() {
        console.log("Daniel MainVR props passed: " + this.props.name);
        return (
        <View style={ MobileStyles.vrContainer }>
            <ViroSceneNavigator
                style={ MobileStyles.button }
                apiKey="API KEY GOES HERE"
                initialScene={{
                    scene: require('./js/release_test/ViroVideoTest.js'),
                    passProps:{...this.props}}} />
                <TouchableHighlight style={ MobileStyles.button } onPress={ () =>
                    this._navigate('Launch into VR') } >
                    <Text style={ MobileStyles.buttonText }>return</Text>
                </TouchableHighlight>
        </View>
        );
    }

    _navigate(){
        this.props.navigator.pop();
    }
};

export default class ViroSample extends Component {
  render() {
    return (
      <NavigatorIOS
        initialRoute={{
          component: MyScene,
          title: 'My Initial Scene',
        }}
        style={{flex: 1}}
      />
    )
  }

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
}

var MobileStyles = StyleSheet.create({
  vrContainer:{
      flex:1,
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
