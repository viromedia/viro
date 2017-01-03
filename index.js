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
  get ViroVideo() { return require('./components/ViroVideo'); },
  get Viro360Video() {return require ('./components/Viro360Video'); },
  get ViroNode() { return require('./components/ViroNode'); },
  get ViroCamera() { return require('./components/ViroCamera'); },
  get ViroOrbitCamera() { return require('./components/ViroOrbitCamera'); },
  get ViroSphere() { return require('./components/ViroSphere'); },
  get ViroImage() { return require('./components/ViroImage'); },
  get Viro360Photo() { return require('./components/Viro360Photo'); },
  get ViroSkyBox() { return require('./components/ViroSkyBox'); },
  get Viro3DObject() { return require('./components/Viro3DObject'); },
  get ViroAnimatedComponent() { return require('./components/ViroAnimatedComponent'); },
  get ViroAnimations() { return require('./components/Animation/ViroAnimations'); },
  get ViroDirectionalLight() { return require('./components/ViroDirectionalLight'); },
  get ViroFlexView() { return require('./components/ViroFlexView'); },
  get ViroUtils() { return require('./components/Utilities/ViroUtils'); },
}

module.exports = ReactViro;
