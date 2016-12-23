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
  View
} from 'react-native';

import {
  ViroSceneNavigator,
  ViroScene,
  ViroBox,
  Materials,
  ViroNode,
  ViroOrbitCamera,
  ViroCamera,
  ViroImage,
  ViroVideo,
  Viro360Photo,
  Viro360Video,
  ViroFlexView,
} from 'react-viro';

var FlexViewTest = React.createClass({
  render: function() {
    return (
      <ViroScene reticleEnabled={true} >

        <ViroFlexView style={styles.containerVertical} position={[0, 0, -2]} width={3} height={2}>
          <ViroFlexView style={styles.containerInner} >
            <ViroImage style={{flex:1}} source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}}
              onLoadStart={this._onLoadStart("Image")} onLoadEnd={this._onLoadEnd("Image")} />
            <ViroImage style={{flex:1}} source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}}
              onLoadStart={this._onLoadStart("Image")} onLoadEnd={this._onLoadEnd("Image")} />
          </ViroFlexView>
          <ViroFlexView style={styles.containerInner} >
            <ViroImage style={{flex:1}} source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}}
              onLoadStart={this._onLoadStart("Image")} onLoadEnd={this._onLoadEnd("Image")} />
          </ViroFlexView>
        </ViroFlexView>

        <ViroFlexView style={styles.containerHorizontal} position={[2, 0, 0]} width={3} height={2} transformBehaviors={"billboard"} >
          <ViroImage style={{flex:1}} source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}} />
          <ViroImage style={{flex:1}} source={{uri: "http://wiki.magicc.org/images/c/ce/MAGICC_logo_small.jpg"}} />
        </ViroFlexView>
      
        <ViroFlexView style={styles.containerHorizontal} position={[-2, 0, 0]} width={3} height={2} transformBehaviors={"billboard"} >
          <ViroFlexView style={{flex:1, backgroundColor: "#ff0000",}} />
          <ViroFlexView style={{flex:1}} materials={"sunTexture"} />
        </ViroFlexView>
      </ViroScene>
    );
  },
  _onLoadStart(component) {
    return () => {
      console.log("scene1 " + component + " load start");
    }
  },
  _onLoadEnd(component) {
    return () => {
      console.log("scene1 " + component + " load end");
    }
  },
  _onFinish(component) {
    return () => {
      console.log("scene1 " + component + " finished");
    }
  }
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
  containerHorizontal: {
    flexDirection: 'row',
    padding: .1,
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    color: '#333333',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

Materials.createMaterials({
  sunTexture: {
    diffuseTexture: require("./res/sun_2302.jpg"),
  }
});

module.exports = FlexViewTest;
