/**
 * Copyright (c) 2016-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroNode
 */

'use strict';

import { requireNativeComponent, View } from 'react-native';

var NativeMethodsMixin = require('react/lib/NativeMethodsMixin');
var NativeModules = require('NativeModules');
var PropTypes = require('react/lib/ReactPropTypes');
var React = require('React');
var StyleSheet = require('StyleSheet');

/**
 * Absolute container for Viro Controls
 */
var ViroNode = React.createClass({
  mixins: [NativeMethodsMixin],

  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    visible: PropTypes.bool,
    opacity: PropTypes.number,
  },

  render: function() {
    // Since transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;
    return (
      <VRTViewContainer {...this.props} transformBehaviors={transformBehaviors} />
    );
  }
});


var VRTViewContainer = requireNativeComponent(
  'VRTViewContainer', ViroNode, {
    nativeOnly: {onTap: true, onGaze: true, materials: []}
  }
);


module.exports = ViroNode;
