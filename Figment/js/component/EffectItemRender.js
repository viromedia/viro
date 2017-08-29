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
import * as UIConstants from '../redux/UIConstants';
import {
  ViroScene,
  ViroARScene,
  ViroARNode,
  ViroARPlane,
  ViroMaterials,
  ViroNode,
  Viro3DObject,
  ViroText,
} from 'react-viro';


var PropTypes = React.PropTypes;

var EffectItemRender = React.createClass({
    propTypes: {
        effectItem: PropTypes.any,
        index: PropTypes.number,
    },

    componentWillMount() {
      this._ref_object = null;
    },

    getInitialState() {
      return {
        rotation : [0, 0, 0],
        position: [0, 0, 0],
      }
    },

    render: function() {
        var j = this.props.index;
        console.log("Rendering effect!!");
        return (
          <ViroNode key={j} position={this.state.position} >
            {this.props.effectItem.effect()}
          </ViroNode>
        );
    },
});

module.exports = EffectItemRender;
