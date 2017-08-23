
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

    getInitialState() {
      return {
        scale : this.props.portalItem.scale,
        rotation : [0, 0, 0],
      }
    },

    componentWillMount() {
      this._ref_object = null;
    },

    render: function() {
        var j = this.props.index;
        return (
          <ViroPortal position={[0, 0, 0]} scale={this.props.portalItem.scale} onRotate={this._onRotateGesture(j)}
          onPinch={this._onPinchIndex(j)} passable={true} ref={this._setComponentRef()} >
             <ViroPortalFrame>
               <Viro3DObject source={this.props.portalItem.obj}
                             position={[0, 0, 0]}
                             materials={this.props.portalItem.materials}
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

    /*
     Rotation should be relative to its current rotation *not* set to the absolute
     value of the given rotationFactor.
     */
    _onRotate(rotateState, rotationFactor, source, index) {
      if(rotateState == 1) {
        console.log("STARTING ROTATE WITH Rotation factor: " + rotationFactor);
        return;
      } else if(rotateState ==2){
        console.log("MID ROTATE WITH Rotation factor: " + rotationFactor);
      } else if(rotateState == 3) {
        console.log("END ROTATE WITH Rotation factor: " + rotationFactor);
        this.setState({
          rotation : [0, this.state.rotation[1] - rotationFactor, 0]
        })
        return;
      }

      console.log("ONROTATE INDEX:" + index);

      this._ref_object.setNativeProps({rotation:[0, this.state.rotation[1] - rotationFactor, 0]});
    },

    /*
     Pinch scaling should be relative to its last value *not* the absolute value of the
     scale factor. So while the pinching is ongoing set scale through setNativeProps
     and multiply the state by that factor. At the end of a pinch event, set the state
     to the final value and store it in state.
     */
    _onPinch(pinchState, scaleFactor, source, index) {
      if(pinchState == 1) {
        console.log("STARTING PINCH WITH Scale factor: " + scaleFactor);
        return;
      } else if(pinchState == 2){
        console.log("MID PINCH WITH Scale factor: " + scaleFactor);
      } else if(pinchState == 3) {
        console.log("END PINCH WITH Scale factor: " + scaleFactor);
        this.setState({
          scale : this.state.scale.map((x)=>{return x * scaleFactor})
        });
        return;
      }

      console.log("ONPINCH INDEX:" + index);

      var newScale = this.state.scale.map((x)=>{return x * scaleFactor})
      this._ref_object.setNativeProps({scale:newScale});
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
