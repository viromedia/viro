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
  ViroAnimatedComponent,
  ViroSurface,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
} from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;
var ReleaseMenu = require("./ReleaseMenu.js");


var Viro3DObjectTest = React.createClass({

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

       <ViroAnimatedComponent animation="loopRotate" run={this.state.runAnimation1} >
         <Viro3DObject source={require('./res/destroyer.obj')}
                       position={[1, 1, -5]}
                       materials={["grey"]}
                       rotation={[0, 45, 0]}
                       scale={[.1, .1, .1]}
                       onHover={this._startAnimation1}
                       onLoadEnd={this._startAnimation4}
          />
       </ViroAnimatedComponent>

       <Viro3DObject source={require('./res/minion.proto')}
                     position={[0, 0, -12.0]}
                     rotation={[0, 0, 0]}
                     scale={[0.1, 0.1, 0.1]}
       />

       <ViroAnimatedComponent animation="loopRotate" run={this.state.runAnimation2} >
         <Viro3DObject source={require('./res/xwing.obj')}
                       position={[2, -2, -5]}
                       materials={["grey"]}
                       rotation={[0,0,0]}
                       scale={[.3,.3,.3]}
                       onClick={this._startAnimation2}
                       onLoadStart={this._startAnimation3}
         />
       </ViroAnimatedComponent>

       <ViroAnimatedComponent animation="loopRotate" run={this.state.runAnimation4} >
         <Viro3DObject source={require('./res/tantiveIV.obj')}
                       position={[-2, -2, -5]}
                       materials={["grey"]}
                       rotation={[0,0,0]}
                       scale={[.2,.2,.2]}
         />
       </ViroAnimatedComponent>

       <Viro3DObject source={require('./res/male02_obj.obj')}
                     resources={[require('./res/male02.mtl'),
                                 require('./res/01_-_Default1noCulling.JPG'),
                                 require('./res/male-02-1noCulling.JPG'),
                                 require('./res/orig_02_-_Defaul1noCulling.JPG')]}
                     position={[-0.0, -20, -10]}
                     scale={[0.1, 0.1, 0.1]}
                     onLoadStart={this._onLoadObjStart}
                     onLoadEnd={this._onLoadObjEnd}
        />

        <ViroAnimatedComponent animation="loopRotate" run={true} loop={true}>
          <Viro3DObject source={require('./res/earth_obj.obj')}
                        resources={[require('./res/earth_jpg.jpg'),
                                    require('./res/earth_normal.jpg')]}
                        position={[2.0, 0.0, 5.0]}
                        scale={[0.01, 0.01, 0.01]}
                        materials={["earth"]}
           />
         </ViroAnimatedComponent>

       <ViroImage source={require('./res/icon_left_w.png')} position={[-2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showPrevious} />
       <ViroText text="Viro3DObject" position={[0, -5, -3]} transformBehaviors={["billboard"]} />
       <ViroImage source={require('./res/icon_right_w.png')} position={[2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showNext} />

     </ViroScene>

    );
  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./ViroCameraTest')});
  },

  _showPrevious() {
    this.props.sceneNavigator.pop();
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
    fontSize: 30,
    color: '#ffffff',
    textAlign: 'center',
  },
});

ViroMaterials.createMaterials({
  heart: {
    lightingModel: "Constant",
    diffuseTexture: require('../res/heart_d.jpg'),
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
