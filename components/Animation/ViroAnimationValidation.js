/**
 * Copyright (c) 2016-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroAnimationValidation
 * @flow
 */
'use strict';

var AnimationPropTypes = require('./ViroAnimationPropTypes');
var ReactPropTypeLocations = require('react/lib/ReactPropTypeLocations');
var ReactPropTypesSecret = require('react/lib/ReactPropTypesSecret');
var invariant = require('fbjs/lib/invariant');

class ViroAnimationValidation {
  static validateAnimationProp(prop, animation, caller) {
    if (!__DEV__) {
      return;
    }
    if (allAnimationTypes[prop] === undefined) {
      var message1 = '"' + prop + '" is not a valid animation property.';
      var message2 = '\nValid animation props: ' +
        JSON.stringify(Object.keys(allAnimationTypes).sort(), null, '  ');
      animationError(message1, animation, caller, message2);
    }
    //TODO: add more specific error checking for Animations here.
    var error = allAnimationTypes[prop](
      animation,
      prop,
      caller,
      ReactPropTypeLocations.prop,
      null,
      ReactPropTypesSecret
    );
    if (error) {
      animationError(error.message, animation, caller);
    }
  }

  static validateAnimation(name, animations) {
    if (!__DEV__) {
      return;
    }
    for (var prop in animations[name]) {
      ViroAnimationValidation.validateAnimationProp(prop, animations[name], 'AnimationValidation ' + name);
    }
  }

  /**
   * Iterate through array to determine if there are empty chains (invalid)
   * or if there are any empty elements within a given chain (invalid)
   */
  static validateAnimationChain(name, animations) {
    if (!__DEV__) {
      return;
    }

    var arrayChains = animations[name];
    if (!(arrayChains.constructor===Array) || arrayChains.length <= 0){
      animationError("Invalid chains: Array of Animation chains must be a non empty array!",
          animations[name], 'AnimationValidation ' + name);
      return;
    }

    for (var chainIndex = 0; chainIndex < arrayChains.length; chainIndex ++){
      if (!(arrayChains[chainIndex].constructor===Array) || arrayChains[chainIndex].length <=0) {
        animationError("Invalid chain: individual Animation chain must be a non empty array!",
            animations[name], 'AnimationValidation ' + name);
        return;
      }
    }
  }

  static addValidAnimationPropTypes(animationPropTypes) {
    for (var key in animationPropTypes) {
      allAnimationTypes[key] = animationPropTypes[key];
    }
  }
}

var animationError = function(message1, animation, caller?, message2?) {
  invariant(
    false,
    message1 + '\n' + (caller || '<<unknown>>') + ': ' +
    JSON.stringify(animation, null, '  ') + (message2 || '')
  );
};

var allAnimationTypes = {};

ViroAnimationValidation.addValidAnimationPropTypes(AnimationPropTypes);

module.exports = ViroAnimationValidation;
