/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroAnimations
 * @flow
 */
import { processColor } from 'react-native';
var AnimationManager = require('react-native').NativeModules.VRTAnimationManager;
var AnimationValidation = require('./ViroAnimationValidation');

class ViroAnimations {
    static registerAnimations(animations:{[key:string]: any}) {
      var result = {};
      for(var key in animations) {
          if (animations[key].constructor===Array){
              // Validate a given animation chain.
              AnimationValidation.validateAnimationChain(key, animations);
          } else {
              // Validate single animation.
              AnimationValidation.validateAnimation(key, animations);
              if(animations[key].properties && animations[key].properties.color) {
                  var newColor = processColor(animations[key].properties.color);
                  animations[key].properties.color = newColor;
              }
          }
      }
      AnimationManager.setJSAnimations(animations);
    }
}

module.exports = ViroAnimations;
