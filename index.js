/**
 * Copyright (c) 2016-present, Viro Media, Inc.
 * All rights reserved. 
 *
 */
'use strict';

const ReactViro = {
  // Components
  get ViroSceneNavigator() { return require('./components/ViroSceneNavigator'); },
  get ViroScene() { return require('./components/ViroScene'); },
  get ViroBox() { return require('./components/ViroBox'); },
  get Materials() { return require('./components/Material/Materials'); },
}

module.exports = ReactViro;
