/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroARPlaneSelector
 */

'use strict';

import { requireNativeComponent, View, StyleSheet, findNodeHandle } from 'react-native';
import React, { Component } from 'react';
import PropTypes from 'prop-types';

var createReactClass = require('create-react-class');

var ViroMaterials = require('../Material/ViroMaterials');
var ViroARPlane = require('./ViroARPlane');
var ViroSurface = require('../ViroSurface');
var ViroNode = require('../ViroNode');

var _maxPlanes = 15;
var _planePrefix = "ViroARPlaneSelector_Plane_"

/**
 * This component wraps the logic required to enable user selection
 * of an AR plane. This currently only allows for 1 plane to be selected,
 * but could easily be modified to allow for more planes.
 */
var ViroARPlaneSelector = createReactClass({
  propTypes: {
    ...View.propTypes,
    maxPlanes: PropTypes.number,
    minHeight: PropTypes.number,
    minWidth: PropTypes.number,
    visible: PropTypes.bool,
    opacity: PropTypes.number,
    ignoreEventHandling: PropTypes.bool,
    dragType: PropTypes.oneOf(["FixedDistance", "FixedToWorld"]),

    onHover: PropTypes.func,
    onClick: PropTypes.func,
    onClickState: PropTypes.func,
    onTouch: PropTypes.func,
    onScroll: PropTypes.func,
    onSwipe: PropTypes.func,
    onDrag: PropTypes.func,
    onPinch: PropTypes.func,
    onRotate: PropTypes.func,
    onFuse: PropTypes.oneOfType([
      PropTypes.shape({
        callback: PropTypes.func.isRequired,
        timeToFuse: PropTypes.number
      }),
      PropTypes.func
    ]),
    onCollision: PropTypes.func,
    viroTag: PropTypes.string,
    onAnchorFound: PropTypes.func,
    onAnchorUpdated: PropTypes.func,
    onAnchorRemoved: PropTypes.func,
    onPlaneSelected: PropTypes.func,
  },

  getInitialState: function() {
    return {
      selectedSurface : -1,
      arPlaneSizes : []
    }
  },

  render: function() {
    return (
      <ViroNode>
        {this._getARPlanes()}
      </ViroNode>
    );
  },

  _getARPlanes() {
    if (this.state.selectedSurface == -1) {
      let arPlanes = [];
      let numPlanes = this.props.maxPlanes || _maxPlanes;
      for (var i = 0; i < numPlanes; i++) {
        let arPlaneSize = this.state.arPlaneSizes[i];
        let surfaceWidth = arPlaneSize ? arPlaneSize.width : 0;
        let surfaceHeight = arPlaneSize ? arPlaneSize.height : 0;
        let surfacePosition = arPlaneSize ? arPlaneSize.center : [0,0,0];
        arPlanes.push((
          <ViroARPlane key={_planePrefix + i}
            minWidth={this.props.minWidth}
            minHeight = {this.props.minHeight}
            onAnchorUpdated={this._onARPlaneUpdated(i)} >
            <ViroSurface materials={"ViroARPlaneSelector_Translucent"}
              onClick={this._getOnClickSurface(i)}
              position={surfacePosition}
              width={surfaceWidth} height={surfaceHeight}
              rotation={[-90,0,0]} />
          </ViroARPlane>
        ));
      }
      return arPlanes;
    } else {
      return (
        <ViroARPlane key={_planePrefix + this.state.selectedSurface}
          {...this.props}>
        </ViroARPlane>
      );
    }
  },

  _getOnClickSurface(index) {
    return ()=>{
      this.setState({selectedSurface : index});
      this._onPlaneSelected();
    }
  },

  _onARPlaneUpdated(index) {
    return (updateMap)=>{
      this.state.arPlaneSizes[index] = updateMap;
      this.setState({
        arPlaneSizes : this.state.arPlaneSizes
      })
    }
  },

  _onPlaneSelected() {
    this.props.onPlaneSelected && this.props.onPlaneSelected();
  },

  /*
  This function allows the user to reset the surface and select a new plane.
  */
  reset() {
    this.setState({
      selectedSurface : -1
    });
  }

});

ViroMaterials.createMaterials({
  ViroARPlaneSelector_Translucent: {
    lightingModel: "Constant",
    diffuseColor: "#88888888"
  },
});

module.exports = ViroARPlaneSelector;
