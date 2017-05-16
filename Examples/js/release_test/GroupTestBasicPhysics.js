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
  ViroAnimatedComponent,
  ViroScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroImage,
  ViroVideo,
  ViroFlexView,
  ViroUtils,
  ViroText,
  ViroSurface,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
  ViroButton,
  ViroSpinner,
  ViroOmniLight,
  ViroAnimations,
  ViroDirectionalLight,
  ViroController,
} from 'react-viro';

var LocalButtonImage = require("./res/icon_live.jpg");
var ReleaseMenu = require("./ReleaseMenu.js");

var GroupTestBasicPhysics = React.createClass({
  getInitialState() {
    return {
      physicsEnabled:false
    };
  },

  togglePhysicsBody(){
    this.setState({
           physicsEnabled:!this.state.physicsEnabled
          });
  },

  render: function() {
    return (
                <ViroScene>
                <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
                <ViroNode position={[2 , 0, -3]} transformBehaviors={["billboard"]}>
                  <ViroText fontSize={20}  style={styles.centeredText}
                    position={[0,0, 0]} width={2} height ={2} maxLines={3}
                    text={"Toggle Physics Body Disabled"} onClick={this.togglePhysicsBody}/>
                </ViroNode>

                <ViroBox
                    physicsBody={{
                      type:'static', shape:{type:'box', params:[30,2,30]}
                    }}
                    position={[0, -6, -8]}
                    scale={[30,1,30]}
                    materials={["box2"]}
                    rotation={[0,0,0]}
                    height={1}
                    width={1}
                    length={1}
                />

                <ViroNode position={[0.8 , 0, -3.5]} >

                  <Viro3DObject source={require('../res/heart.obj')}
                      scale={[1.8, 1.8, 1.8]}
                      position={[-3.2, 2.5, -4.5]}
                      materials={["heart"]}
                      physicsBody={{type:'dynamic', enabled:this.state.physicsEnabled ,mass:1, useGravity:false}}/>

                  <ViroBox
                      position={[-1, 1, 0]}
                      scale={[0.4, 0.4, 0.4]}
                      materials={["redColor","blue","redColor","blue","redColor","blue"]}
                      height={1}
                      width={1}
                      length={1}
                      physicsBody={this.state.physicsEnabled?
                        {type:'dynamic', mass:1, enabled:true, useGravity:this.state.physicsEnabled}:undefined}
                      />
{/*
                  Un-comment once VIRO-1295 is fixed
                  <ViroButton
                      position={[0, 1, 0]}
                      scale={[0.2, 0.2, 0.1]}
                      source={LocalButtonImage}
                      hoverSource={LocalButtonImage}
                      clickSource={LocalButtonImage}
                      />
*/}

                  <ViroFlexView
                      position={[1, 1, 0]}
                      scale={[0.3, 0.3, 0.1]}
                      materials={["redColor"]}
                      width={1}
                      height={1}
                      physicsBody={{
                        type:'dynamic', mass:1, enabled:this.state.physicsEnabled, shape:{type:'box', params:[0.3,0.3,0.01]}
                      }}/>

                  <ViroImage
                      width={1} height={1}
                      format="RGBA8" mipmap={true}
                      position={[-2, 0, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      physicsBody={{
                        type:'dynamic', enabled:this.state.physicsEnabled, mass:1
                      }}
                      resizeMode="scaleToFill"
                      imageClipMode="clipToBounds"
                      source={{uri: "https://upload.wikimedia.org/wikipedia/commons/7/74/Earth_poster_large.jpg"}}/>

                  <ViroNode
                    position={[-1, 0, 0]}
                    scale={[0.5, 0.5, 0.1]}
                    physicsBody={{
                      type:'dynamic', mass:1, enabled:this.state.physicsEnabled, shape:{type:'box', params:[0.6,0.6,0.2]}
                    }}
                    rotation={[0,0,0]}>
                    <ViroText
                        style={styles.baseTextTwo}
                        text="This is a text in a ViroNode" />
                  </ViroNode>

                  <ViroSphere
                      position={[0, 0, 0]}
                      scale={[0.3, 0.3, 0.3]}
                      widthSegmentCount={5}
                      heightSegmentCount={5}
                      radius={1}
                      physicsBody={{type:'dynamic', enabled:this.state.physicsEnabled, mass:1}}
                      materials={["redColor"]}
                      />

                  <ViroSpinner
                      physicsBody={{
                        type:'dynamic', mass:1, enabled:this.state.physicsEnabled, shape:{type:'box', params:[0.4,0.4,0.2]}
                      }}
                      position={[1, 0, 0]}
                      scale={[0.3, 0.3, 0.1]}/>

                  <ViroSurface
                      physicsBody={{type:'dynamic', enabled:this.state.physicsEnabled, mass:1}}
                      position={[-2, -1, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      materials={["redColor"]}
                      width={1}
                      height={1}/>

                  <ViroText
                      physicsBody={{type:'dynamic', enabled:this.state.physicsEnabled, mass:1}}
                      position={[-1, -1, 0]}
                      width={2} height ={2}
                      style={styles.baseTextTwo}
                      text={"This is a Viro Text"}/>

                  <ViroVideo
                      physicsBody={{type:'dynamic', enabled:this.state.physicsEnabled, mass:1}}
                      position={[0 , -1,0]}
                      scale={[0.1, 0.1, 0.1]}
                      height={4} width={4}
                      source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}} />
                </ViroNode>

                <ViroOmniLight
                      position={[0, 0, 0]}
                      color={"#ffffff"}
                      attenuationStartDistance={30}
                      attenuationEndDistance={40}/>
            </ViroScene>

    );
  },
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
    color: '#ffffff',
    flex: 1,
  },
  centeredText: {
       fontFamily: 'Arial',
       color: '#ffffff',
       flex: 1,
  },

});



ViroMaterials.createMaterials({
  redColor: {
    shininess: 2.0,
    diffuseColor: "#ff0000"
  },
  box2: {
    shininess : 2.0,
    diffuseColor: "#00F0FF",
  },
  blue: {
      shininess: 2.0,
      diffuseColor: "#0000ff"
    },
  heart: {
      lightingModel: "Phong",
      diffuseTexture: require('../res/heart_d.jpg'),
    },
 });

module.exports = GroupTestBasicPhysics;
