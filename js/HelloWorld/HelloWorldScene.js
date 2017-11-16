'use strict';

import React, { Component } from 'react';
import {StyleSheet} from 'react-native';

import {
  ViroScene,
  ViroText,
  Viro360Image,
} from 'react-viro';

var createReactClass = require('create-react-class');

var HelloWorldScene = createReactClass({
  getInitialState() {
    return {

    };
  },
  render: function() {
    return (
     <ViroScene>
      <Viro360Image source={require('./res/guadalupe_360.jpg')} />
      <ViroText text="Hello World!" position={[0, 0, -2]} style={styles.helloWorldTextStyle} />
     </ViroScene>
    );
  },
});

var styles = StyleSheet.create({
  helloWorldTextStyle: {
    fontFamily: 'HelveticaNeue-Medium',
    fontSize: 40,
    color: '#ffffff',
  },
});

module.exports = HelloWorldScene;
