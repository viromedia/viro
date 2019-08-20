/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

import React, { Component } from 'react';
import Config from 'react-native-config'
import {
  AppRegistry,
  Text,
  View,
  StyleSheet,
  PixelRatio,
  TouchableHighlight,
  NativeModules,
  SectionList,
  SafeAreaView
} from 'react-native';

import {
  ViroSceneNavigator,
  ViroARSceneNavigator,
  Viro3DSceneNavigator,
  ViroVRSceneNavigator,
  ViroUtils,
} from 'react-viro';

import renderIf from './js/release_test/renderIf';

var InitialVRScene = require('./js/release_test/ViroSkyBoxTest');
var InitialARScene = require('./js/AR/release_test/ARImageTrackingTest');

var UNSET = "UNSET";
var VR_NAVIGATOR_TYPE = "VR";
var AR_NAVIGATOR_TYPE = "AR";
var SCENE_NAVIGATOR_TYPE = "3D Scene";
var VR_DEPRECATED_TYPE = "VR (deprecated)";

var isARSupportedOnDevice = ViroUtils.isARSupportedOnDevice;

var ARTests = [
  { name: 'ARSceneAndNavigatorTest', scene: require('./js/AR/release_test/ARSceneAndNavigatorTest') },
  { name: 'ARDragTest', scene: require('./js/AR/release_test/ARDragTest') },
  { name: 'ARFBXVersionTest', scene: require('./js/AR/release_test/ARFBXVersionTest') },
  { name: 'ARPlaneSelectorTest', scene: require('./js/AR/release_test/ARPlaneSelectorTest') },
  { name: 'ARPlaneTest', scene: require('./js/AR/release_test/ARPlaneTest') },
  { name: 'ARPlaneManualTest', scene: require('./js/AR/release_test/ARPlaneManualTest') },
  { name: 'ARHitTest', scene: require('./js/AR/release_test/ARHitTest') },
  { name: 'ARHitTestCamera', scene: require('./js/AR/release_test/ARHitTestCamera') },
  { name: 'ARViroPortalTest', scene: require('./js/AR/release_test/ViroPortalTestAR') },
  { name: "ARViroPostProcessTest", scene: require('./js/AR/release_test/ViroPostProcessTestAR') },
  { name: 'ARImageMarkerTest', scene: require('./js/AR/release_test/ARImageMarkerTest') },
  { name: 'ARObjectMarkerTest', scene: require('./js/AR/release_test/ARObjectMarkerTest') },
  { name: 'ARPbrTest', scene: require('./js/AR/release_test/ViroPbrTestAR') },
  { name: 'ARPbrTestSurfacesAR', scene: require('./js/AR/release_test/ViroPbrTestSurfacesAR') },
  { name: 'ARShadowTest', scene: require('./js/AR/release_test/ARShadowTest') },
  { name: 'ARPolygonPlaneTest', scene: require('./js/AR/release_test/ARPolygonPlaneTest') },
  { name: 'ARCameraHUDTest', scene: require('./js/AR/release_test/ARCameraHUDTest') },
  { name: 'ARImageTrackingTest', scene: require('./js/AR/release_test/ARImageTrackingTest') },
];
var VRTests = [
  { name: 'SkyBoxTest', scene: require('./js/release_test/ViroSkyBoxTest') },
  { name: '360ImageTest', scene: require('./js/release_test/Viro360ImageTest') },
  { name: '360VideoTest', scene: require('./js/release_test/Viro360VideoTest') },
  { name: 'VideoTest', scene: require('./js/release_test/ViroVideoTest') },
  { name: 'AnimatedComponentTest', scene: require('./js/release_test/ViroAnimatedComponentTest') },
  { name: 'BoxTest', scene: require('./js/release_test/ViroBoxTest') },
  { name: 'SphereTest', scene: require('./js/release_test/ViroSphereTest') },
  { name: 'ButtonTest', scene: require('./js/release_test/ViroButtonTest') },
  { name: 'QuadFlexViewTest', scene: require('./js/release_test/ViroQuadFlexViewTest') },
  { name: 'ImageTest', scene: require('./js/release_test/ViroImageTest') },
  { name: 'TextTest', scene: require('./js/release_test/ViroTextTest') },
  { name: '3DObjectTest', scene: require('./js/release_test/Viro3DObjectTest') },
  { name: 'CameraTest', scene: require('./js/release_test/ViroCameraTest') },
  { name: 'SceneTest', scene: require('./js/release_test/SceneTest1') },
  { name: 'AnimationTest', scene: require('./js/release_test/ViroAnimationTest') },
  { name: 'SoundTest', scene: require('./js/release_test/ViroSoundTest') },
  { name: 'LightTest', scene: require('./js/release_test/ViroLightTest') },
  { name: 'GroupTestBasicProps', scene: require('./js/release_test/GroupTestBasicProps') },
  { name: 'GroupTestBasicAnimation', scene: require('./js/release_test/GroupTestBasicAnimation') },
  { name: 'GroupTestBasicBillboard', scene: require('./js/release_test/GroupTestBasicBillboard') },
  { name: 'GroupTestMaterials', scene: require('./js/release_test/GroupTestMaterials') },
  { name: 'GroupTestBasicEvents', scene: require('./js/release_test/GroupTestBasicEvents') },
  { name: 'GroupTestDragEvents', scene: require('./js/release_test/GroupTestDragEvents') },
  { name: 'PhysicsGroupTest', scene: require('./js/release_test/PhysicsGroupTest') },
  { name: 'PhysicsForcesTest', scene: require('./js/release_test/PhysicsForcesTest') },
  { name: 'PhysicsCollisionTest', scene: require('./js/release_test/PhysicsCollisionTest') },
  { name: 'PhysicsPropertiesTest', scene: require('./js/release_test/PhysicsPropertiesTest') },
  { name: 'PhysicsScaleTest', scene: require('./js/release_test/PhysicsScaleTest') },
  { name: 'ParticleTest', scene: require('./js/release_test/ViroParticleTest') },
  { name: 'ShadowGroupTest', scene: require('./js/release_test/ViroShadowGroupTest') },
  { name: 'ShadowTest', scene: require('./js/release_test/ViroShadowTest') },
  { name: 'PortalTestVR', scene: require('./js/release_test/ViroPortalTestVR') },
  { name: 'FlexViewLayoutTest', scene: require('./js/release_test/ViroFlexViewLayoutTest') },
  { name: 'AnimatedImageTest', scene: require('./js/release_test/ViroAnimatedImageTest') },
  { name: 'PolylineTest', scene: require('./js/release_test/ViroPolylineTest') },
  { name: 'PbrTest', scene: require('./js/release_test/ViroPbrTest') },
  { name: 'BlendingTest', scene: require('./js/release_test/ViroBlendingTest') },
  { name: 'InternationalTextTest', scene: require('./js/release_test/ViroInternationalTextTest') },
  { name: 'FontTest', scene: require('./js/release_test/ViroFontTextTest') },
  { name: 'MaterialVideoTest', scene: require('./js/release_test/ViroMaterialVideoTest') },
  { name: 'GLTFTest', scene: require('./js/release_test/ViroGLTFTest') },
  { name: 'HierarchyTest', scene: require('./js/release_test/ViroHierarchyTest') },
  { name: 'CameraHUDTest', scene: require('./js/release_test/ViroCameraHUDTest') },
  { name: 'ChromaKeyTest', scene: require('./js/release_test/ViroChromaKeyTest') },
  { name: 'PolygonTest', scene: require('./js/release_test/ViroPolygonTest') },
  { name: 'TextSpecialTest', scene: require('./js/release_test/ViroTextSpecialTest') },
  { name: 'ColorMaskTest', scene: require('./js/release_test/ViroColorMaskTest') },
  { name: 'GeometryTest', scene: require('./js/release_test/ViroGeometryTest') },
];
var SceneTests = [
  { name: 'GroupTestBasicEvents', scene: require('./js/release_test/GroupTestBasicEvents')},
  { name: 'GroupTestDragEvents', scene:  require('./js/release_test/GroupTestDragEvents')},
  { name: 'VideoTest', scene: require('./js/release_test/ViroVideoTest')},
  { name: 'SoundTest', scene: require('./js/release_test/ViroSoundTest')},
];


export default class ViroExperienceSelector extends Component {

GetSectionListItem = (section, item) => {
  //Function for click on an item
  if (section.title == "AR Tests") {
    this.setState({navigatorType : AR_NAVIGATOR_TYPE,
                   initialScene:item,
    });
  } else if (section.title == "VR Tests") {
    this.setState({navigatorType : VR_NAVIGATOR_TYPE,
                  initialScene: item,
    });
  } else if (section.title == "Scene Tests"){
    this.setState({navigatorType : SCENE_NAVIGATOR_TYPE,
                   initialScene: item,
    });
  }
};
FlatListItemSeparator = () => {
  return (
    //Item Separator
    <View
      style={{ height: 0.5, width: '100%', backgroundColor: '#C8C8C8' }}
    />
  );
};


  constructor() {
    super();

    var sharedProps = {
      apiKey:"7EEDCB99-2C3B-4681-AE17-17BC165BF792",
    }
    console.log("Config.FLAVOR - " + Config.VR_PLATFORM);
    this.state = {
      navigatorType : Config.VR_PLATFORM == "OVR_MOBILE" ? VR_NAVIGATOR_TYPE : UNSET,
      sharedProps : sharedProps,
      vrMode : Config.FLAVOR == "ovr" ? true : UNSET,
      worldAlignment : UNSET,
      autofocus : true,
      videoQualityHigh : true,
      initial3DScene : UNSET,
      numberOfTrackedImages: 0,
      initialScene: null,
    }
    this._getSelectionButtons = this._getSelectionButtons.bind(this);
    this._getARNavigator = this._getARNavigator.bind(this);
    this._getVRNavigator = this._getVRNavigator.bind(this);
    this._get3DSceneNavigator = this._get3DSceneNavigator.bind(this);
    this._getButtonPress = this._getButtonPress.bind(this);
    this._toggleAutofocus = this._toggleAutofocus.bind(this);
    this._toggleVideoQuality = this._toggleVideoQuality.bind(this);
    this._toggleNumberOfTrackedImages = this._toggleNumberOfTrackedImages.bind(this);
    this._handleARSupported = this._handleARSupported.bind(this);
    this._handleARNotSupported = this._handleARNotSupported.bind(this);
    this._getInitialSelectionButtons = this._getInitialSelectionButtons.bind(this);
    this._getSceneSelectionButtons = this._getSceneSelectionButtons.bind(this);
  }

  componentDidMount() {
    isARSupportedOnDevice(this._handleARNotSupported,this._handleARSupported);
  }

  _handleARSupported() {
    console.log("AR supported");
  }
  _handleARNotSupported(result) {
    console.log("AR not supported: " + result);
  }
  render() {
    if (this.state.navigatorType == UNSET) {
      return this._getSceneSelectionButtons();
    } else if (this.state.navigatorType == VR_NAVIGATOR_TYPE || this.state.navigatorType == VR_DEPRECATED_TYPE ) {
      if (this.state.vrMode == UNSET) {
        return this._getSelectionButtons();
      } else {
        return this._getVRNavigator();
      }
    } else if (this.state.navigatorType == AR_NAVIGATOR_TYPE) {
        return this._getARNavigator();
    } else if (this.state.navigatorType == SCENE_NAVIGATOR_TYPE ) {
      //if (this.state.initial3DScene == UNSET) {
      //  return this._getInitialSelectionButtons();
      // } else {
      return this._get3DSceneNavigator();
      // }
    }
  }
    _getSceneSelectionButtons() {
    return (
      <SafeAreaView style={localStyles.outer} >
          <SectionList
            ItemSeparatorComponent={this.FlatListItemSeparator}
            sections={[
              { title: 'AR Tests', data: ARTests },
              { title: 'VR Tests', data: VRTests },
              { title: 'Scene Tests', data: SceneTests },
            ]}
            renderSectionHeader={({ section }) => (
              <Text style={localStyles.sectionHeaderStyle}> {section.title} </Text>
            )}
            renderItem={({ item, section }) => (
              // Single Comes here which will be repeatative for the FlatListItems
              <Text
                style={localStyles.sectionListItemStyle}
                //Item Separator View
                onPress={this.GetSectionListItem.bind(
                  this, section, item.scene
                )}>
                {item.name}
              </Text>
            )}
            keyExtractor={(item, index) => index}
          />
      </SafeAreaView>
    );
  }
  _getInitialSelectionButtons() {
    return (
      <SafeAreaView style={localStyles.outer} >
        <View style={localStyles.inner} >

          <Text style={localStyles.titleText}>
            {this._getTitleText()}
          </Text>

          <TouchableHighlight style={this.state.navigatorType == UNSET ? localStyles.buttons : localStyles.vrModeButtons}
            onPress={()=>{this.setState({initial3DScene : require('./js/release_test/GroupTestBasicEvents')})}}
            underlayColor={'#68a0ff'} >

            <Text style={localStyles.buttonText}>
              {this._getButtonText(1)}
            </Text>
          </TouchableHighlight>

          <TouchableHighlight style={this.state.navigatorType == UNSET ? localStyles.buttons : localStyles.vrModeButtons}
            onPress={()=>{this.setState({initial3DScene : require('./js/release_test/GroupTestDragEvents')})}}
            underlayColor={'#68a0ff'} >

            <Text style={localStyles.buttonText}>
              {this._getButtonText(2)}
            </Text>
          </TouchableHighlight>

          <TouchableHighlight style={this.state.navigatorType == UNSET ? localStyles.buttons : localStyles.vrModeButtons}
            onPress={()=>{this.setState({initial3DScene : require('./js/release_test/ViroVideoTest')})}}
            underlayColor={'#68a0ff'} >

            <Text style={localStyles.buttonText}>
              {this._getButtonText(3)}
            </Text>
          </TouchableHighlight>

          <TouchableHighlight style={this.state.navigatorType == UNSET ? localStyles.buttons : localStyles.vrModeButtons}
            onPress={()=>{this.setState({initial3DScene : require('./js/release_test/ViroSoundTest')})}}
            underlayColor={'#68a0ff'} >

            <Text style={localStyles.buttonText}>
              {this._getButtonText(4)}
            </Text>
          </TouchableHighlight>
        </View>
      </SafeAreaView>
    );
  }
  _getSelectionButtons() {
    return (
      <View style={localStyles.outer} >
        <View style={localStyles.inner} >

          <Text style={localStyles.titleText}>
            {this._getTitleText()}
          </Text>

          <TouchableHighlight style={this.state.navigatorType == UNSET ? localStyles.buttons : localStyles.vrModeButtons}
            onPress={this._getButtonPress(1)}
            underlayColor={'#68a0ff'} >

            <Text style={localStyles.buttonText}>
              {this._getButtonText(1)}
            </Text>
          </TouchableHighlight>

          <TouchableHighlight style={this.state.navigatorType == UNSET ? localStyles.buttons : localStyles.vrModeButtons}
            onPress={this._getButtonPress(2)}
            underlayColor={'#68a0ff'} >

            <Text style={localStyles.buttonText}>
              {this._getButtonText(2)}
            </Text>
          </TouchableHighlight>
        </View>
      </View>
    );
  }
  _getTitleText() {
    if (this.state.navigatorType == UNSET) {
      return "Choose your desired experience:"
    } else {
      if (this.state.navigatorType == VR_NAVIGATOR_TYPE) {
        return "VR Mode or 360?"
      } else if (this.state.navigatorType == SCENE_NAVIGATOR_TYPE) {
        return "Scene View Spot Check Options:"
      } else {
        return "World Alignment? (No effect on Android)"
      }

    }
  }
  _getButtonText(buttonNum) {
    if (this.state.navigatorType == UNSET) {
      switch (buttonNum) {
        case 1:
         return AR_NAVIGATOR_TYPE;
        case 2:
          return VR_NAVIGATOR_TYPE;
        case 3:
          return SCENE_NAVIGATOR_TYPE;
        case 4:
          return VR_DEPRECATED_TYPE;
      }
    } else {
      if (this.state.navigatorType == VR_NAVIGATOR_TYPE || this.state.navigatorType == VR_DEPRECATED_TYPE) {
        return (buttonNum == 1 ? "VR" : "360")
      } else if (this.state.navigatorType == SCENE_NAVIGATOR_TYPE) {
        switch (buttonNum) {
          case 1:
            return "Basic Events";
          case 2:
            return "Drag Events";
          case 3:
            return "Videos";
          case 4:
            return "Sounds";
        }
      }
    }
  }
  _getARNavigator() {
    return (
      <View style={localStyles.viroContainer} >
        <ViroARSceneNavigator
          {...this.state.sharedProps}
          initialScene={{scene: this.state.initialScene}}
          multisamplingEnabled={false}
          numberOfTrackedImages={this.state.numberOfTrackedImages}
          worldAlignment={"Gravity"}
          videoQuality={this.state.videoQualityHigh ? "High" : "Low"}
          autofocus={this.state.autofocus}
          viroAppProps={{ toggleAutofocus : this._toggleAutofocus,
                          toggleVideoQuality : this._toggleVideoQuality,
                          toggleNumberOfTrackedImages: this._toggleNumberOfTrackedImages,
                          onExitViro:()=>{this.setState({navigatorType : UNSET, vrMode : UNSET})}}} />

        <View style={localStyles.exitButtonContainer}>
          <TouchableHighlight style={localStyles.exitButton}
            onPress={()=>{this.setState({navigatorType : UNSET, vrMode : UNSET, worldAlignment : UNSET})}}
            underlayColor={'#00000000'} >
            <Text style={localStyles.buttonText}>Exit</Text>
          </TouchableHighlight>
        </View>
      </View>
    );
  }

  _getVRNavigator() {
    return (
      <View style={localStyles.viroContainer} >
        {renderIf(this.state.navigatorType == VR_NAVIGATOR_TYPE,
          <ViroVRSceneNavigator {...this.state.sharedProps}
            initialScene={{scene: this.state.initialScene}}
            vrModeEnabled={this.state.vrMode}
            onExitViro={()=>{this.setState({navigatorType : UNSET, vrMode : UNSET})}}/>)}
        {renderIf(this.state.navigatorType == VR_DEPRECATED_TYPE,
          <ViroSceneNavigator {...this.state.sharedProps}
            initialScene={{scene: this.state.initialScene}}
            vrModeEnabled={this.state.vrMode}
            onExitViro={()=>{this.setState({navigatorType : UNSET, vrMode : UNSET})}}/>)}

        <View style={localStyles.exitButtonContainer}>
          <TouchableHighlight style={localStyles.exitButton}
            onPress={()=>{this.setState({navigatorType : UNSET, vrMode : UNSET})}}
            underlayColor={'#00000000'} >
            <Text style={localStyles.buttonText}>Exit</Text>
          </TouchableHighlight>
        </View>
      </View>
    );
  }

  _get3DSceneNavigator() {
    return (
      <View style={localStyles.viroContainer} >
        <Viro3DSceneNavigator
          {...this.state.sharedProps}
          initialScene={{scene: this.state.initialScene}}
          onExitViro={()=>{this.setState({navigatorType : UNSET, vrMode : UNSET, initial3DScene: UNSET})}}/>

        <View style={localStyles.exitButtonContainer}>
          <TouchableHighlight style={localStyles.exitButton}
            onPress={()=>{this.setState({navigatorType : UNSET, vrMode : UNSET, worldAlignment : UNSET, initial3DScene: UNSET})}}
            underlayColor={'#00000000'} >
            <Text style={localStyles.buttonText}>Exit</Text>
          </TouchableHighlight>
        </View>
      </View>
    );
  }

  _getButtonPress(buttonNum) {
    if (buttonNum == 1) {
      if (this.state.navigatorType == UNSET) {
        return ()=>{this.setState({navigatorType : AR_NAVIGATOR_TYPE})}
      } else {
        // It doesn't matter that we're changing 2 states because AR uses one and VR uses the other
        // and both get reset upon "exit".
        return ()=>{this.setState({vrMode : true, worldAlignment : "Gravity"})}
      }
    } else if (buttonNum == 2) {
      if (this.state.navigatorType == UNSET) {
        return ()=>{this.setState({navigatorType : VR_NAVIGATOR_TYPE})}
      } else {
        return ()=>{this.setState({vrMode : false, worldAlignment : "GravityAndHeading"})}
      }
    } else if (buttonNum == 3) {
      if (this.state.navigatorType == UNSET) {
        return ()=>{this.setState({navigatorType : SCENE_NAVIGATOR_TYPE})}
      }
    } else if (buttonNum == 4) {
      if (this.state.navigatorType == UNSET) {
        return ()=>{this.setState({navigatorType : VR_DEPRECATED_TYPE})}
      } else {
        return ()=>{this.setState({vrMode : false, worldAlignment : "GravityAndHeading"})}
      }
    }

    return () => {
      this.setState({
        navigatorType : navigatorType
      })
    }
  }

  _toggleNumberOfTrackedImages() {
    this.setState({
      numberOfTrackedImages: this.state.numberOfTrackedImages == 0 ? 1: 0,
    });
  }

  _toggleAutofocus() {
    this.setState({
      autofocus : !this.state.autofocus
    });
  }

  _toggleVideoQuality() {
    this.setState({
      videoQualityHigh : !this.state.videoQualityHigh
    });
  }
}

var localStyles = StyleSheet.create({
  viroContainer : {
    flex : 1,
    backgroundColor: "black",
  },
  outer : {
    flex : 1,
    flexDirection: 'row',
    alignItems:'center',
    backgroundColor: "black",
  },
  inner: {
    flex : 1,
    flexDirection: 'column',
    alignItems:'center',
    backgroundColor: "black",
  },
  titleText: {
    paddingTop: 30,
    paddingBottom: 20,
    color:'#fff',
    textAlign:'center',
    fontSize : 25
  },
  buttonText: {
    color:'#fff',
    textAlign:'center',
    fontSize : 20
  },
  buttons : {
    height: 80,
    width: 150,
    paddingTop:20,
    paddingBottom:20,
    marginTop: 10,
    marginBottom: 10,
    backgroundColor:'#68a0cf',
    borderRadius: 10,
    borderWidth: 1,
    borderColor: '#fff',
  },
  vrModeButtons : {
    height: 80,
    width: 150,
    paddingTop:20,
    paddingBottom:20,
    marginTop: 10,
    marginBottom: 10,
    backgroundColor:'#1111aa',
    borderRadius: 10,
    borderWidth: 1,
    borderColor: '#fff',
  },
  exitButtonContainer : {
    position: 'absolute',
    bottom: 20,
    alignItems: 'center',
    alignSelf : 'center'
  },
  exitButton : {
    height: 50,
    width: 100,
    paddingTop:10,
    paddingBottom:10,
    marginTop: 10,
    marginBottom: 10,
    backgroundColor:'#68a0cf',
    borderRadius: 10,
    borderWidth: 1,
    borderColor: '#fff',
  },
  sectionHeaderStyle: {
    backgroundColor: '#6699CC',
    fontSize: 20,
    padding: 5,
    color: '#fff',
  },
  sectionListItemStyle: {
    fontSize: 15,
    padding: 15,
    color: '#000',
    backgroundColor: "#FFF",
  },
});

AppRegistry.registerComponent('ViroSample', () => ViroExperienceSelector);

module.exports = ViroExperienceSelector;
