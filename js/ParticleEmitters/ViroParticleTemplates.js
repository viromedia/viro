'use strict';

import React, { Component } from 'react';
import {StyleSheet} from 'react-native';

var SnowEmitter = require('./SnowEmitter');
var FireworkEmitter = require('./FireworkEmitter');
var FlameEmitter = require('./FlameEmitter');
var SmokeEmitter = require('./SmokeEmitter');
var createReactClass = require('create-react-class');

import {
  ViroScene,
  ViroText,
  Viro360Image,
  ViroMaterials,
  ViroBox,
  Viro3DObject,
  ViroOmniLight
} from 'react-viro';

/*
 Scene with a set of particle template examples, where the user
 can iterate through each particle emitters by clicking the scene.
 */
var ViroParticleTemplates = createReactClass({
  getInitialState() {
    return {
      currentEffect:0
    };
  },

  clickedScene(){
    var nextEffect = this.state.currentEffect + 1;
    if (this.state.currentEffect > 3){
      nextEffect = 0;
    }
    this.setState({
      currentEffect:nextEffect
    })
  },

  render: function() {
    return (
     <ViroScene onClick={this.clickedScene}>
        {this.getEffect()}
     </ViroScene>
    );
  },

  getEffect(){
    if (this.state.currentEffect == 0){
      return this.getSmoke();
    } else if (this.state.currentEffect == 1){
      return this.getFlames();
    } else if (this.state.currentEffect == 2){
      return this.getSnow();
    }
    return this.getFireworks();
  },

  getFlames(){
    return(
      <FlameEmitter
        location={[0,-0.8,-2]} />
    );
  },

  getSmoke(){
    return(
      <SmokeEmitter
        run={true}
        location={[0,-1,-5]}
        power={5.0}
      />
    );
  },

  getFireworks(){
    return(
      <FireworkEmitter
        loop={true}
        run={true}
        explosionLocation={[0,5,-8]}
        explosionSize={6.0}
        explosionDelay={1000}
        startColor={"#ff2d2d"}
        endColor={"#42ff42"}  />
    );
  },

  getSnow(){
    return (
      <SnowEmitter
        run={true}
        emissionArea={[20, 20]}
        emissionHeight={4.5}
        snowRate={1.0}
        fallSpeed={1.0}
        windShear={1.0} />
    );
  },
});

var styles = StyleSheet.create({
  helloWorldTextStyle: {
    fontFamily: 'HelveticaNeue-Medium',
    fontSize: 40,
    color: '#ffffff',
  },
});

ViroMaterials.createMaterials({
  ground:{
    cullMode: "None",
    shininess: 2.0,
    diffuseColor: "#ff9999",
    lightingModel: "Blinn",
  }
 });
module.exports = ViroParticleTemplates;
