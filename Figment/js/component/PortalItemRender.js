
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
  ViroNode,
  Viro3DObject,
  Viro360Image,
  ViroPortal,
  ViroPortalFrame,
  ViroMaterials,
} from 'react-viro';


var PropTypes = require('react/lib/ReactPropTypes');

var PortalItemRender = React.createClass({
    propTypes: {
        portalItem: PropTypes.any,
        onLoadCallback: PropTypes.func,
        index: PropTypes.number,
    },

    componentWillMount() {
      this._ref_object = null;
    },

    render: function() {
        var j = this.props.index;
        return (
          <ViroPortal position={[0, 0, 0]} scale={this.props.portalItem.scale}>
             <ViroPortalFrame>
               <Viro3DObject source={this.props.portalItem.obj}
                             position={[0, 0, 0]}
                             rotation={[0, 0, 0]}
                             materials={this.props.portalItem.materials}
                             scale={this.props.portalItem.scale}
                             onPinch={this._onRotateGesture(j)}
                             onRotate={this._onPinchIndex(j)}
                             resource={this.props.portalItem.resources} onLoadStart={this._onObjectLoadStart(j)} onLoadEnd={this._onObjectLoadEnd(j)}/>
             </ViroPortalFrame>
            <Viro360Image source={require('../res/360_diving.jpg')} />
          </ViroPortal>
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

      var scaleX = this.props.portalItem.scale[0] * scaleFactor;
      var scaleY = this.props.portalItem.scale[1] * scaleFactor;
      var scaleZ = this.props.portalItem.scale[2] * scaleFactor;
      console.log("ONPINCH INDEX:" + index);

      this._ref_object.setNativeProps({scale:[scaleX, scaleY, scaleZ]});
    },

    _onError(index) {
        return () => {
          console.log("MODEL has error HAS ERROR" + index);
          this.props.loadCallback(index, LoadConstants.ERROR);
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
        };
    },
});

ViroMaterials.createMaterials({
  portal_ship: {
    diffuseTexture: require("../res/portal_ship/portal_ship_diffuse.png"),
    normalTexture: require("../res/portal_ship/portal_ship_normal.png"),
  },
});

module.exports = PortalItemRender;
