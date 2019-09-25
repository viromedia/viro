/**
 * Copyright (c) 2016-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroPropTypes
 * @flow
 */
'use strict';

/**
 * This file is derived from react-native's ViewStylePropTypes by removing
 * the props that we don't support.
 */

var ShadowPropTypesIOS = require('react-native/Libraries/DeprecatedPropTypes/DeprecatedShadowPropTypesIOS');
var LayoutPropTypes = require('react-native/Libraries/DeprecatedPropTypes/DeprecatedLayoutPropTypes');
/**
 * Warning: Some of these properties may not be supported in all releases.
 */
var ViroPropTypes = {
  ...LayoutPropTypes,
  ...ShadowPropTypesIOS,
};

module.exports = ViroPropTypes;
