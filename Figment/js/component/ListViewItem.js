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

class ListViewItem extends Component {
  constructor(props) {
    super(props);
    this.scaleValue = new Animated.Value(0);
    // Bindings
    this.scale = this.scale.bind(this);
    this._onPress = this._onPress.bind(this);

    var imgSource = this.props.stateImageArray[1];
    var imgClickSource = this.props.stateImageArray[0];

    this.buttonScale = this.scaleValue.interpolate({
      inputRange: [0, 0.5, 0.8, 1],
      outputRange: [1, 0.8, 1.1, 1]
    });

    this.state = {
      showSelection:false,
    };
  }  
  render() {
    console.log("render ListViewItem");
    return (
      <TouchableHighlight underlayColor="#00000000" onPress={this._onPress}>
      <View>

        <Animated.Image 
            source={this.props.stateImageArray[0]}
            style={[this.props.style,
                      {
                        transform:[
                          {scale: this.buttonScale}
                        ]
                      }
                  ]} />

      </View>
      </TouchableHighlight>
      );
  }

  _onPress() {
    this.scale();

    // Picked from here https://facebook.github.io/react-native/docs/performance.html#my-touchablex-view-isn-t-very-responsive
    requestAnimationFrame(() => {
      this.props.onPress();   
    });
  }
  scale() {
    this.scaleValue.setValue(0);
    Animated.timing(
        this.scaleValue,
        {
          toValue: 1,
          duration: 300,
          easing: Easing.easeInOutBack,
          useNativeDriver: true,
        }
    ).start(this.props.animationDoneCallBack());
  }
}

var styles = StyleSheet.create({
  photo: {
    position: 'absolute',
    height: 53,
    width: 56.8,
    marginTop: 10,
  },
});

ListViewItem.propTypes = {
        onPress: PropTypes.func.isRequired,
        stateImageArray: PropTypes.array.isRequired,
        style: PropTypes.any,
        selected: PropTypes.bool,
        animationDoneCallBack: PropTypes.func,
};

export default ListViewItem;
