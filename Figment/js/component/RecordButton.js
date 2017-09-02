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
import {StyleSheet,
        TouchableHighlight,
        Image,
        Animated,
        Easing,
        LayoutAnimation,
      } from 'react-native';

var PropTypes = React.PropTypes;

class RecordButton extends Component {
  constructor(props) {
    super(props);
    this.spinValue = new Animated.Value(0);

    var animations = [
      {
        animation: 'spin',
        enabled: true,
      }
    ];
    this.state = {
      animations: animations
    };

  }

  spin() {
    this.spinValue.setValue(0);
    Animated.timing(
      this.spinValue,
      {
        toValue: 1,
        duration: 1500,
        easing: Easing.linear
      }
    ).start(() => {
      this.spin();
    });
  }

  render() {
    const spin = this.spinValue.interpolate({
      inputRange:[0, 1],
      outputRange: ['0deg', '360deg']
    });
    {this.spin()}
    var index = 0;
    if(this.props.buttonState === 'off') {
      console.log("BUTTONCOMPONENT: SETTING BUTTON STATE TO OFF!!!");
      index = 1;
    } else {
      console.log("BUTTONCOMPONENT: SETTING BUTTON STATE TO ON!!!");
      console.log(this.props.buttonState);
    }
    return (
        <Animated.Image 
        style={[this.props.style,
                          {
                            transform:[
                              {rotate: spin}
                            ]
                          }
                        ]}
        source={this.props.stateImageArray[index]} 
                         />
    );
  }
}
  RecordButton.propTypes = {
      onPress: PropTypes.func.isRequired,
      buttonState: PropTypes.oneOf(['on', 'off']).isRequired,
      stateImageArray: PropTypes.array.isRequired,
      style: PropTypes.any
  };

export default RecordButton;
