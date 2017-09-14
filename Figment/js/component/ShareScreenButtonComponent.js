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
        Animated,
        Easing,
        Image,
        View,
        } from 'react-native';
import renderIf from '../helpers/renderIf';

var PropTypes = React.PropTypes;

class ShareScreenButtonComponent extends Component {
  constructor(props) {
    super(props);
    this.scaleValue = new Animated.Value(0);
    this.fadeInValue = new Animated.Value(0);
    // Bindings
    this.scale = this.scale.bind(this);
    this._onPress = this._onPress.bind(this);
    this.crossFade = this.crossFade.bind(this);

    var imgSource = this.props.stateImageArray[1];
    var imgClickSource = this.props.stateImageArray[0];

    this.buttonScale = this.scaleValue.interpolate({
      inputRange: [0, 0.5, 0.8, 1],
      outputRange: [1, 0.8, 1.1, 1]
    });

    this.opacity = this.fadeInValue.interpolate({
      inputRange: [0,1],
      outputRange: [0,1]
    });

  }
  componentWillMount() {
    if (this.props.selected) {
        this.fadeInValue.setValue(1);
    }
  }

  
  render() {

    return (
      <TouchableHighlight underlayColor="#00000000" onPress={this._onPress}>
      <View style={{position: 'absolute', width: 70, height: 70, justifyContent: 'center', alignItems: 'center',}}>
        <Animated.Image 
            source={this.props.stateImageArray[1]}
            style={[this.props.style,
                      {
                        transform:[
                          {scale: this.buttonScale}
                        ]
                      }
                  ]} />
        <Animated.Image 
          source={this.props.stateImageArray[0]}
          style={[this.props.style, {opacity: this.opacity}]} />
      </View>
      </TouchableHighlight>
      );
  }

    componentDidUpdate() {
    // Don't know why this works. Don't touch this. Magic.
    if(this.props.buttonState === 'off') {
      this.fadeInValue.setValue(0);
    }
    
    // }
  }
  _onPress() {

    if (this.props.buttonState === 'off') {
      this.scale();
      // Picked from here https://facebook.github.io/react-native/docs/performance.html#my-touchablex-view-isn-t-very-responsive
      requestAnimationFrame(() => {
        this.props.onPress();   
        
      });
    }
  }
  scale() {
    this.scaleValue.setValue(0);
    this.fadeInValue.setValue(0);
    Animated.timing(
        this.scaleValue,
        {
          toValue: 1,
          duration: 300,
          easing: Easing.easeInOutBack,
          useNativeDriver: true,
        }
    ).start(() => {
      this.crossFade();
    });
  }
  crossFade() {
    this.fadeInValue.setValue(0);
    Animated.timing(
      this.fadeInValue,
      {
        toValue: 1,
        duration: 100,
        easing: Easing.linear,
        useNativeDriver: true, 
      }
    ).start();
  }
}


ShareScreenButtonComponent.propTypes = {
        onPress: PropTypes.func.isRequired,
        buttonState: PropTypes.oneOf(['on', 'off']).isRequired,
        stateImageArray: PropTypes.array.isRequired,
        style: PropTypes.any,
        selected: PropTypes.bool,
};

export default ShareScreenButtonComponent;
