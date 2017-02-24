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

var Viro360ImageTest = require('./Viro360ImageTest');
var ViroVideoTest = require('./ViroVideoTest');

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
     <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>

     <ViroAnimatedComponent animation="loopRotate"
      run={this.state.runAnimation1} >
     <Viro3DObject source={require('./res/destroyer.obj')}
       position={[1, 1, -5]}
       materials={["grey"]}
       rotation={[0,45,0]}
       scale={[.1,.1,.1]}
       onHover={this._startAnimation1}
       onLoadEnd={this._startAnimation4}
       />
     </ViroAnimatedComponent>

     <Viro3DObject source={{uri:"https://s3-us-west-2.amazonaws.com/viro/heart.obj"}}
            position={[2, -2, -5]}
            materials={["grey"]}
            rotation={[0,0,0]}
            scale={[.3,.3,.3]}
            />

     <ViroAnimatedComponent animation="loopRotate"
      run={this.state.runAnimation2} >
     <Viro3DObject source={require('./res/xwing.obj')}
       position={[2, -2, -5]}
       materials={["grey"]}
       rotation={[0,0,0]}
       scale={[.3,.3,.3]}
       onClick={this._startAnimation2}
       onLoadStart={this._startAnimation3}
       />
     </ViroAnimatedComponent>



     <ViroAnimatedComponent animation="loopRotate"
      run={this.state.runAnimation4} >
     <Viro3DObject source={require('./res/tantiveIV.obj')}
       position={[-2, -2, -5]}
       materials={["grey"]}
       rotation={[0,0,0]}
       scale={[.2,.2,.2]}
       />
     </ViroAnimatedComponent>

     <ViroImage source={require('./res/icon_left_w.png')} position={[-2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showPrevious} />
     <ViroText text="Viro3DObject" position={[0, -5, -3]} transformBehaviors={["billboard"]} />
     <ViroImage source={require('./res/icon_right_w.png')} position={[2, -4, -3]} scale={[1, 1, 1]} transformBehaviors={["billboard"]} onClick={this._showNext} />

     </ViroScene>

    );
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

  _showNext() {
    this.props.sceneNavigator.push({scene:Viro360ImageTest});
  },

});

ViroAnimations.registerAnimations({
  loopRotate:{properties:{rotateY:"+10"}, duration:250},
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

   grey: {
     shininess : 2.0,
     lightingModel: "Blinn",
     diffuseTexture: require('./res/grey.jpg'),
   },

});

module.exports = Viro3DObjectTest;
