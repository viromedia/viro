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
var PropTypes = require('react/lib/ReactPropTypes');

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

var ReleaseMenu = React.createClass({
propTypes: {
    showMenu: PropTypes.bool,
  },

  render: function() {
    console.log("Daniel RELEASE MENU RENDER this.props.showMenu:" + this.props.showMenu);
    if (!this.props.showMenu){
        console.log("Daniel RELEASE MENU RENDER 2 ");

        return (
                <ViroText style={styles.centeredText}  position={[-3, -1, -3]} width={2} height ={2}
                                        text={"Show Release Menu"} transformBehaviors={["billboard"]} onClick={this._showMenu}/>
        );
    }


    return(
            <ViroScene>
                <ViroNode position={[0,0,-2]}>
                   <ViroText style={styles.centeredText}  position={[0, 0, -4]} width={2} height ={2}
                         text={"Release Test Menu"} />

                   <ViroText style={styles.centeredText}  position={[-3, -1, -4]} width={1.5} height ={2}
                         text={"ViroSkyBoxTest"} onClick={this.goToScene("ViroSkyBoxTest", require("./ViroSkyBoxTest"), true)} />

                   <ViroText style={styles.centeredText}  position={[-3, -2, -4]} width={1.5} height ={2}
                         text={"Viro360ImageTest"} onClick={this.goToScene("Viro360ImageTest", require("./Viro360ImageTest"), true)} />

                   <ViroText style={styles.centeredText}  position={[-3, -3, -4]} width={1.5} height ={2}
                         text={"Viro360VideoTest"} onClick={this.goToScene("Viro360VideoTest", require("./Viro360VideoTest"), true)} />

                   <ViroText style={styles.centeredText}  position={[-3, -4, -4]} width={1.5} height ={2}
                         text={"ViroVideoTest"} onClick={this.goToScene("ViroVideoTest", require("./ViroVideoTest"), true)} />

                   <ViroText style={styles.centeredText} position={[-3, -5, -4]} width={1.5} height ={2}
                         text={"ViroAnimatedComponentTest"} onClick={this.goToScene("ViroAnimatedComponentTest", require("./ViroAnimatedComponentTest"), true)} />

                   <ViroText style={styles.centeredText} position={[-3, -6, -4]} width={1.5} height ={2}
                         text={"ViroBoxTest"} onClick={this.goToScene("ViroBoxTest", require("./ViroBoxTest"), true)} />

                   <ViroText style={styles.centeredText}  position={[-3, -7, -4]} width={1.5} height ={2}
                         text={"ViroSphereTest"} onClick={this.goToScene("ViroSphereTest", require("./ViroSphereTest"), true)} />

                   <ViroText style={styles.centeredText}  position={[0, -1, -4]} width={1.5} height ={2}
                         text={"ViroButtonTest"} onClick={this.goToScene("ViroButtonTest", require("./ViroButtonTest"), true)} />

                   <ViroText style={styles.centeredText}  position={[0, -2, -4]} width={1.5} height ={2}
                         text={"ViroSurfaceFlexViewTest"} onClick={this.goToScene("ViroSurfaceFlexViewTest", require("./ViroSurfaceFlexViewTest"), true)} />

                   <ViroText style={styles.centeredText}  position={[0, -3, -4]} width={1.5} height ={2}
                         text={"ViroImageTest"} onClick={this.goToScene("ViroImageTest", require("./ViroImageTest"), true)} />

                   <ViroText style={styles.centeredText} position={[0, -4, -4]} width={1.5} height ={2}
                         text={"ViroTextTest"} onClick={this.goToScene("ViroTextTest", require("./ViroTextTest"), true)} />

                   <ViroText style={styles.centeredText} position={[0, -5, -4]} width={1.5} height ={2}
                         text={"Viro3DObjectTest"} onClick={this.goToScene("Viro3DObjectTest", require("./Viro3DObjectTest"), true)} />

                   <ViroText style={styles.centeredText}  position={[0, -6, -4]} width={1.5} height ={2}
                         text={"ViroCameraTest"} onClick={this.goToScene("ViroCameraTest", require("./ViroCameraTest"), true)} />

                   <ViroText style={styles.centeredText}  position={[0, -7, -4]} width={1.5} height ={2}
                         text={"SceneTest1"} onClick={this.goToScene("SceneTest1", require("./SceneTest1"), true)} />

                   <ViroText style={styles.centeredText}  position={[3,-1, -4]} width={1.5} height ={2}
                         text={"ViroSoundTest"} onClick={this.goToScene("ViroSoundTest", require("./ViroSoundTest"), true)} />

                   <ViroText style={styles.centeredText}  position={[3,-2, -4]} width={1.5} height ={2}
                         text={"ViroLightTest"} onClick={this.goToScene("ViroLightTest", require("./ViroLightTest"), true)} />

                   <ViroText style={styles.centeredText} position={[3,-3, -4]} width={1.5} height ={2}
                         text={"GroupTestBasicProps"} onClick={this.goToScene("GroupTestBasicProps", require("./GroupTestBasicProps"), true)} />

                   <ViroText style={styles.centeredText} position={[3,-4, -4]} width={1.5} height ={2}
                         text={"GroupTestBasicAnimation"} onClick={this.goToScene("GroupTestBasicAnimation", require("./GroupTestBasicAnimation"), true)} />

                   <ViroText style={styles.centeredText}  position={[3,-5, -4]} width={1.5} height ={2}
                         text={"GroupTestBasicBillboard"} onClick={this.goToScene("GroupTestBasicBillboard", require("./GroupTestBasicBillboard"), true)} />

                   <ViroText style={styles.centeredText}  position={[3,-6, -4]} width={1.5} height ={2}
                         text={"GroupTestMaterials"} onClick={this.goToScene("GroupTestMaterials", require("./GroupTestMaterials"), true)} />

                   <ViroText style={styles.centeredText}  position={[3,-7, -4]} width={1.5} height ={2}
                         text={"GroupTestBasicEvents"} onClick={this.goToScene("GroupTestBasicEvents", require("./GroupTestBasicEvents"), true)} />

                   <ViroText style={styles.centeredText} position={[3,-8, -4]} width={1.5} height ={2}
                         text={"GroupTestDragEvents"} onClick={this.goToScene("GroupTestDragEvents", require("./GroupTestDragEvents"), true)} />

                </ViroNode>
                </ViroScene>
    );
  },

_showMenu(){
    console.log("Daniel SHOW THIS MENi");
    this.props.sceneNavigator.jump("ReleaseMenu", {scene:require("./ReleaseMenu"), passProps:{showMenu:true}});
},

    popScene(){
    this.props.sceneNavigator.pop();
    },

  goToScene(sceneKey, scene, isJumping){
    return () => {
        if (isJumping){
        this.props.sceneNavigator.jump(sceneKey, {scene:scene, passProps:{showMenu:true}});
        } else {
        this.props.sceneNavigator.push(sceneKey, {scene:scene, passProps:{showMenu:true}});
        }


    }
  }
});

const styles = StyleSheet.create({
   centeredText: {
        fontSize: 25,
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


module.exports = ReleaseMenu;
