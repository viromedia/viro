/**
 * Copyright (c) 2018-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

/*
 This function is a simple helper function that attempts to check
 for misnamed props. Returns whether or not a prop was misnamed.

 The following components don't call this function (for efficency's sake), you can
 enable the checks by uncommenting the line in the respective render() functions. If
 you do uncomment the checks, don't forget to update this list:

 Viro3DSceneNavigator
 ViroAmbientLight
 ViroAnimatedComponent
 ViroCamera
 ViroController
 ViroDirectionalLight
 ViroOmniLight
 ViroOrbitCamera
 ViroScene
 ViroSceneNavigator
 ViroSpotLight
 ViroVRSceneNavigator
 AR/ViroARImageMarker
 AR/ViroARPlane
 AR/ViroARPlaneSelector
 AR/ViroARScene
 AR/ViroARSceneNavigator

 */
export function checkMisnamedProps(component, props) {

  var upperCaseComponent = component.toUpperCase();

  if (props.transformBehavior) {
    console.warn('The <' + component + '> component takes a `transformBehaviors` property rather than `transformBehavior`.');
    return true;
  }

  if (props.material) {
    console.warn('The <' + component + '> component takes a `materials` property rather than `material`.');
    return true;
  }

  if (props.src) {
    console.warn('The <' + component + '> component takes a `source` property rather than `src`.');
    return true;
  }

  if (props.dragPlane) {
    if (!props.dragPlane.planePoint) {
      console.warn('The <' + component + '> component with `dragPlane` property requires a `planePoint`.')
      return true;
    } else if (!props.dragPlane.planeNormal) {
      console.warn('The <' + component + '> component with `dragPlane` property requires a `planeNormal`.')
      return true;
    }
  }

  if (upperCaseComponent === 'ViroSkyBox'.toUpperCase()
      || upperCaseComponent === 'Viro360Image'.toUpperCase()
      || upperCaseComponent === 'Viro360Video'.toUpperCase()) {

    if (props.onHover) {
      console.warn('The <' + component + '> component does not take on an `onHover` property. Pass the `onHover` prop to <ViroScene> instead.');
      return true;
    }

    if (props.onClick) {
      console.warn('The <' + component + '> component does not take on an `onClick` property. Pass the `onClick` prop to <ViroScene> instead.');
      return true;
    }
  }

  return false;
};