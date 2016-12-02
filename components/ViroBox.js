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

var ViroBox = React.createClass({
  propTypes: {
      position: PropTypes.arrayOf(PropTypes.number),
      rotation: PropTypes.arrayOf(PropTypes.number),
      scale: PropTypes.arrayOf(PropTypes.number),
      opacity: PropTypes.number,
      visible: PropTypes.bool,
  },

    render: function() {
      return (
        <VRTBox {...this.props}
        />
      );
    },

});

var VRTBox = requireNativeComponent(
    'VRTBox', ViroBox
);

module.exports = ViroBox;
