'use strict';

import React, { Component } from 'react';

import {
  StyleSheet,
  ViroScene,
  ViroText,
  Viro360Photo,
} from 'react-viro';

var HelloWorldScene = React.createClass({
  getInitialState() {
    return {

    };
  },
  render: function() {
    return (
     <ViroScene>
      <Viro360Photo source={require('./res/guadalupe_360.jpg')} />
      <ViroText text="Hello World!" position={[0, 0, -2]} style={styles.helloWorldTextStyle} />
     </ViroScene>
    );
  },
});

var styles = StyleSheet.create({
  helloWorldTextStyle: {
    fontFamily: 'HelveticaNeue-Medium',
    fontSize: 70,
    color: '#ffffff',
  },
});

module.exports = HelloWorldScene;
