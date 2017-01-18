/**
 * Copyright (c) 2017-present, Viro, Inc.
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
  ViroImage,
  ViroVideo,
  Viro360Image,
  Viro360Video,
  ViroFlexView,
  ViroUtils,
  ViroAnimations,
  ViroAnimatedComponent,
  ViroSurface,
  ViroSkyBox,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;

var stateOne = {
  flex: {flex: 1},
  material: "redColor",
  width: 3
};

var stateTwo = {
  flex: {flex: 2},
  material: "greenColor",
  width: 3
}

/*
 This file tests a simple flex view and updating properties within it.
 */
var FlexViewTest = React.createClass({
  getInitialState: function() {
    return {
      ...stateOne,
      state: 1
    }
  },
  render: function() {
    return (
      <ViroScene reticleEnabled={true} onTap={this._onTap}>

        <ViroSkyBox color="#ff69b4" />

        <ViroFlexView style={styles.containerVertical} position={polarToCartesian([2, 0, 0])} width={3} height={2} opacity={1}>
          <ViroFlexView style={styles.containerInner} >
            <ViroSurface style={this.state.flex} materials={"redColor"} />
            <ViroImage style={{flex:1}} source ={require("./res/sun_2302.jpg")} />
          </ViroFlexView>
          <ViroFlexView style={styles.containerInner} >
            <ViroImage style={{flex:1}} source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}} />
          </ViroFlexView>
        </ViroFlexView>

      </ViroScene>
    );
  },
  _onTap(component) {
    if (this.state.state == 1) {
      this.setState({
        ...stateTwo,
        state: 2
      })
    } else {
      this.setState({
        ...stateOne,
        state: 1
      })
    }
  },
  /*
  <Viro360Image source={{uri: "http://cdn3-www.dogtime.com/assets/uploads/gallery/pembroke-welsh-corgi-dog-breed-pictures/prance-8.jpg"}} rotation={[-30,90,0]} />
  */
});

const styles = StyleSheet.create({
  containerVertical: {
    flexDirection: 'column',
  },
  containerInner: {
    flexDirection: 'row',
    flex: 1,
    padding: .1,
    backgroundColor: "#0000ff",
  },
  containerInnerTwo: {
    flexDirection: 'row',
    flex: 2,
    padding: .1,
    backgroundColor: "#0000ff",
  },
  containerHorizontal: {
    flexDirection: 'row',
    padding: .1,
  },
});

stateOne.innerStyle = styles.containerInner;
stateTwo.innerStyle = styles.containerInnerTwo;

ViroMaterials.createMaterials({
  redColor: {
    diffuseColor: "#ff0000"
  },
  greenColor: {
    diffuseColor: "#00ff00"
  },
});

module.exports = FlexViewTest;
