'use strict';

import React, { Component } from 'react';
import {StyleSheet} from 'react-native';
import {ViroParticleEmitter} from 'react-viro';
import PropTypes from 'prop-types';
var createReactClass = require('create-react-class');

/*
 Basic snow emitter that "rains snow" over a given area.
 */
var SnowEmitter = createReactClass({
  propTypes: {
    run: PropTypes.bool,
    /*
     A 2 dimensional array describing the [width, length]
     squarea "snow-zone" area.
     */
    emissionArea: PropTypes.arrayOf(PropTypes.number),

    /*
     The height at which to start spawning the snow
     */
    emissionHeight: PropTypes.number.isRequired,

    /*
     Density of the snow storm.
     */
    snowRate: PropTypes.number,

    /*
     The speed of falling snow.
     */
    fallSpeed: PropTypes.number,

    /*
     The scale of wind shear in this snow storm that would causes
     snow to drift sideways. Set to 0 for vertically falling snow.
     */
    windShear: PropTypes.number,
  },

  render: function() {
    var snowSpawnRate;
    if (this.props.snowRate){
      snowSpawnRate = 100 * this.props.snowRate;
    } else {
      snowSpawnRate = 100;
    }

    var fallSpeed = (this.props.fallSpeed == undefined) ? 1.0 : this.props.fallSpeed;
    var windShear = (this.props.windShear == undefined) ? 1.0 : this.props.windShear;
    return (
      <ViroParticleEmitter
        position={[0, 4.5, 0]}
        duration={2000}
        visible={true}
        delay={0}
        run={this.props.run}
        loop={true}
        fixedToEmitter={true}

        image={{
          source:require("./res/particle_snow.png"),
          height:0.01,
          width:0.01,
          bloomThreshold:1.0
        }}

        spawnBehavior={{
          particleLifetime:[5000,5000],
          emissionRatePerSecond:[snowSpawnRate, snowSpawnRate],
          spawnVolume:{
            shape:"box",
            params:[20, 1, 20],
            spawnOnSurface:false
          },
          maxParticles:2000
        }}

        particleAppearance={{
          opacity:{
            initialRange:[0, 0],
            factor:"time",
            interpolation:[
              {endValue:1.0, interval:[0,500]},
              {endValue:0.0, interval:[4000,5000]}
            ]
          },
          rotation:{
            initialRange:[0, 360],
            factor:"time",
            interpolation:[
              {endValue:1080, interval:[0,5000]},
            ]
          },
          scale:{
            initialRange:[[5,5,5], [10,10,10]],
            factor:"time",
            interpolation:[
              {endValue:[6,6,6], interval:[0,1000]},
              {endValue:[10,10,10], interval:[3000,5000]},
              {endValue:[5,5,5], interval:[4000,5000]}
            ]
          }
        }}

        particlePhysics={{
          velocity:{
            initialRange:[
              [-2* windShear,-.5,0],
              [2 * windShear,-3.0 * fallSpeed,0]
            ]
          }
        }}
      />
    );
  }
});

module.exports = SnowEmitter;
