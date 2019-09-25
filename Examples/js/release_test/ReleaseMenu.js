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
import PropTypes from 'prop-types';

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

var ReleaseMenu = createReactClass({
  propTypes: {
    position:  PropTypes.arrayOf(PropTypes.number),
    showMenu: PropTypes.bool,
  },

  render: function() {
    var releaseMenuPosition = this.props.position != undefined ? this.props.position : [-3, -1, -3];
    if (!this.props.showMenu){
        return (
                <ViroText style={styles.centeredText}  position={releaseMenuPosition} width={2} height ={2}
                          text={"Show Release Menu"} transformBehaviors={["billboard"]} onClick={this._showMenu}/>
        );
    }

    return(
            <ViroScene>
                <ViroNode position={[0,3,-2]}>
                   <ViroText style={styles.centeredText}  position={[0, 0, -4]} width={2} height={2}
                         text={"Release Test Menu"} />

                   <ViroText style={styles.centeredText}  position={[-3, -1, -4]} width={1.5} height={2}
                         text={"ViroSkyBoxTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroSkyBoxTest", {scene:require("./ViroSkyBoxTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[-3, -2, -4]} width={1.5} height={2}
                         text={"Viro360ImageTest"}
                         onClick={() => {this.props.sceneNavigator.replace("Viro360ImageTest", {scene:require("./Viro360ImageTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[-3, -3, -4]} width={1.5} height={2}
                         text={"Viro360VideoTest"}
                         onClick={() => {this.props.sceneNavigator.replace("Viro360VideoTest", {scene:require("./Viro360VideoTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[-3, -4, -4]} width={1.5} height={2}
                         text={"ViroVideoTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroVideoTest", {scene:require("./ViroVideoTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[-3, -5, -4]} width={1.5} height={2}
                         text={"ViroAnimatedComponentTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroAnimatedComponentTest", {scene:require("./ViroAnimatedComponentTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[-3, -6, -4]} width={1.5} height={2}
                         text={"ViroBoxTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroBoxTest", {scene:require("./ViroBoxTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[-3, -7, -4]} width={1.5} height={2}
                         text={"ViroSphereTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroSphereTest", {scene:require("./ViroSphereTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[0, -1, -4]} width={1.5} height={2}
                         text={"ViroButtonTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroButtonTest", {scene:require("./ViroButtonTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[0, -2, -4]} width={1.5} height={2}
                         text={"ViroQuadFlexViewTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroQuadFlexViewTest", {scene:require("./ViroQuadFlexViewTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[0, -3, -4]} width={1.5} height={2}
                         text={"ViroImageTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroImageTest", {scene:require("./ViroImageTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[0, -4, -4]} width={1.5} height={2}
                         text={"ViroTextTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroTextTest", {scene:require("./ViroTextTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[0, -5, -4]} width={1.5} height={2}
                         text={"Viro3DObjectTest"}
                         onClick={() => {this.props.sceneNavigator.replace("Viro3DObjectTest", {scene:require("./Viro3DObjectTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[0, -6, -4]} width={1.5} height={2}
                         text={"ViroCameraTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroCameraTest", {scene:require("./ViroCameraTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[0, -7, -4]} width={1.5} height={2}
                         text={"SceneTest1"}
                         onClick={() => {this.props.sceneNavigator.replace("SceneTest1", {scene:require("./SceneTest1"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[3, 0, -4]} width={1.5} height={2}
                         text={"ViroAnimationTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroAnimationTest", {scene:require("./ViroAnimationTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[3, -1, -4]} width={1.5} height ={2}
                         text={"ViroSoundTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroSoundTest", {scene:require("./ViroSoundTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[3, -2, -4]} width={1.5} height={2}
                         text={"ViroLightTest"}
                         onClick={() => {this.props.sceneNavigator.replace("ViroLightTest", {scene:require("./ViroLightTest"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[3, -3, -4]} width={1.5} height={2}
                         text={"GroupTestBasicProps"}
                         onClick={() => {this.props.sceneNavigator.replace("GroupTestBasicProps", {scene:require("./GroupTestBasicProps"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[3, -4, -4]} width={1.5} height={2}
                         text={"GroupTestBasicAnimation"}
                         onClick={() => {this.props.sceneNavigator.replace("GroupTestBasicAnimation", {scene:require("./GroupTestBasicAnimation"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[3, -5, -4]} width={1.5} height={2}
                         text={"GroupTestBasicBillboard"}
                         onClick={() => {this.props.sceneNavigator.replace("GroupTestBasicBillboard", {scene:require("./GroupTestBasicBillboard"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[3, -6, -4]} width={1.5} height={2}
                         text={"GroupTestMaterials"}
                         onClick={() => {this.props.sceneNavigator.replace("GroupTestMaterials", {scene:require("./GroupTestMaterials"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[3, -7, -4]} width={1.5} height={2}
                         text={"GroupTestBasicEvents"}
                         onClick={() => {this.props.sceneNavigator.replace("GroupTestBasicEvents", {scene:require("./GroupTestBasicEvents"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText} position={[3, -8, -4]} width={1.5} height={2}
                         text={"GroupTestDragEvents"}
                         onClick={() => {this.props.sceneNavigator.replace("GroupTestDragEvents", {scene:require("./GroupTestDragEvents"), passProps:{showMenu:true}})}} />

                   <ViroText style={styles.centeredText}  position={[-3, -8, -4]} width={1.5} height={2}
                         text={"PhysicsGroupTest"}
                         onClick={() => {this.props.sceneNavigator.replace("PhysicsGroupTest", {scene:require("./PhysicsGroupTest"), passProps:{showMenu:true}})}} />
                   <ViroText style={styles.centeredText}  position={[0, -8, -4]} width={1.5} height={2}
                         text={"PhysicsForcesTest"}
                         onClick={() => {this.props.sceneNavigator.replace("PhysicsForcesTest", {scene:require("./PhysicsForcesTest"), passProps:{showMenu:true}})}} />
                   <ViroText style={styles.centeredText} position={[-3, -9, -4]} width={1.5} height={2}
                         text={"PhysicsCollisionTest"}
                         onClick={() => {this.props.sceneNavigator.replace("PhysicsCollisionTest", {scene:require("./PhysicsCollisionTest"), passProps:{showMenu:true}})}} />
                   <ViroText style={styles.centeredText}  position={[0, -9, -4]} width={1.5} height={2}
                         text={"PhysicsPropertiesTest"}
                         onClick={() => {this.props.sceneNavigator.replace("PhysicsPropertiesTest", {scene:require("./PhysicsPropertiesTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[3, -9, -4]} width={1.5} height={2}
                          text={"PhysicsScaleTest"}
                          onClick={() => {this.props.sceneNavigator.replace("PhysicsScaleTest", {scene:require("./PhysicsScaleTest"), passProps:{showMenu:true}})}}
                   />

                   <ViroText style={styles.centeredText}  position={[-3, -10, -4]} width={1.5} height={2}
                          text={"Particle Test"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroParticleTest", {scene:require("./ViroParticleTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[0, -10, -4]} width={1.5} height={2}
                          text={"ShadowGroupTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroShadowGroupTest", {scene:require("./ViroShadowGroupTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[3, -10, -4]} width={1.5} height={2}
                          text={"ViroShadowTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroShadowTest", {scene:require("./ViroShadowTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[-3, -11, -4]} width={1.5} height={2}
                          text={"ViroPortalTestVR"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroPortalTestVR", {scene:require("./ViroPortalTestVR"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText} position={[-3, -12, -4]} width={1.5} height={2}
                          text={"ViroFlexViewLayoutTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroFlexViewLayoutTest", {scene:require("./ViroFlexViewLayoutTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[0, -11, -4]} width={1.5} height={2}
                          text={"ViroAnimationTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroAnimationTest", {scene:require("./ViroAnimationTest"), passProps:{showMenu:true}})}}
                    />
                   <ViroText style={styles.centeredText}  position={[3, -11, -4]} width={1.5} height={2}
                          text={"ViroPivotTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroPivotTest", {scene:require("./ViroPivotTest"), passProps:{showMenu:true}})}}
                   />

                   <ViroText style={styles.centeredText}  position={[3, -12, -4]} width={1.5} height={2}
                          text={"ViroAnimatedImageTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroAnimatedImageTest", {scene:require("./ViroAnimatedImageTest"), passProps:{showMenu:true}})}}
                   />

                   <ViroText style={styles.centeredText}  position={[0, -12, -4]} width={1.5} height={2}
                          text={"ViroPolylineTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroPolylineTest", {scene:require("./ViroPolylineTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -1, -4]} width={1.5} height={2}
                          text={"ViroPbrTestBasic"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroPbrTestBasic", {scene:require("./ViroPbrTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -2, -4]} width={1.5} height={2}
                          text={"ViroPbrTestSurfaces"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroPbrTestSurfaces", {scene:require("./ViroPbrTestSurfaces"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -3, -4]} width={1.5} height={2}
                          text={"ViroBlendModes"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroBlendModes", {scene:require("./ViroBlendingTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -4, -4]} width={1.5} height={2}
                          text={"ViroInternationalText"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroInternationalText", {scene:require("./ViroInternationalTextTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -5, -4]} width={1.5} height={2}
                          text={"ViroFontText"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroFontText", {scene:require("./ViroFontTextTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -6, -4]} width={1.5} height={2}
                          text={"ViroMaterialVideoTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroMaterialVideoTest", {scene:require("./ViroMaterialVideoTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -7, -4]} width={1.5} height={2}
                          text={"ViroHierarchyTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroHierarchyTest", {scene:require("./ViroHierarchyTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -8, -4]} width={1.5} height={2}
                          text={"ViroGLTFTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroGLTFTest", {scene:require("./ViroGLTFTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -9, -4]} width={1.5} height={2}
                          text={"ViroCameraHUDTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroCameraHUDTest", {scene:require("./ViroCameraHUDTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -10, -4]} width={1.5} height={2}
                          text={"ViroChromaKeyTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroChromaKeyTest", {scene:require("./ViroChromaKeyTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -11, -4]} width={1.5} height={2}
                          text={"ViroPolygonTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroPolygonTest", {scene:require("./ViroPolygonTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -12, -4]} width={1.5} height={2}
                          text={"ViroTextSpecialTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroTextSpecialTest", {scene:require("./ViroTextSpecialTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -13, -4]} width={1.5} height={2}
                          text={"ViroColorMaskTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroColorMaskTest", {scene:require("./ViroColorMaskTest"), passProps:{showMenu:true}})}}
                   />
                   <ViroText style={styles.centeredText}  position={[6, -14, -4]} width={1.5} height={2}
                          text={"ViroGeometryTest"}
                          onClick={() => {this.props.sceneNavigator.replace("ViroGeometryTest", {scene:require("./ViroGeometryTest"), passProps:{showMenu:true}})}}
                   />

                </ViroNode>
            </ViroScene>
    );
  },

  _showMenu(){
    this.props.sceneNavigator.replace("ReleaseMenu", {scene:require("./ReleaseMenu"), passProps:{showMenu:true}});
  },
});

const styles = StyleSheet.create({
   centeredText: {
     fontSize: 25,
     fontFamily: 'Arial',
     color: '#ffffff',
   }
});

module.exports = ReleaseMenu;
