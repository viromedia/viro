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
  ViroSceneNavigator,
  ViroScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroCamera,
  ViroAmbientLight,
  ViroOmniLight,
  ViroSpotLight,
  ViroDirectionalLight,
  ViroImage,
  ViroVideo,
  Viro360Image,
  Viro360Video,
  ViroFlexView,
  ViroUtils,
  ViroText,
  ViroAnimations,
  ViroQuad,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;
var ReleaseMenu = require("./ReleaseMenu.js");


var Viro3DObjectTest = createReactClass({
  getInitialState() {
    return {
      runAnimation1:false,
      runAnimation2:false,
      runAnimation3:false,
      setVisibility:true,
      runAnimation4:false,
    };
  },

  render: function() {
    return (
     <ViroScene>
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
      <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

      <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]}
                 onClick={this._showNext} />


         <Viro3DObject ref={(obj)=>{this.destroyer = obj}}
                       source={require('./res/destroyer.obj')}
                       position={[0, 5, 0]}
                       materials={["grey"]}
                       rotation={[0, 45, 0]}
                       scale={[.1, .1, .1]}
                       type="OBJ"
                       onHover={this._startAnimation1}
                       onLoadEnd={this._getDestroyerBounds}
                       animation={{name:"loopRotate",
                                   run:this.state.runAnimation1 }}
          />


       <Viro3DObject source={{"uri" : "https://s3-us-west-2.amazonaws.com/viro/heart.obj"}}
              scale={[1.8, 1.8, 1.8]}
              position={[-2, 1, 0]}
              type="OBJ"
              materials="heart"/>

      <Viro3DObject ref={(obj)=>{this.ball = obj}}
                    source={require('./res/object_basketball.vrx')}
                    position={[0, 0, -2.0]}
                    rotation={[0, 0, 0]}
                    resources={[require('./res/object_basketball_diffuse.png'),
                                require('./res/object_basketball_normal.png'),
                                require('./res/object_basketball_specular.png')]}
                    type="VRX"
                    onLoadEnd={this._getBallBounds} />

         <Viro3DObject source={require('./res/xwing.obj')}
                       resources={[require("./res/star-wars-x-wing.mtl")]}
                       position={[2, -2, -5]}
                       materials={["grey"]}
                       rotation={[0,0,0]}
                       scale={[.3,.3,.3]}
                       type="OBJ"
                       onClick={this._startAnimation2}
                       onLoadStart={this._startAnimation3}
                       animation={{name:"loopRotate",
                                   run:this.state.runAnimation2 }}
         />

       <ViroAnimatedComponent animation="loopRotate" run={this.state.runAnimation4} >
         <Viro3DObject source={require('./res/object_star_anim.vrx')}
                       resources={[require("./res/object_star_diffuse.png"),
                                   require("./res/object_star_specular.png")]}
                       position={[-2, -2, -5]}
                       type="VRX"
                       rotation={[0,0,0]}
                       scale={[1,1,1]}
                       animation={{name:"02_spin", delay:0, loop:true, run:true}}
         />
       </ViroAnimatedComponent>

        <Viro3DObject ref={(obj)=>{this.earth = obj}}
                      source={require('./res/earth_obj.obj')}
                      resources={[require('./res/earth_jpg.jpg'),
                                  require('./res/earth_normal.jpg')]}
                      position={[2.0, 0.0, 5.0]}
                      scale={[0.01, 0.01, 0.01]}
                      materials={["earth"]}
                      type="OBJ"
                      onLoadEnd={this._getEarthBounds}
        />

        <ViroText position={[.7,0,-2]} text={this._getBallBoundingBoxText()}
          style={styles.elementText} onClick={this._startStopRecording} transformBehaviors={["billboard"]}/>

        <ViroText position={[-.7,0,-2]} text={this._getEarthBoundingBoxText()}
          style={styles.elementText} onClick={this._startStopRecording} transformBehaviors={["billboard"]}/>

       <ViroImage source={require('./res/icon_left_w.png')} position={[-2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showPrevious} />
       <ViroText text="Viro3DObject" position={[0, -5, -3]} transformBehaviors={["billboard"]} />
       <ViroImage source={require('./res/icon_right_w.png')} position={[2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showNext} />

     </ViroScene>

    );
  },

  _getBallBoundingBoxText() {
    if (this.state.boundingBox == undefined) {
      return "Loading basketball..."
    } else {
      let boundingBox = this.state.boundingBox;
      return "Basketball Bounds: " + boundingBox.minX + ", " + boundingBox.maxX +", " + boundingBox.minY + ", " + boundingBox.maxY + ", " + boundingBox.minZ + ", " + boundingBox.maxZ;
    }
  },

  _getEarthBoundingBoxText() {
    if (this.state.earthBoundingBox == undefined) {
      return "Loading earth..."
    } else {
      let boundingBox = this.state.earthBoundingBox;
      return "Earth bounds: " + boundingBox.minX.toFixed(2) + ", " + boundingBox.maxX.toFixed(2) + ", " + boundingBox.minY.toFixed(2) + ", " + boundingBox.maxY.toFixed(2) + ", " + boundingBox.minZ.toFixed(2) + ", " + boundingBox.maxZ.toFixed(2);
    }
  },

  _getBallBounds() {
    this.ball.getBoundingBoxAsync().then((retMap)=>{
      let boundingBox = retMap.boundingBox;
      console.log("3dobjtest ball boundingBox is: " + boundingBox.minX + ", " + boundingBox.maxX +", " + boundingBox.minY + ", " + boundingBox.maxY + ", " + boundingBox.minZ + ", " + boundingBox.maxZ);
      this.setState({
        boundingBox : retMap.boundingBox
      })
    })
  },

  _getEarthBounds() {
    this.earth.getBoundingBoxAsync().then((retMap)=>{
      let boundingBox = retMap.boundingBox;
      console.log("3dobjtest earth boundingBox is: " + boundingBox.minX + ", " + boundingBox.maxX +", " + boundingBox.minY + ", " + boundingBox.maxY + ", " + boundingBox.minZ + ", " + boundingBox.maxZ);
      this.setState({
        earthBoundingBox : retMap.boundingBox
      })
    })
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./ViroCameraTest')});
  },

  _showPrevious() {
    this.props.sceneNavigator.pop();
  },

  _onAnimStart() {
    console.log("Viro3DObjectTest: _onAnimStart");
  },

  _onAnimEnd() {
    console.log("Viro3DObjectTest: _onAnimEnd");
  },

  _startAnimation1() {
    this.setState({
        runAnimation1:true,
      });
  },

  _startAnimation2() {
    this.setState({
        runAnimation2:true,
      });
  },

  _startAnimation3() {
    this.setState({
        runAnimation3:true,
      });
  },

  _setVisibility() {
    this.setState({
        setVisibility:!this.state.setVisibility,
      });
  },

  _startAnimation4() {
    this.setState({
        runAnimation4:true,
      });
  },

});

ViroAnimations.registerAnimations({
  loopRotate:{properties:{rotateY:"+=2"}, duration:250},
  scaleSphere:{properties:{scaleX:1.1, scaleY:1.1, scaleZ:1.1}, duration:300},
  spinSphere:[
      ["loopRotate"],
      ["scaleSphere"],
  ],
});

var styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  elementText: {
    fontFamily: 'HelveticaNeue-Medium',
    fontSize: 10,
    color: '#ffffff',
    textAlign: 'center',
  },
});

ViroMaterials.createMaterials({
  heart: {
    lightingModel: "Constant",
    diffuseTexture: { "uri" : "https://s3-us-west-2.amazonaws.com/viro/heart_d.jpg" },
  },
  grey: {
    shininess : 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('./res/grey.jpg'),
  },
  earth: {
    lightingModel: "Blinn",
    diffuseTexture: require('./res/earth_jpg.jpg'),
    normalTexture: require('./res/earth_normal.jpg'),
  }
});

module.exports = Viro3DObjectTest;
