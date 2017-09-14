/**
 * Copyright (c) 2016-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule MaterialValidation
 * @flow
 */
'use strict';

import React from 'react';
import PropTypes from 'prop-types';

var MaterialPropTypes = require('./MaterialPropTypes');
var invariant = require('fbjs/lib/invariant');

class MaterialValidation {

  /*
   This method checks 1 prop at a time even though we could be checking
   all of them at a time so that we can surface a better error message
   */
  static validateMaterialProp(prop, materialName, material, caller) {
    if (!__DEV__) {
      return;
    }
    if (allMaterialTypes[prop] === undefined) {
      var message1 = '"' + prop + '" of material "' + materialName + '" is not a valid material property.';
      var message2 = '\nValid material props: ' +
        JSON.stringify(Object.keys(allMaterialTypes).sort(), null, '  ');
      materialError(message1, material, caller, message2);
    }

    var errorCallback = ()=>{
      materialError('"' + prop + '" of material "' + materialName + '" is not valid.', material, caller);
    };
    let validationDict = {};
    validationDict[prop] = MaterialPropTypes[prop];
    let valueDict = {};
    valueDict[prop] = material[prop];
    PropTypes.checkPropTypes(validationDict, valueDict, 'prop', caller, errorCallback);
  }

  static validateMaterial(name, materials) {
    if (!__DEV__) {
      return;
    }
    for (var prop in materials[name]) {
      MaterialValidation.validateMaterialProp(prop, name, materials[name], 'MaterialValidation ' + name);
    }

    // If we don't want to "loop", then we can use the below commented out code to simply
    // check all the props at once!
    // var errorCallback = ()=>{
    //   materialError("Error validating Material: [" + name + "]", materials[name], 'MaterialValidation ' + name);
    // };
    // PropTypes.checkPropTypes(MaterialPropTypes, name, materials[name], 'prop', 'MaterialValidation ' + name, errorCallback);

  }

  static addValidMaterialPropTypes(materialPropTypes) {
    for (var key in materialPropTypes) {
      allMaterialTypes[key] = materialPropTypes[key];
    }
  }
}

var materialError = function(message1, material, caller?, message2?) {
  invariant(
    false,
    message1 + '\n' + (caller || '<<unknown>>') + ': ' +
    JSON.stringify(material, null, '  ') + (message2 || '')
  );
};

var allMaterialTypes = {};

MaterialValidation.addValidMaterialPropTypes(MaterialPropTypes);


module.exports = MaterialValidation;
