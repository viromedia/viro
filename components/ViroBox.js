/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

var NativeModules = require('NativeModules');
var PropTypes = require('react/lib/ReactPropTypes');
var React = require('React');

var requireNativeComponent = require('requireNativeComponent');

var ViroBox = React.createClass({
  propTypes: {
    position: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    visible: PropTypes.bool,
    opacity: PropTypes.number,
    width: PropTypes.number,
    height: PropTypes.number,
    length: PropTypes.number,
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
  },
  render: function() {
    let materials = typeof this.props.materials === 'string' ? new Array(this.props.materials) : this.props.materials;

    if (this.props.material) {
      console.error('The <ViroBox> component takes a `materials` property rather than `material`.');
    }

    if(materials) {
      if(materials.length!=1 && materials.length!=6) {
        console.error("<ViroBox> can only take 1 material for all sides or 6, one for each side.")
      }
    }
    return (
        <VRTBox {...this.props} materials={materials} />
    );
  }
});

var VRTBox = requireNativeComponent(
    'VRTBox', ViroBox
);

module.exports = ViroBox;
