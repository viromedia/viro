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

class RecordButton extends Component {
  constructor(props) {
    super(props);
    this.scaleDownValue = new Animated.Value(0);
    this.scaleUpValue = new Animated.Value(0);

    this.fadeInValue = new Animated.Value(0);
    this.fadeOutValue = new Animated.Value(0);

    // Bindings
    this.scale = this.scale.bind(this);
    this._onPress = this._onPress.bind(this);
    this.crossFade = this.crossFade.bind(this);
    this.scaleAndFade = this.scaleAndFade.bind(this);

    var imgSource = this.props.stateImageArray[1];
    var imgClickSource = this.props.stateImageArray[0];

    this.circleScale = this.scaleDownValue.interpolate({
      inputRange: [0, 1],
      outputRange: [1, 0.8]
    });

    this.circleOpacity = this.fadeOutValue.interpolate({
      inputRange: [0,1],
      outputRange: [1,0]
    });

    this.squareOpacity = this.fadeInValue.interpolate({
      inputRange: [0,1],
      outputRange: [0,1]
    });
    this.squareScale = this.scaleUpValue.interpolate({
      inputRange: [0, 1],
      outputRange: [0.8, 1]
    });

  }
  componentWillMount() {
    this.scaleUpValue.setValue(0);
    this.scaleDownValue.setValue(0);

    this.fadeInValue.setValue(0);
    this.fadeOutValue.setValue(0);
  }
  render() {

    return (
      <TouchableHighlight underlayColor="#00000000" onPress={this._onPress}>
      <View style={{backgroundColor: '#0000ff'}} >
        <Animated.Image 
            source={this.props.stateImageArray[1]}
            style={[this.props.style, {opacity: this.props.buttonState == 'off' ? this.circleOpacity : this.squareOpacity},
                      {
                        transform:[
                          {scale: this.props.buttonState == 'off' ? this.circleScale : this.squareScale}
                        ]
                      }
                  ]} />
        <Animated.Image 
          source={this.props.stateImageArray[0]}
          style={[this.props.style, {opacity: this.props.buttonState == 'off' ? this.squareOpacity : this.circleOpacity},
                      {
                        transform:[
                          {scale: this.props.buttonState == 'off' ? this.squareScale : this.circleScale}
                        ]
                      }
                  ]} />
      </View>
      </TouchableHighlight>
      );
  }
  componentDidUpdate() {
    // Don't know why this works. Don't touch this. Magic.
    if (this.props.buttonState == 'off') {
          this.fadeOutValue.setValue(0);
          this.scaleDownValue.setValue(0);
    }
  }
  _onPress() {
      this.scaleAndFade();
     
      // Picked from here https://facebook.github.io/react-native/docs/performance.html#my-touchablex-view-isn-t-very-responsive
      requestAnimationFrame(() => {
        this.props.onPress();   
        
      });
  }
  scale() {
    this.scaleUpValue.setValue(0);
    this.scaleDownValue.setValue(0);

    this.fadeInValue.setValue(0);
    this.fadeOutValue.setValue(0);

    Animated.timing(
        this.scaleValue,
        {
          toValue: 1,
          duration: 200,
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
  scaleAndFade() {
    this.scaleUpValue.setValue(0);
    this.scaleDownValue.setValue(0);

    this.fadeInValue.setValue(0);
    this.fadeOutValue.setValue(0);

    Animated.parallel([
      Animated.timing(
        this.scaleDownValue,
        {
          toValue: 1,
          duration: 200,
          easing: Easing.easeInOutBack,
          useNativeDriver: true,
        }
    ),
    Animated.timing(
        this.scaleUpValue,
        {
          toValue: 1,
          duration: 200,
          easing: Easing.easeInOutBack,
          useNativeDriver: true,
        }
    ),
    Animated.timing(
      this.fadeInValue,
      {
        toValue: 1,
        duration: 200,
        easing: Easing.linear,
        useNativeDriver: true, 
      }
    ),
    Animated.timing(
      this.fadeOutValue,
      {
        toValue: 1,
        duration: 200,
        easing: Easing.linear,
        useNativeDriver: true, 
      }
    )
    ]).start();
  }
}


RecordButton.propTypes = {
        onPress: PropTypes.func.isRequired,
        buttonState: PropTypes.oneOf(['on', 'off']).isRequired,
        stateImageArray: PropTypes.array.isRequired,
        style: PropTypes.any,
        selected: PropTypes.bool,
};

export default RecordButton;
