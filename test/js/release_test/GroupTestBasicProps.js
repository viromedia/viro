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
  ViroDirectionalLight,
  ViroPolygon
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var UriImage = {uri:"https://s3-us-west-2.amazonaws.com/viro/Explorer/360_horseshoe.jpg"};
var LocalImage = require("./res/360_park.jpg");

var Uri360Video = {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf.mp4"};
var Local360Video = require("./res/360Asteroids.mp4");

var LocalButtonImage = require("./res/icon_live.jpg");

var ReleaseMenu = require("./ReleaseMenu.js");

var TEST_COMPONENT_NUM = 12;

var GroupTestBasicProps = createReactClass({
  getInitialState() {
    var offsetArray = [12];
    var i;
    for ( i = 0; i < TEST_COMPONENT_NUM ; i ++ ){
        var offset ={
            translate: 0,
            rotate: 0,
            scale: 1,
            isVisible:true,
            opacity: 1
        }
        offsetArray[i] = offset;
    }

    return {
        currentInvisibleView:-1,
        offsetData:offsetArray,
        currentToggle:-1,
    };
  },

  render: function() {
    return (
            <ViroScene onClick={this.sceneClick}>
            <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>

            <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />


                <ViroNode position={[0.8 , 0, -3.5]}>
                <Viro3DObject source={require('../res/heart.obj')} opacity={this.state.offsetData[10].opacity}
                         scale={[1.8 * this.state.offsetData[10].scale, 1.8 * this.state.offsetData[10].scale , 1.8]}
                         position={[-3.2 + this.state.offsetData[10].translate, 2.5, -4.5]}
                          rotation={[0,this.state.offsetData[10].rotate,0]}
                          type="OBJ"
                          materials={["heart"]} onClick={this._elementClick(10)} visible={this.state.offsetData[10].isVisible}/>

                  <ViroBox
                      position={[-1 + this.state.offsetData[0].translate, 1, 0]} opacity={this.state.offsetData[0].opacity}
                      scale={[0.4 * this.state.offsetData[0].scale , 0.4 * this.state.offsetData[0].scale, 0.4]}
                      materials={["redColor"]}
                      rotation={[0,this.state.offsetData[0].rotate,0]}
                      height={1}
                      width={1}
                      length={1}
                       onClick={this._elementClick(0)} visible={this.state.offsetData[0].isVisible}/>

                  <ViroButton
                      position={[0 + this.state.offsetData[1].translate, 1, 0]} opacity={this.state.offsetData[1].opacity}
                      scale={[0.4 * this.state.offsetData[1].scale, 0.4 * this.state.offsetData[1].scale, 0.4]}
                      rotation={[0,this.state.offsetData[1].rotate,0]}
                      source={LocalButtonImage}
                      hoverSource={LocalButtonImage}
                      clickSource={LocalButtonImage}
                       onClick={this._elementClick(1)} visible={this.state.offsetData[1].isVisible}/>

                  <ViroFlexView
                      position={[1 + this.state.offsetData[2].translate, 1, 0]} opacity={this.state.offsetData[2].opacity}
                      scale={[0.2* this.state.offsetData[2].scale, 0.2* this.state.offsetData[2].scale, 0.1]}
                      rotation={[0,this.state.offsetData[2].rotate,0]}
                      materials={["redColor"]}
                      width={3}
                      height={2}
                      onClick={this._elementClick(2)} visible={this.state.offsetData[2].isVisible}/>

                  <ViroImage
                      width={1} height={1}
                      format="RGBA8" mipmap={true}
                      position={[-2 + this.state.offsetData[3].translate, 0, 0]} opacity={this.state.offsetData[3].opacity}
                      scale={[0.5* this.state.offsetData[3].scale, 0.5* this.state.offsetData[3].scale, 0.1]}
                      rotation={[0,this.state.offsetData[3].rotate,0]}
                      onClick={this._elementClick(3)}
                      source={{uri: "https://upload.wikimedia.org/wikipedia/commons/7/74/Earth_poster_large.jpg"}}
                      visible={this.state.offsetData[3].isVisible}/>

                  <ViroNode
                    position={[-1 + this.state.offsetData[4].translate, 0, 0]} opacity={this.state.offsetData[4].opacity}
                    scale={[0.5* this.state.offsetData[4].scale, 0.5* this.state.offsetData[4].scale, 0.1]}
                    onClick={this._elementClick(4)}
                    rotation={[0,this.state.offsetData[4].rotate,0]} visible={this.state.offsetData[4].isVisible}>

                    <ViroText
                        style={styles.baseTextTwo}
                        text="This is a text in a ViroNode" />

                  </ViroNode>

                  <ViroSphere
                      position={[0 + this.state.offsetData[5].translate, 0, 0]} opacity={this.state.offsetData[5].opacity}
                      scale={[0.3* this.state.offsetData[5].scale, 0.3* this.state.offsetData[5].scale, 0.3]}
                      rotation={[0,this.state.offsetData[5].rotate,0]}
                      widthSegmentCount={5}
                      heightSegmentCount={5}
                      radius={1}
                      onClick={this._elementClick(5)}
                      materials={["redColor"]} visible={this.state.offsetData[5].isVisible}/>

                  <ViroSpinner
                      position={[1 + this.state.offsetData[6].translate, 0, 0]} opacity={this.state.offsetData[6].opacity}
                      scale={[0.3* this.state.offsetData[6].scale, 0.3* this.state.offsetData[6].scale, 0.1]}
                      rotation={[0,this.state.offsetData[6].rotate,0]}
                      onClick={this._elementClick(6)} visible={this.state.offsetData[6].isVisible}/>

                  <ViroQuad
                      position={[-2 + this.state.offsetData[7].translate, -1, 0]} opacity={this.state.offsetData[7].opacity}
                      scale={[0.5* this.state.offsetData[7].scale, 0.5 * this.state.offsetData[7].scale, 0.1]}
                      rotation={[0,this.state.offsetData[7].rotate,0]}
                      materials={["redColor"]}
                      width={1}
                      onClick={this._elementClick(7)}
                      height={1} visible={this.state.offsetData[7].isVisible}/>


{/*
Note: Re-enable opacity for text once VIRO-885 is fixed
*/}
                  <ViroText
                      position={[-1 + this.state.offsetData[8].translate, -1, 0]}
                       scale={[0.5 * this.state.offsetData[8].scale, 0.5 * this.state.offsetData[8].scale, 0.1]}
                       rotation={[0,this.state.offsetData[8].rotate,0]}
                      style={styles.baseTextTwo}
                      onClick={this._elementClick(8)}
                      opacity={this.state.offsetData[8].opacity}
                      visible={this.state.offsetData[8].isVisible}
                      text="This is a Viro Text"  />

                  <ViroVideo
                      visible={this.state.offsetData[9].isVisible} opacity={this.state.offsetData[9].opacity}
                      position={[0+ this.state.offsetData[9].translate , -1,0]}
                      scale={[0.1 * this.state.offsetData[9].scale, 0.1 * this.state.offsetData[9].scale, 0.1]}
                      rotation={[0,this.state.offsetData[9].rotate,0]}
                      height={4} width={4}
                      onClick={this._elementClick(9)}
                      source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}} />

                  <ViroPolygon
                      vertices={[[-1.0,1.0], [0.0,1.75],  [1.5,1.5] , [2.0, -1.0],[-1.5,-1.5] ]}
                      visible={this.state.offsetData[11].isVisible} opacity={this.state.offsetData[11].opacity}
                      position={[1.0+ this.state.offsetData[11].translate , -1,0]}
                      scale={[0.1 * this.state.offsetData[11].scale * 2.5, 0.1 * this.state.offsetData[11].scale * 2.5, 0.1 * 2.5]}
                      rotation={[0,this.state.offsetData[11].rotate,0]}
                      onClick={this._elementClick(11)}
                      materials={"blue_plane"}/>

                 <ViroText
                      position={[-1.5 , -2, 0]}
                      scale={[1, 1, 0.1]}
                      style={styles.baseTextTwo}
                      text="Toogle Position"
                      onClick={this._togglePosition}/>

                 <ViroText
                       position={[0 , -2, 0]}
                       scale={[1, 1, 0.1]}
                       style={styles.baseTextTwo}
                       text="Toggle Scale"
                       onClick={this._toggleScale}/>

                 <ViroText
                       position={[1.5 , -2, 0]}
                       scale={[1, 1, 0.1]}
                       style={styles.baseTextTwo}
                       text="Toggle Rotation"
                       onClick={this._toggleRotation}/>

                  <ViroText
                       position={[-1.5 , -3, 0]}
                       scale={[1, 1, 0.1]}
                       style={styles.baseTextTwo}
                       text="Toggle Visible"
                       onClick={this._toggleVisibility}/>

                  <ViroText
                       position={[0 , -3, 0]}
                       scale={[1, 1, 0.1]}
                       style={styles.baseTextTwo}
                       text="Toggle Opacity"
                       onClick={this._toggleOpacity}/>
                </ViroNode>
            </ViroScene>

    );
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./GroupTestBasicAnimation')});
  },

  _toggleVisibility(){
     this.setState({
              currentToggle:4,
          });
  },

  _toggleOpacity(){
     this.setState({
              currentToggle:5,
          });
  },

  _elementClick(item){
        return () => {
              var currentOffsetData = this.state.offsetData[item];
              if (this.state.currentToggle == 1){
                currentOffsetData.translate = currentOffsetData.translate == 0.1 ? 0 : 0.1;
              } else if (this.state.currentToggle == 2){
                currentOffsetData.scale = currentOffsetData.scale == 0.5 ? 1 : 0.5;
              } else if (this.state.currentToggle == 3){
                currentOffsetData.rotate = currentOffsetData.rotate == 45 ? 0 : 45;
              } else if (this.state.currentToggle == 4){
                currentOffsetData.isVisible = !currentOffsetData.isVisible;

                // If we are already hiding a view, show it before hiding the next one
                if (!currentOffsetData.isVisible ){
                    if (this.state.currentInvisibleView != -1){
                         var updatedDataArray = this.state.offsetData;
                         var updateToVisibleView = updatedDataArray[this.state.currentInvisibleView];
                         updateToVisibleView.isVisible = true;
                         updatedDataArray[this.state.currentInvisibleView] = updateToVisibleView;

                         this.setState({
                            offsetData:updatedDataArray,
                            currentInvisibleView: item
                          });
                    } else {
                        // If not, then just track this view as the current invisible view
                        this.setState({
                         currentInvisibleView: item
                        });
                    }
                }
              } else if (this.state.currentToggle == 5){
                currentOffsetData.opacity = currentOffsetData.opacity == 0.5 ? 1 : 0.5;
              }  else {
                return;
              }

              var updatedData = this.state.offsetData;
              updatedData[item] = currentOffsetData;

               this.setState({
                offsetData:updatedData,
               });
            }
  },

  _togglePosition(){
      this.setState({
          currentToggle:1,
      });
  },

  _toggleScale(){
      this.setState({
          currentToggle:2,
      });
  },

    _toggleRotation(){
        this.setState({
            currentToggle:3,
        });
    },

    sceneClick(){
        if (this.state.currentInvisibleView == -1){
            return;
        }

        var currentOffsetData = this.state.offsetData[this.state.currentInvisibleView];
        currentOffsetData.isVisible = true;

        var updatedData = this.state.offsetData;
        updatedData[this.state.currentInvisibleView] = currentOffsetData;

        this.setState({
            currentInvisibleView:-1,
            offsetData:updatedData,
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
  sunTexture: {
    diffuseTexture: require("../res/sun_2302.jpg"),
  },
  redColor: {
    diffuseColor: "#ff0000"
  },
  heart: {
      lightingModel: "Constant",
      diffuseTexture: require('../res/heart_d.jpg'),
    },
    blue_plane: {
      lightingModel: "Constant",
      diffuseColor: "#0000ff50"
    },
 });

module.exports = GroupTestBasicProps;
