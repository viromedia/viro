
/**
 * Copyright (c) 2016-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroTextStylePropTypes
 * @flow
 */
'use strict';

import ReactPropTypes from 'react';
import ColorPropType from 'react-native';

var ViroStylePropTypes = require('ViroPropTypes');


var ViroTextStylePropTypes =  Object.assign(Object.create(ViroStylePropTypes), {
  color: ColorPropType,
  backgroundColor: ColorPropType,
  fontFamily: ReactPropTypes.string,
  fontSize: ReactPropTypes.number,
  fontStyle: ReactPropTypes.oneOf(['normal', 'italic']),
  /**
   * Specifies font weight. The values 'normal' and 'bold' are supported for
   * most fonts. Not all fonts have a variant for each of the numeric values,
   * in that case the closest one is chosen.
   */
  fontWeight: ReactPropTypes.oneOf(
    ['normal' /*default*/, 'bold',
     '100', '200', '300', '400', '500', '600', '700', '800', '900']
  ),
  textShadowOffset: ReactPropTypes.shape(
    {width: ReactPropTypes.number, height: ReactPropTypes.number}
  ),
  textShadowRadius: ReactPropTypes.number,
  textShadowColor: ColorPropType,
  /**
   * @platform ios
   */
  letterSpacing: ReactPropTypes.number,
  lineHeight: ReactPropTypes.number,
  /**
   * Specifies text alignment. The value 'justify' is only supported on iOS and
   * fallbacks to `left` on Android.
   */
  textAlign: ReactPropTypes.oneOf(
    ['auto' /*default*/, 'left', 'right', 'center', 'justify']
  ),
  /**
   * @platform android
   */
  textAlignVertical: ReactPropTypes.oneOf(
    ['auto' /*default*/, 'top', 'bottom', 'center']
  ),
  textDecorationLine: ReactPropTypes.oneOf(
    ['none' /*default*/, 'underline', 'line-through', 'underline line-through']
  ),
  /**
   * @platform ios
   */
  textDecorationStyle: ReactPropTypes.oneOf(
    ['solid' /*default*/, 'double', 'dotted','dashed']
  ),
  /**
   * @platform ios
   */
  textDecorationColor: ColorPropType,
  /**
   * @platform ios
   */
  writingDirection: ReactPropTypes.oneOf(
    ['auto' /*default*/, 'ltr', 'rtl']
  ),
});

module.exports = ViroTextStylePropTypes;
