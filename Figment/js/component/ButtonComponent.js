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
import {StyleSheet, TouchableHighlight, Image} from 'react-native';

var PropTypes = require('react/lib/ReactPropTypes');

var ButtonComponent = React.createClass({
    propTypes: {
        onPress: PropTypes.func.isRequired,
        buttonState: PropTypes.oneOf(['on', 'off']).isRequired,
        stateImageArray: PropTypes.array.isRequired,
    },

    render: function() {
      var index = 0;
      if(this.props.buttonState === 'off') {
        console.log("BUTTONCOMPONENT: SETTING BUTTON STATE TO OFF!!!");
        index = 1;
      } else {
        console.log("BUTTONCOMPONENT: SETTING BUTTON STATE TO ON!!!");
        console.log(this.props.buttonState);
      }
      return (
        <TouchableHighlight onPress={this.props.onPress}>
          <Image source={this.props.stateImageArray[index]} style={this.props.style} />
        </TouchableHighlight>
        );
    },
});

module.exports = ButtonComponent;
