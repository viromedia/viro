/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroMaterials
 * @flow
 */

import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
import assetRegistry from "react-native/Libraries/Image/AssetRegistry"

var MaterialManager = require('react-native').NativeModules.VRTMaterialManager;
var MaterialPropTypes = require('./MaterialPropTypes');
var MaterialValidation = require('./MaterialValidation');
var invariant = require('fbjs/lib/invariant');
var processColor = require('react-native').processColor;

class ViroMaterials {

  static createMaterials(materials:{[key:string]: any}){
    var result = {};
    for(var key in materials){
      MaterialValidation.validateMaterial(key, materials);
      var materialDict = materials[key];
      var resultMaterial = {};
      for(var materialProperty in materialDict) {
        //not the best check, modify to make sure property ends with texture..
        if(materialProperty.endsWith('texture')  || materialProperty.endsWith('Texture')) {
          //textures point to assets, so lets resolve the asset
          if((materialProperty === 'ReflectiveTexture') ||  (materialProperty === 'reflectiveTexture')) {
            var reflectiveShape = {};
            for(var cubeMapTexture in materialDict[materialProperty]) {
              var cubeMapSource = resolveAssetSource(materialDict[materialProperty][cubeMapTexture]);
              reflectiveShape[cubeMapTexture] = cubeMapSource;
            }
            resultMaterial[materialProperty] = reflectiveShape;
          }
          else if (materialDict[materialProperty].hasOwnProperty('source')){
            var source = resolveAssetSource(materialDict[materialProperty]['source']);
            resultMaterial[materialProperty] = materialDict[materialProperty];
            resultMaterial[materialProperty]['source'] = source;
          }
          else {
            var assetType="unknown";
            if (typeof materialDict[materialProperty] !== 'object' ) {
              var asset = assetRegistry.getAssetByID(materialDict[materialProperty]);
              if (asset) {
                assetType = asset.type;
              }
            }

            var source = resolveAssetSource(materialDict[materialProperty]);
            source["type"] = assetType;
            resultMaterial[materialProperty] = source;
          }
        }else if(materialProperty.endsWith('color') || materialProperty.endsWith('Color')) {
          var color = processColor(materialDict[materialProperty]);
          resultMaterial[materialProperty] = color;
        }
        else{
          //just apply material property directly.
          resultMaterial[materialProperty] = materialDict[materialProperty];
        }
        result[key] = resultMaterial;
      }
    }
    MaterialManager.setJSMaterials(result);
  }

  /*
   This function tells the platform to delete/release the given materials from
   memory. This means that the given materials can no longer be referenced. Existing
   components that have already had their materials set will continue to work.

   materials - an array of material names to delete
   */
  static deleteMaterials(materials) {
    MaterialManager.deleteMaterials(materials);
  }
}

module.exports = ViroMaterials;
