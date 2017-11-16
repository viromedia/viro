'use strict';

import React, { Component } from 'react';
import {StyleSheet} from 'react-native';
import {ViroParticleEmitter} from 'react-viro';
import PropTypes from 'prop-types';
var ColorPropType = require('react-native').ColorPropType;
var createReactClass = require('create-react-class');

var FireworkEmitter = createReactClass({
  propTypes: {
    loop: PropTypes.loop,
    run: PropTypes.bool,
    explosionLocation:PropTypes.arrayOf(PropTypes.number),
    explosionSize:PropTypes.number.isRequired,
    explosionDelay:PropTypes.number,
    startColor:PropTypes.arrayOf(ColorPropType),
    endColor:PropTypes.arrayOf(ColorPropType)
  },

  render: function() {
    var viroFireworkColors =["#ff2d2d","#42ff42","#00edff","#ffff00","#ffb5f8","#00ff1d","#00edff","#ffb14c", "#ff7cf4"];
    var colorRand1 = viroFireworkColors[Math.floor((Math.random() * 5) + 0)];
    var colorRand2 = viroFireworkColors[Math.floor((Math.random() * 5) + 0)];
    var colorRand3 = viroFireworkColors[Math.floor((Math.random() * 5) + 0)];

    var startColorRange1 = this.props.startColor == undefined ? colorRand1 : this.props.startColor;
    var startColorRange2 = this.props.startColor == undefined ? colorRand2 : this.props.startColor;
    var endColor = this.props.endColor == undefined ? colorRand3 : this.props.endColor;

    return (
      <ViroParticleEmitter
          position={this.props.explosionLocation}
          duration={1200}
          delay={this.props.explosionDelay}
          visible={true}
          run={true}
          loop={this.props.loop}
          fixedToEmitter={true}

          image={{
            source:require("./res/particle_firework.png"),
            height:0.1,
            width:0.1,
            bloomThreshold:0.0
          }}

          spawnBehavior={{
            particleLifetime:[1200,1200],
            emissionRatePerSecond:[0,0],
            emissionBurst:[
              {time:0, min:300, max:350, cycles:1}
            ],
            spawnVolume:{shape:"sphere", params:[0.15], spawnOnSurface:true},
            maxParticles:1000
          }}

          particleAppearance={{
            opacity:{
              initialRange:[1.0, 1.0],
              factor:"time",
              interpolation:[
                {endValue:0.0, interval:[800,1200]}
              ]
            },

            color:{
              initialRange:[startColorRange1, startColorRange2],
              factor:"time",
              interpolation:[
                {endValue:endColor, interval:[300,1200]}
              ]
            }
          }}

          particlePhysics={{
            explosiveImpulse:{impulse:0.12 * this.props.explosionSize, position:[0,0,0], decelerationPeriod:1.0},
          }}
        />
    );
  }
});

module.exports = FireworkEmitter;
