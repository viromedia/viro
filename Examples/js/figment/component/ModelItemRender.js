/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */
'use strict';

import React, { Component } from 'react';
import * as LoadConstants from '../redux/LoadingStateConstants';
import {
  ViroScene,
  ViroARScene,
  ViroARNode,
  ViroARPlaneSelector,
  ViroARPlane,
  ViroBox,
  ViroMaterials,
  ViroNode,
  Viro3DObject,
} from 'react-viro';


var PropTypes = require('react/lib/ReactPropTypes');

var ModelItemRender = React.createClass({
    propTypes: {
        modelItem: PropTypes.any,
        onLoadCallback: PropTypes.func,
        index: PropTypes.number,
    },

    componentWillMount() {
      this._ref_object = null;
    },



    render: function() {
        var j = this.props.index;
        return (
        <Viro3DObject ref={this._setComponentRef()}
            scale={this.props.modelItem.scale}
            source={this.props.modelItem.obj}
            materials={this.props.modelItem.materials}
            resources={this.props.modelItem.resources}
            animation={this.props.modelItem.animation}
            onError={this._onError(j)}  onRotate={this._onRotateGesture(j)} onLoadStart={this._onObjectLoadStart(j)} onLoadEnd={this._onObjectLoadEnd(j)}
            position={[0,0,0]} onPinch={this._onPinchIndex(j)} />
        );
    },

    _setComponentRef() {
      return (component) => {
        console.log("SETTING COMPONENT REF!!! index:" + this.props.index);
        console.log("Component ref value:");
        this._ref_object = component;
      }
    },

    _onRotateGesture(index) {
      return ((rotateState, rotationFactor, source)=> {
          this._onRotate(rotateState, rotationFactor, source, index);
      });
    },

    _onPinchIndex(index) {
      return ((pinchState, scaleFactor, source)=> {
          this._onPinch(pinchState, scaleFactor, source, index);
      });
    },

    _onRotate(rotateState, rotationFactor, source, index) {
      if(rotateState == 1) {
        console.log("STARTING ROTATE WITH Scale factor: " + rotationFactor);
      } else if(rotateState ==2){
        console.log("MID ROTATE WITH Scale factor: " + rotationFactor);
      } else if(rotateState == 3) {
        console.log("END ROTATE WITH Scale factor: " + rotationFactor);
      }

      console.log("ONROTATE INDEX:" + index);

      this._ref_object.setNativeProps({rotation:[0, -rotationFactor, 0]});
    },

    _onPinch(pinchState, scaleFactor, source, index) {
      if(pinchState == 1) {
        console.log("STARTING PINCH WITH Scale factor: " + scaleFactor);
      } else if(pinchState ==2){
        console.log("MID PINCH WITH Scale factor: " + scaleFactor);
      } else if(pinchState == 3) {
        console.log("END PINCH WITH Scale factor: " + scaleFactor);
      }

      var scaleX = this.props.modelItem.scale[0] * scaleFactor;
      var scaleY = this.props.modelItem.scale[1] * scaleFactor;
      var scaleZ = this.props.modelItem.scale[2] * scaleFactor;
      console.log("ONPINCH INDEX:" + index);

      this._ref_object.setNativeProps({scale:[scaleX, scaleY, scaleZ]});
    },

    _onError(index) {
        return () => {
          console.log("MODEL has error HAS ERROR" + index);
          this.props.loadCallback(index, LoadConstants.ERROR);
          //this.props.arSceneNavigator.viroAppProps.loadingObjectCallback(index, LoadingConstants.LOAD_ERROR);
        };

      },

    _onObjectLoadStart(index) {
        return () => {
          this.props.onLoadCallback(index, LoadConstants.LOADING);
        };
    },

    _onObjectLoadEnd(index) {
        return () => {
          this.props.onLoadCallback(index, LoadConstants.LOADED);
          //this.props.arSceneNavigator.viroAppProps.loadingObjectCallback(index, LoadingConstants.LOADED);
        };
    },
});

ViroMaterials.createMaterials({
  transparentFloor: {
    diffuseColor: "#ff000000",
  },
  porsche: {
    diffuseTexture: require("../res/car_porsche/Porsche911turboS_diff.jpg"),
  },
  bball: {
    diffuseTexture: require("../res/bball/bball.jpg"),
  },
  tesla: {
    shininess: 1.0,
    lightingModel:"Blinn",
  },
  transparentFloor: {
    diffuseColor: "#ff000000",
    writesToDepthBuffer: false,
    readsFromDepthBuffer: false,
  },

});

module.exports = ModelItemRender;
