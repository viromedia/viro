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
  ViroText,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroImage,
  ViroVideo,
  ViroSkyBox,
  Viro360Video
} from 'react-viro';

var createReactClass = require('create-react-class');

var SceneTest3 = createReactClass({

  render: function() {
    return (
        <ViroScene position={[0,0,0]} reticleEnabled={false} >

           <ViroText style={styles.centeredText}  position={[0, 0, -4]} width={2} height ={2}
                 text={"Current: "+ this.props.title} />

           <ViroText style={styles.centeredText}  position={[-2, -1, -4]} width={1.5} height ={2}
                 text={"Push Scene 1"} onClick={this.goToScene("scene1", false)} />

           <ViroText style={styles.centeredText}  position={[0, -1, -4]} width={1.5} height ={2}
                 text={"Push Scene 2"} onClick={this.goToScene("scene2", false)} />

           <ViroText style={styles.centeredText}  position={[2,-1, -4]} width={1.5} height ={2}
                 text={"Push Scene 3"} onClick={this.goToScene("scene3", false)} />

           <ViroText style={styles.centeredText}  position={[-2, -2, -4]} width={1.5} height ={2}
                 text={"Jump Scene 1"} onClick={this.goToScene("scene1", true)} />

           <ViroText style={styles.centeredText} position={[0, -2, -4]} width={1.5} height ={2}
                 text={"Jump Scene 2"} onClick={this.goToScene("scene2", true)} />

           <ViroText style={styles.centeredText} position={[2,-2, -4]} width={1.5} height ={2}
                 text={"Jump Scene 3"} onClick={this.goToScene("scene3", true)} />

                 <ViroText style={styles.centeredText} position={[0,-3, -4]} width={1.5} height ={2}
                                  text={"Pop Scene"} onClick={this.popScene} />
        </ViroScene>
    );
  },
    popScene(){
    this.props.sceneNavigator.pop();
    },


  goToScene(sceneKey, isJumping){
    return () => {
      var scene;
      if (sceneKey == "scene1"){
            scene = require("./SceneTest1");
      } else if (sceneKey == "scene2") {
            scene = require("./SceneTest2");
      } else {
            scene = require("./SceneTest3");
      }


        if (isJumping){
        this.props.sceneNavigator.jump(sceneKey, {scene:scene, passProps:{title:sceneKey}});
        } else {
        this.props.sceneNavigator.push(sceneKey, {scene:scene, passProps:{title:sceneKey}});
        }


    }
  }
});

const styles = StyleSheet.create({
   centeredText: {
        fontSize: 30,
        fontFamily: 'Arial',
        color: '#ffffff',
   }
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
  box_texture: {
    diffuseTexture: require("../res/sun_2302.jpg"),
  }
});


module.exports = SceneTest3;
