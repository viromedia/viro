/**
 * Copyright (c) 2017-present, Viro Media, Inc.
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
  CameraRoll,
  View
} from 'react-native';

import {
  ViroScene,
  ViroARScene,
  ViroARPlane,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroImage,
  ViroVideo,
  ViroSkyBox,
  Viro360Video,
  ViroPortal,
  ViroPortalScene,
  Viro360Image,
  ViroText,
  Viro3DObject,
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var CameraRollTest = React.createClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      text : "not tapped",
      visible: true,
      everythingVisible: false,
      photos: undefined,
    }
  },
  componentDidMount() {
    console.log("[CameraRollTest]: componentWillMount");
    CameraRoll.getPhotos({
      first: 20,
      assetType: 'All'
    })
    .then(retValue => {
      this.setState({ photos: retValue.edges });
      console.log("[CameraRollTest]: got " + retValue.edges.length + " Camera Roll assets.");
      console.log(retValue.edges);
    }).catch((err) => {
      console.log("[CameraRollTest]: Error while fetching camera roll assets" + err.message);
    })
  },
  render: function() {
    console.log("[CameraRollTest]: render");
    return (
        <ViroARScene position={[0,0,0]} reticleEnabled={false} onTrackingInitialized={()=>{this.setState({"everythingVisible": true})}}>
            <ViroNode visible={this.state.everythingVisible} >
            {console.log("[CameraRollTest]: rendering ViroImage")}
            {console.log(this.state.photos)}
            <ViroImage
                height={3.024}
                width={4.032}
                source={this.state.photos == undefined ? require("./res/sun_2302.jpg"): { uri : this.state.photos[1].node.image.uri}}
                position={[-0.5,.15,-3.5]}
             />

             <ViroPortalScene passable={true} dragType="FixedDistance" onDrag={()=>{}}>
          <ViroPortal position={[0.5, 0, -1]} scale={[.1, .1, .1]}>
            <Viro3DObject source={require('./res/portal_ship/portal_ship.vrx')}
              resources={[require('./res/portal_ship/portal_ship_diffuse.png'),
                          require('./res/portal_ship/portal_ship_normal.png'),
                          require('./res/portal_ship/portal_ship_specular.png')]}
              type="VRX"/>
          </ViroPortal>
          <Viro360Image source={this.state.photos == undefined ? require("./res/sun_2302.jpg"): { uri : this.state.photos[0].node.image.uri}} />
        </ViroPortalScene>


          </ViroNode>

        </ViroARScene>
    );
  },
  _getPhotos() {
  console.log("[CameraRollTest]: _getPhotos");
  CameraRoll.getPhotos({
    first: 20,
    assetType: 'All'
  })
  .then(retValue => {
    this.setState({ photos: retValue.edges });
    console.log("[CameraRollTest]: got " + retValue.edges.length + " Camera Roll assets.");
    console.log(retValue.edges);
  }).catch((err) => {
    console.log("[CameraRollTest]: Error while fetching camera roll assets" + err.message);
  })
},
  /*
          <ViroARPlane minHeight={0} maxHeight={0} >
            <ViroVideo
              height={.3} width={.3} position={[0,.15,0]} onFinish={this._onFinish("Video")}
              onClick={()=>{console.log("tapped video!!!!")}}
              onDrag={()=>{console.log("dragging!!!!")}}
              source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}}
            />
          </ViroARPlane>



          <ViroARPlane minHeight={.5} minWidth={1} >
            <ViroBox materials="wework_title" scale={[.3, .3, .3]} position={[0, .15, 0]} />
          </ViroARPlane>
  */
  _onTap() {
    console.log("tapped video!!!!")
    this.setState({
      text : "tapped!",
      visible: !this.state.visible
    })
    this.setTimeout( () => {
      this.setState({
        text : "not tapped"
      });
    }, 1000);
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
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#FFFFFF',
  },
  welcome: {
    fontSize: 13,
    textAlign: 'center',
    color: '#ffffff',
    margin: 2,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

ViroMaterials.createMaterials({
  blue: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#0000ff"
  },
  black: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#000000"
  },
  red: {
    shininess: 2.0,
    lightingModel: "Lambert",
    diffuseColor: "#ff0000"
  },
  wework_title: {
    shininess: 1.0,
    lightingModel: "Constant",
    diffuseTexture: {"uri": "https://s3-us-west-2.amazonaws.com/viro/guadalupe_360.jpg"},
    diffuseTexture: require("../res/new_menu_screen.jpg"),
    fresnelExponent: .5,
  },
  box_texture: {
    diffuseTexture: require("../res/sun_2302.jpg"),
  }
});

module.exports = CameraRollTest;
