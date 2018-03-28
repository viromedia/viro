/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

'use strict';

import React, { Platform } from 'react';
import {
  StyleSheet,
  ViroScene,
  ViroAmbientLight,
  ViroOmniLight,
  Viro360Image,
  ViroMaterials,
  ViroAnimations,
  ViroAnimatedComponent,
  ViroSkyBox,
  ViroQuad,
  Viro3DObject,
  ViroUtils,
  ViroDirectionalLight,
  ViroButton,
  ViroSphere,
  ViroImage,
  ViroNode
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;

var createReactClass = require('create-react-class');

var HomeMenuScene = createReactClass({
  getInitialState() {
    return {
      runSphereAnimation: false,
      moveForwardAnimation1: false,
      moveForwardAnimation2: false,
      button1Animation: "moveForward1",
      button2Animation: "moveForward2",
    };
  },
  render: function() {
    return (
      <ViroScene >
        <ViroAmbientLight color="#ffffff"/>
    
        <Viro360Image source={{uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/360_horseshoe.jpg"}}
        rotation={[0,180,0]}/>

        <Viro3DObject source={require("./res/home_logo_viro_top.obj")} position={polarToCartesian([6.5, 0, 30])}
        materials={["home_logo_viro"]} transformBehaviors={"billboardY"} type="OBJ" />

        <ViroSphere radius={1.7} position={polarToCartesian([6.5, -35, 9])} materials={["spherematerial1"]} />
        <ViroAnimatedComponent animation='animateSphere' run={this.state.runSphereAnimation}>
          <ViroSphere radius={1.7} position={polarToCartesian([6.5, 0, 9])} materials={["spherematerial2"]} onClick={this._onSphereClicked} facesOutward={false}/>
        </ViroAnimatedComponent>
        <ViroSphere radius={1.7} position={polarToCartesian([6.5, 35, 9])} materials={["spherematerial3"]} />

        <ViroAnimatedComponent animation={this.state.button1Animation} run={this.state.moveForwardAnimation1} onFinish={this._onAnimationFinished}>
          <ViroNode>
              <Viro3DObject source={require("./res/btn_home_a.obj")} position={polarToCartesian([6.1, -40, -20])}
              materials={["home_button_cameraroll"]} transformBehaviors={"billboardY"} onHover={this._cameraRollMoveForward} type="OBJ"/>
              <Viro3DObject source={require("./res/home_cameraroll.obj")} position={polarToCartesian([5.8, -40, -20])}
              materials={["home_cameraroll"]} transformBehaviors={"billboardY"} type="OBJ"/>
              <ViroImage source={require('./res/camera_roll_shadow.png')} transformBehaviors={"billboardY"} position={polarToCartesian([5.9, -41, -21])}/>
          </ViroNode>
        </ViroAnimatedComponent>
        <ViroAnimatedComponent animation={this.state.button2Animation} run={this.state.moveForwardAnimation2} onFinish={this._onAnimationFinished}>
          <ViroNode>
            <Viro3DObject source={require("./res/btn_home_a.obj")} position={polarToCartesian([6.1, -20, -20])}
            materials={["home_button_viro"]} transformBehaviors={"billboardY"} onHover={this._viroMoveForward} type="OBJ"/>
            <Viro3DObject source={require("./res/btn_viro.obj")} position={polarToCartesian([5.8, -20, -14])}
            materials={["home_viro"]} transformBehaviors={"billboardY"} scale={[0.1,0.1,0.1]} type="OBJ"/>
            <ViroImage source={require('./res/viro_shadow.png')} transformBehaviors={"billboardY"} position={polarToCartesian([5.9, -20.5, -20.5])}/>
          </ViroNode>
        </ViroAnimatedComponent>
        <Viro3DObject source={require("./res/btn_home_a.obj")} position={polarToCartesian([6.1, 0, -20])}
        materials={["home_button_flickr"]} transformBehaviors={"billboardY"} type="OBJ"/>
        <Viro3DObject source={require("./res/home_global360.obj")} position={polarToCartesian([5.4, -2, -22])}
        materials={["home_flickr"]} transformBehaviors={"billboardY"} scale={[0.1,0.1,0.1]} type="OBJ"/>
        <ViroImage source={require('./res/global360_shadow.jpg')} transformBehaviors={"billboardY"} position={polarToCartesian([5.9, 0, -20.5])} scale={[1.6,1.6,1.6]}/>

        <Viro3DObject source={require("./res/btn_home_a.obj")} position={polarToCartesian([6.1, 20, -20])}
        materials={["home_button_fblive"]} transformBehaviors={"billboardY"} type="OBJ" />
        <Viro3DObject source={require("./res/home_livevideo.obj")} position={polarToCartesian([5.7, 20, -20])}
        materials={["home_livevideo"]} transformBehaviors={"billboardY"} type="OBJ" />
        <ViroImage source={require('./res/fb_live_shadow.png')} transformBehaviors={"billboardY"} position={polarToCartesian([5.8, 20.5, -20.5])}/>


        <Viro3DObject source={require("./res/btn_home_a.obj")} position={polarToCartesian([6.1, 40, -20])}
        materials={["home_button_movie"]} transformBehaviors={"billboardY"} type="OBJ"/>
        <Viro3DObject source={require("./res/home_movie.obj")} position={polarToCartesian([6, 40, -20])}
        materials={["home_movie"]} transformBehaviors={"billboardY"} type="OBJ"/>
      </ViroScene>
    );
  },
  _onSphereClicked: function(source) {
      this.setState({
          runSphereAnimation: true
      });
  },
  _cameraRollMoveForward: function(source, isHovering) {
    if (isHovering) {
      this.setState({
        moveForwardAnimation1: true,
        button1Animation: 'moveForward1',
        moveForwardAnimation2: false,

      });
    } else if (!isHovering) {
      this.setState({
        moveForwardAnimation1: true,
        button1Animation: 'moveBack1',
        moveForwardAnimation2: false,
      });
    }
  },
  _viroMoveForward: function(source, isHovering) {
    if (isHovering) {
      this.setState({
        moveForwardAnimation2: true,
        button2Animation: 'moveForward2',
        moveForwardAnimation1: false,
      });
    } else if (!isHovering) {
      this.setState({
        moveForwardAnimation2: true,
        button2Animation: 'moveBack2',
        moveForwardAnimation1: false,
      });
    }
  },
  _onAnimationFinished: function() {
    this.setState({
        moveForwardAnimation1: false,
        moveForwardAnimation2: false,
      });
  }
});

ViroMaterials.createMaterials({
  home_logo_viro: {
    shininess:1.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/home_logo_viro.png"),
  },
  home_cameraroll: {
    shininess:1.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/camera_roll.jpg"),
  },
  home_button_cameraroll: {
    shininess:1.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/button_bg_cameraroll.jpg"),
  },
  home_flickr: {
    shininess:1.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/global360.jpg"),
  },
  home_button_flickr: {
    shininess:1.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/button_bg_global360.jpg"),
  },
  home_viro: {
    shininess:1.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/viro_logo.jpg"),
  },
  home_button_viro: {
    shininess:1.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/button_bg_viro.jpg"),
  },
  home_livevideo: {
    shininess:1.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/fb_live.jpg"),
  },
    home_button_fblive: {
    shininess:1.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/button_bg_fblive.jpg"),
  },
  home_movie: {
    shininess:1.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/fearless_train.jpg"),
  },
  home_button_movie: {
    shininess:1.0,
    lightingModel: "Lambert",
    diffuseTexture: require("./res/button_revoked.jpg"),
  },
  spherematerial1: {
    shininess:1.0,
    lightingModel: "Blinn",
    diffuseTexture: {uri:"http://s3-us-west-2.amazonaws.com/viro/Explorer/360_waikiki.jpg"},
  },
    spherematerial2: {
    shininess:1.0,
    lightingModel: "Blinn",
    diffuseTexture: require("./res/demo_wework_commons_south_4.jpg"),
  },
  spherematerial3: {
    shininess:1.0,
    lightingModel: "Blinn",
    diffuseTexture: {uri:"http://s3-us-west-2.amazonaws.com/viro/Explorer/360_conference.jpg"},
  },

});

ViroAnimations.registerAnimations({
    animateSphere:{properties:{positionX:0.0, positionY:0.0, positionZ:0.0},
                  easing:"EaseOut", 
                  duration: 700},
    moveForward1:{properties:{positionX:"+=0.5",positionY:"+=0.3",positionZ:"+=0.3"},
                  easing:"EaseOut", 
                  duration: 700},
    moveBack1:{properties:{positionX:"-=0.5",positionY:"-=0.3",positionZ:"-=0.3"},
                  easing:"EaseOut", 
                  duration: 700},
    moveForward2:{properties:{positionX:"+=0.5",positionY:"+=0.3",positionZ:"+=0.3"},
                  easing:"EaseOut", 
                  duration: 700},
    moveBack2:{properties:{positionX:"-=0.5",positionY:"-=0.3",positionZ:"-=0.3"},
                  easing:"EaseOut", 
                  duration: 700},

    cardIn:{properties:{scaleX:1, scaleY:.6, scaleZ:1, opacity: 1}, easing:"Bounce", duration: 2000},

    testLoopColorYellow:{properties:{material:"yellowColor"}, duration:3000},
    testLoopColorYellowMaterial:{properties:{material:"redColor"}, duration:3000},
    testLoopColorBlue:{properties:{material:"blueColor"}, duration:3000},
    testLoopColor:[
      ["testLoopColorYellow", "testLoopColorBlue"]
    ],
    testAnimateTexture:{properties:{material:"cardpetite"}, duration:3000},

    testMakeOpaque: {properties:{opacity:1.0}, duration: 1000},
    testMakeTransparent: {properties:{opacity:0}, duration: 1000},
    testLoopOpacity: [
      ["testMakeTransparent", "testMakeOpaque"]
    ]
});

module.exports = HomeMenuScene;