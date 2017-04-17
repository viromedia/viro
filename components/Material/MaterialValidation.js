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

var MaterialPropTypes = require('./MaterialPropTypes');
var ReactPropTypesSecret = require('react/lib/ReactPropTypesSecret');
var invariant = require('fbjs/lib/invariant');

class MaterialValidation {
  static validateMaterialProp(prop, material, caller) {
    if (!__DEV__) {
      return;
    }
    if (allMaterialTypes[prop] === undefined) {
      var message1 = '"' + prop + '" is not a valid material property.';
      var message2 = '\nValid material props: ' +
        JSON.stringify(Object.keys(allMaterialTypes).sort(), null, '  ');
      materialError(message1, material, caller, message2);
    }
    var error = allMaterialTypes[prop](
      material,
      prop,
      caller,
      'prop',
      null,
      ReactPropTypesSecret
    );
    if (error) {
      materialError(error.message, material, caller);
    }
  }

  static validateMaterial(name, materials) {
    if (!__DEV__) {
      return;
    }
    for (var prop in materials[name]) {
      MaterialValidation.validateMaterialProp(prop, materials[name], 'MaterialValidation ' + name);
    }
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
