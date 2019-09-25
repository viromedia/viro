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
  ViroDirectionalLight,
  ViroController,
} from 'react-viro';

var createReactClass = require('create-react-class');

var LocalButtonImage = require("./res/icon_live.jpg");
var ReleaseMenu = require("./ReleaseMenu.js");

var GroupTestBasicPhysics = createReactClass({
  getInitialState() {
    return {
      physicsEnabled:false,
      gravityEnabled:false,
      drawBounds:false,
      toggleDraggable:false,
      reset:false
    };
  },

  togglePhysicsBody(tag){
    return () => {
        console.log("Click numeric tag: " + tag);
        if (tag == 1){
          this.setState({
            physicsEnabled:!this.state.physicsEnabled
          });
        } else if (tag ==2){
          this.setState({
            gravityEnabled:!this.state.gravityEnabled
          });
        } else if (tag ==3){
          this.setState({
            drawBounds:!this.state.drawBounds
          });
        } else if (tag ==4){
          this.setState({
            toggleDraggable:!this.state.toggleDraggable
          });
        }
    }
  },


  onDrag(objectTag){
    return (dragtoPos, source) => {
                          console.log("GroupTest: " + objectTag+ " onDrag dragtoPos" +
                          dragtoPos[0] +","+ dragtoPos[1]+","+ dragtoPos[2]);
    }
  },


  onReset(){
    let that = this;
    this.setState({
      reset:true
    });

    setTimeout(function(){
      that.setState({
        reset:false
      });

    }, 500);
  },

  render: function() {
    if (this.state.reset){
      return (<ViroScene />);
    }

    return (
                <ViroScene physicsWorld={{gravity:[0,-9.81,0],drawBounds:this.state.drawBounds}}>
                <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
                <ViroNode position={[4 , -1, -3]} transformBehaviors={["billboard"]}>
                  <ViroText fontSize={35}  style={styles.centeredText}
                    position={[0,2, 0]} width={4} height ={2} maxLines={3}
                    color={this.state.physicsEnabled ? '#0000ff' : '#ffffff'}
                    text={"Toggle PhysicsBody"} onClick={this.togglePhysicsBody(1)}/>

                  <ViroText fontSize={35}  style={styles.centeredText}
                      position={[0,1, 0]} width={4} height ={2} maxLines={3}
                      color={this.state.gravityEnabled ? '#0000ff' : '#ffffff'}
                      text={"Toggle Gravity"} onClick={this.togglePhysicsBody(2)}/>

                  <ViroText fontSize={35}  style={styles.centeredText}
                      position={[0,0, 0]} width={4} height ={2} maxLines={3}
                      color={this.state.drawBounds ? '#0000ff' : '#ffffff'}
                      text={"Toggle Collision Lines"} onClick={this.togglePhysicsBody(3)}/>

                  <ViroText fontSize={35}  style={styles.centeredText}
                      position={[0,-1, 0]} width={4} height ={2} maxLines={3}
                      color={this.state.toggleDraggable ? '#0000ff' : '#ffffff'}
                      text={"Toggle Interactivity"} onClick={this.togglePhysicsBody(4)}/>

                  <ViroText fontSize={35}  style={styles.centeredText}
                      position={[0,-2, 0]} width={4} height ={2} maxLines={3}
                      text={"Hard Reset."} onClick={this.onReset}/>
                </ViroNode>

                <ViroBox
                    physicsBody={{
                      type:'Static',
                      restitution:0.4
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
                      type="OBJ"
                      onDrag={this.state.toggleDraggable ? this.onDrag("Viro3DObject") : undefined}
                      physicsBody={this.state.physicsEnabled?
                        {type:'Dynamic', mass:1, enabled:true, useGravity:this.state.gravityEnabled}:undefined}
                      />

                  <ViroBox
                      position={[-1, 1, 0]}
                      scale={[0.4, 0.4, 0.4]}
                      materials={["redColor","blue","redColor","blue","redColor","blue"]}
                      height={1}
                      width={1}
                      length={1}
                      onDrag={this.state.toggleDraggable ? this.onDrag("ViroBox") : undefined}
                      physicsBody={this.state.physicsEnabled?
                        {type:'Dynamic', mass:1, enabled:true, useGravity:this.state.gravityEnabled}:undefined}
                      />

                  <ViroButton
                      position={[0, 1, 0]}
                      scale={[0.2, 0.2, 0.1]}
                      source={LocalButtonImage}
                      hoverSource={LocalButtonImage}
                      clickSource={LocalButtonImage}
                      onDrag={this.state.toggleDraggable ? this.onDrag("ViroButton") : undefined}
                      physicsBody={this.state.physicsEnabled?
                        { shape:{type:'Box', params:[0.2,0.2,0.05]}, type:'Dynamic', mass:1, enabled:true, useGravity:this.state.gravityEnabled}:undefined}
                      />

                  <ViroFlexView
                      position={[1, 1, 0]}
                      scale={[0.3, 0.3, 0.1]}
                      materials={["redColor"]}
                      width={1}
                      height={1}
                      onDrag={this.state.toggleDraggable ? this.onDrag("ViroFlexView") : undefined}
                      physicsBody={this.state.physicsEnabled?
                        {type:'Dynamic', mass:1, enabled:true, shape:{type:'Box', params:[0.3,0.3,0.01]},
                        useGravity:this.state.gravityEnabled}:undefined}
                      />

                  <ViroImage
                      width={1} height={1}
                      format="RGBA8" mipmap={true}
                      position={[-2, 0, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      onDrag={this.state.toggleDraggable ? this.onDrag("ViroImage") : undefined}
                      physicsBody={this.state.physicsEnabled?
                        {type:'Dynamic', mass:1, enabled:true, useGravity:this.state.gravityEnabled}:undefined}
                      resizeMode="ScaleToFill"
                      imageClipMode="ClipToBounds"
                      source={{uri: "https://upload.wikimedia.org/wikipedia/commons/7/74/Earth_poster_large.jpg"}}/>

                  <ViroNode
                    position={[-1, 0, 0]}
                    scale={[0.5, 0.5, 0.1]}
                    onDrag={this.state.toggleDraggable ? this.onDrag("ViroNode") : undefined}
                    physicsBody={this.state.physicsEnabled?
                      {type:'Dynamic', mass:1, enabled:true, shape:{type:'box', params:[0.6,0.6,0.2]},
                       useGravity:this.state.gravityEnabled}:undefined}

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
                      onDrag={this.state.toggleDraggable ? this.onDrag("ViroSphere") : undefined}
                      physicsBody={this.state.physicsEnabled?
                        {type:'Dynamic', mass:1, enabled:true, useGravity:this.state.gravityEnabled}:undefined}
                      materials={["redColor"]}
                      />

                  <ViroSpinner
                    onDrag={this.state.toggleDraggable ? this.onDrag("ViroSpinner") : undefined}
                    physicsBody={this.state.physicsEnabled?
                    {type:'Dynamic', mass:1, enabled:true, shape:{type:'Box', params:[0.4,0.4,0.2]}, useGravity:this.state.gravityEnabled}:undefined}

                      position={[1, 0, 0]}
                      scale={[0.3, 0.3, 0.1]}/>

                  <ViroQuad
                    onDrag={this.state.toggleDraggable ? this.onDrag("ViroQuad") : undefined}
                    physicsBody={this.state.physicsEnabled?
                      {type:'Dynamic', mass:1, enabled:true, useGravity:this.state.gravityEnabled}:undefined}
                    position={[-2, -1, 0]}
                      scale={[0.5, 0.5, 0.1]}
                      materials={["redColor"]}
                      width={1}
                      height={1}/>

                  <ViroText
                    onDrag={this.state.toggleDraggable ? this.onDrag("ViroText") : undefined}
                    physicsBody={this.state.physicsEnabled?
                      {type:'Dynamic', mass:1, enabled:true, useGravity:this.state.gravityEnabled}:undefined}
                      position={[-0.5, -1.7, 0]}
                      width={2} height ={2}
                      style={styles.baseTextTwo}
                      text={"This is a Viro Text"}/>

                  <ViroVideo
                    onDrag={this.state.toggleDraggable ? this.onDrag("ViroVideo") : undefined}
                    physicsBody={this.state.physicsEnabled?
                      {type:'Dynamic', mass:1, enabled:true, useGravity:this.state.gravityEnabled}:undefined}
                      position={[0 , -1,0]}
                      scale={[0.1, 0.1, 0.1]}
                      height={4} width={4}
                      rotation={[0,0,-45]}
                      loop={false}
                      source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}} />


                      <ViroNode
                      scale={[0.2, 0.2, 0.2]}
                        position={[3, -1, 0]}
                        onDrag={this.state.toggleDraggable ? this.onDrag("CompoundNode") : undefined}
                        physicsBody={this.state.physicsEnabled?
                          {type:'Dynamic', mass:1, enabled:true, restitution:1, shape:{type:'Compound'},
                           useGravity:this.state.gravityEnabled}:undefined}
                        rotation={[0,0,-90]}>

                        <ViroBox
                          position={[-0.5,0,0]}
                          scale={[1, 1, 1]}
                          materials={'blue'}
                          height={1} width={1} length={1}
                          />
                        <ViroBox
                          position={[0.5,0,0]}
                          scale={[1, 1, 1]}
                          materials={'blue'}
                          height={1} width={1} length={1}
                          />
                        <ViroBox
                            position={[-1.5,0,0]}
                            scale={[1, 1, 1]}
                            materials={'blue'}
                            height={1} width={1} length={1}
                            />
                        <ViroBox
                          position={[0,-0.5,0]}
                          scale={[1, 1, 1]}
                          materials={'blue'}
                          height={1} width={1} length={1}
                          />
                        <ViroBox
                          position={[0,-1.5,0]}
                          scale={[1, 1, 1]}
                          materials={'blue'}
                          height={1} width={1} length={1}
                          />
                          <ViroBox
                            position={[0,-2.5,0]}
                            scale={[1, 1, 1]}
                            materials={'blue'}
                            height={1} width={1} length={1}
                            />

                        </ViroNode>
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
    diffuseColor: "#F0F099",
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
