/**
 * Copyright (c) 2016-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

import {
  ViroSceneNavigator,
  ViroScene,
  ViroBox,
} from 'react-viro';

var InitialScene = require('./js/HelloWorldScene');

export default class ViroSample extends Component {
  render() {
    return (
      <ViroSceneNavigator apiKey="7EEDCB99-2C3B-4681-AE17-17BC165BF792"
       initialScene={{scene: InitialScene}}
        />
    );
  }
}

AppRegistry.registerComponent('APP_NAME_HERE', () => ViroSample);

