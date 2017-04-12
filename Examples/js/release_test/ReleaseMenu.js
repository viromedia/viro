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
                         text={"ViroSkyBoxTest"}
                         onClick={() => {this.props.sceneNavigator.jump("ViroSkyBoxTest", {scene:require("./ViroSkyBoxTest"), passProps:{showMenu:true}})}} />
                   <ViroText style={styles.centeredText}  position={[-3, -2, -4]} width={1.5} height ={2}
                         text={"Viro360ImageTest"}
                         onClick={() => {this.props.sceneNavigator.jump("Viro360ImageTest", {scene:require("./Viro360ImageTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[-3, -3, -4]} width={1.5} height ={2}
                         text={"Viro360VideoTest"}
                         onClick={() => {this.props.sceneNavigator.jump("Viro360VideoTest", {scene:require("./Viro360VideoTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[-3, -4, -4]} width={1.5} height ={2}
                         text={"ViroVideoTest"}
                         onClick={() => {this.props.sceneNavigator.jump("ViroVideoTest", {scene:require("./ViroVideoTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[-3, -5, -4]} width={1.5} height ={2}
                         text={"ViroAnimatedComponentTest"}
                         onClick={() => {this.props.sceneNavigator.jump("ViroAnimatedComponentTest", {scene:require("./ViroAnimatedComponentTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[-3, -6, -4]} width={1.5} height ={2}
                         text={"ViroBoxTest"} 
                         onClick={() => {this.props.sceneNavigator.jump("ViroBoxTest", {scene:require("./ViroBoxTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[-3, -7, -4]} width={1.5} height ={2}
                         text={"ViroSphereTest"} 
                         onClick={() => {this.props.sceneNavigator.jump("ViroSphereTest", {scene:require("./ViroSphereTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[0, -1, -4]} width={1.5} height ={2}
                         text={"ViroButtonTest"} 
                         onClick={() => {this.props.sceneNavigator.jump("ViroButtonTest", {scene:require("./ViroButtonTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[0, -2, -4]} width={1.5} height ={2}
                         text={"ViroSurfaceFlexViewTest"} 
                         onClick={() => {this.props.sceneNavigator.jump("ViroSurfaceFlexViewTest", {scene:require("./ViroSurfaceFlexViewTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[0, -3, -4]} width={1.5} height ={2}
                         text={"ViroImageTest"} 
                         onClick={() => {this.props.sceneNavigator.jump("ViroImageTest", {scene:require("./ViroImageTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[0, -4, -4]} width={1.5} height ={2}
                         text={"ViroTextTest"} 
                         onClick={() => {this.props.sceneNavigator.jump("ViroTextTest", {scene:require("./ViroTextTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[0, -5, -4]} width={1.5} height ={2}
                         text={"Viro3DObjectTest"} 
                         onClick={() => {this.props.sceneNavigator.jump("Viro3DObjectTest", {scene:require("./Viro3DObjectTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[0, -6, -4]} width={1.5} height ={2}
                         text={"ViroCameraTest"} 
                         onClick={() => {this.props.sceneNavigator.jump("ViroCameraTest", {scene:require("./ViroCameraTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[0, -7, -4]} width={1.5} height ={2}
                         text={"SceneTest1"} 
                         onClick={() => {this.props.sceneNavigator.jump("SceneTest1", {scene:require("./SceneTest1"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[3,-1, -4]} width={1.5} height ={2}
                         text={"ViroSoundTest"} 
                         onClick={() => {this.props.sceneNavigator.jump("ViroSoundTest", {scene:require("./ViroSoundTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[3,-2, -4]} width={1.5} height ={2}
                         text={"ViroLightTest"} 
                         onClick={() => {this.props.sceneNavigator.jump("ViroLightTest", {scene:require("./ViroLightTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[3,-3, -4]} width={1.5} height ={2}
                         text={"GroupTestBasicProps"} 
                         onClick={() => {this.props.sceneNavigator.jump("GroupTestBasicProps", {scene:require("./GroupTestBasicProps"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[3,-4, -4]} width={1.5} height ={2}
                         text={"GroupTestBasicAnimation"} 
                         onClick={() => {this.props.sceneNavigator.jump("GroupTestBasicAnimation", {scene:require("./GroupTestBasicAnimation"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[3,-5, -4]} width={1.5} height ={2}
                         text={"GroupTestBasicBillboard"} 
                         onClick={() => {this.props.sceneNavigator.jump("GroupTestBasicBillboard", {scene:require("./GroupTestBasicBillboard"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[3,-6, -4]} width={1.5} height ={2}
                         text={"GroupTestMaterials"} 
                         onClick={() => {this.props.sceneNavigator.jump("GroupTestMaterials", {scene:require("./GroupTestMaterials"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[3,-7, -4]} width={1.5} height ={2}
                         text={"GroupTestBasicEvents"} 
                         onClick={() => {this.props.sceneNavigator.jump("GroupTestBasicEvents", {scene:require("./GroupTestBasicEvents"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[3,-8, -4]} width={1.5} height ={2}
                         text={"GroupTestDragEvents"} 
                         onClick={() => {this.props.sceneNavigator.jump("GroupTestDragEvents", {scene:require("./GroupTestDragEvents"), passProps:{showMenu:true}})}} />

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
