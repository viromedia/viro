/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 */
'use strict';

import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

import {
  ViroScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroImage,
  ViroVideo,
  ViroFlexView,
  ViroUtils,
  ViroText,
  ViroQuad,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
  ViroButton,
  ViroSpinner,
  ViroOmniLight,
  ViroAnimations,
  ViroDirectionalLight
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var UriImage = {uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/360_horseshoe.jpg"};
var LocalImage = require("./res/360_park.jpg");

var Uri360Video = {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf.mp4"};
var Local360Video = require("./res/360Asteroids.mp4");

var LocalButtonImage = require("./res/icon_live.jpg");

var ReleaseMenu = require("./ReleaseMenu.js");

var TOGGLE_PLAY ="Toggle Play";
var TOGGLE_PAUSE ="Toggle Pause";


var GroupTestBasicBillboard = createReactClass({
  getInitialState() {
    return {
        transformFlag: 1,
        transformBehaviors:["billboard"]
    };
  },

  componentWillMount() {
    console.log("componentWillMount ran!");
    ViroAnimations.registerAnimations({
        testLoopRotate:{properties:{rotateZ:"+45"}, duration:2000, easing:"EaseInEaseOut"},

        moveRight:{properties:{positionX:"+=9.5"}, duration: 2000, easing:"Linear", delay:2000},
        moveLeft:{properties:{positionX:" -=9.5"}, duration: 2000, easing:"Linear"},
        moveUp:{properties:{positionY:"+=9.5"}, duration: 2000, easing:"Linear", delay:2000},
        moveDown:{properties:{positionY:"-=9.5"}, duration: 2000, easing:"Linear"},
        moveBack:{properties:{positionY:"-=9.5", positionZ:"-=9.5"}, duration: 2000, easing:"Linear", delay:2000},
        moveForth:{properties:{positionY:"+=9.5", positionZ:"+=9.5"}, duration: 2000, easing:"Linear"},
        testLoopMove:[
            ["moveRight", "moveLeft", "moveUp", "moveDown", "moveBack", "moveForth"]
        ],
    });
  },

  render: function() {
    var newTransformBehaviors;
    var transformText;
    if (this.state.transformFlag == 0){
        newTransformBehaviors=[];
        transformText = "None";
    } else if (this.state.transformFlag == 1){
        newTransformBehaviors=["billboardX"];
        transformText = "billboardX";
    } else if (this.state.transformFlag == 2){
        newTransformBehaviors=["billboardY"];
        transformText = "billboardY";
    } else if (this.state.transformFlag == 3){
              newTransformBehaviors=["billboardX", "billboardY"];
              transformText = "billboardX + billboardY";
    }else {
        newTransformBehaviors=["billboard"];
        transformText = "billboard";
    }

    return (
                <ViroScene>
                <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

                <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

                <ViroNode position={[0.8, 0, -3.5]}  animation={{name:"testLoopMove",
                              loop:true,
                              run:true }} >


                    <Viro3DObject source={require('./res/xwing.obj')}
                             scale={[.2 , .2  , .2]}
                              position={[-2 , 1, 0]}
                              materials={["heart"]}
                              type="OBJ"
                              transformBehaviors={newTransformBehaviors}/>

                  <ViroBox
                      position={[-1 , 1, 0]}
                      scale={[0.4 , 0.4 , 0.4]}
                      materials={["redColor","blue","redColor","blue","redColor","blue"]}
                      height={1}
                      width={1}
                      length={1}
                      transformBehaviors={newTransformBehaviors}/>

                  <ViroButton
                      transformBehaviors={newTransformBehaviors}
                      position={[0, 1, 0]}
                      scale={[0.08, 0.08, 0.1]}
                      source={LocalButtonImage}
                      hoverSource={LocalButtonImage}
                      clickSource={LocalButtonImage}
                      />

                  <ViroFlexView
                  transformBehaviors={newTransformBehaviors}
                      position={[1, 1, 0]}
                      scale={[0.2, 0.2, 0.1]}
                      materials={["redColor"]}
                      width={3}
                      height={2} />
                  <ViroImage
                  transformBehaviors={newTransformBehaviors}
                      width={1} height={1}
                      format="RGBA8" mipmap={true}
                      position={[-2, 0, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      source={{uri: "https://upload.wikimedia.org/wikipedia/commons/7/74/Earth_poster_large.jpg"}}
                      />

                  <ViroNode
                  transformBehaviors={newTransformBehaviors}
                    position={[-1, 0, 0]}
                    scale={[0.5, 0.5, 0.1]}
                    rotation={[0,0,0]}>
                    <ViroText
                        style={styles.baseTextTwo}
                        text="This is a text in a ViroNode" />
                  </ViroNode>

                  <ViroSphere
                  transformBehaviors={newTransformBehaviors}
                      position={[0, 0, 0]}
                      scale={[0.3, 0.3, 0.3]}
                      widthSegmentCount={5}
                      heightSegmentCount={5}
                      radius={1}
                      materials={["redColor"]}
                      />

                  <ViroSpinner
                  transformBehaviors={newTransformBehaviors}
                      position={[0 , -1,0]}
                      scale={[0.3, 0.3, 0.1]}/>

                  <ViroQuad
                  transformBehaviors={newTransformBehaviors}
                      position={[-2, -1, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      materials={["redColor"]}
                      width={1}
                      height={1}/>

                  <ViroText
                  transformBehaviors={newTransformBehaviors}
                      position={[-1, -1, 0]}
                      scale={[0.5 , 0.5, 0.1]}
                      style={styles.baseTextTwo}
                      text="This is a Viro Text"/>

                  <ViroVideo
                  transformBehaviors={newTransformBehaviors}
                      position={[0 , -1,0]}
                      scale={[0.1, 0.1, 0.1]}
                      height={4} width={4}
                      source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}} />

                </ViroNode>

                <ViroText style={styles.baseTextTwo} position={[0.8 , -2, -3.5]} width={2} height ={2} fontFamily={'Arial'}
                      text={"Toggle billboard: " + transformText} textLineBreakMode='Justify' onClick={this._toggleTransform}/>

                   <ViroOmniLight
                                     position={[0, 0, 0]}
                                     color={"#ffffff"}
                                     attenuationStartDistance={30}
                                     attenuationEndDistance={40}/>

            </ViroScene>

    );
  },
  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./GroupTestMaterials')});
  },

  _toggleTransform(){
    var newtransformflag = this.state.transformFlag + 1;
    if (newtransformflag > 4){
        newtransformflag = 0;
    }
     this.setState({
            transformFlag:newtransformflag
           });
  }


});

var styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  elementText: {
    fontFamily: 'HelveticaNeue-Medium',
    fontSize: 30,
    color: '#ffffff',
    textAlign: 'center',
  },
  baseTextTwo: {
      fontFamily: 'Arial',
      fontSize: 20,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});



ViroMaterials.createMaterials({
  redColor: {
  fresnelExponent: .5,
   shininess: 2.0,
    diffuseColor: "#ff0000"
  },
  blue: {
      shininess: 2.0,
      lightingModel: "Lambert",
      diffuseColor: "#0000ff"
    },
  heart: {
      lightingModel: "Phong",
      diffuseTexture: require('../res/heart_d.jpg'),
    },
  grey: {
    shininess : 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('./res/grey.jpg'),
  },
 });







module.exports = GroupTestBasicBillboard;
