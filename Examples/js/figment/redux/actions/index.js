/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

var nextImageId = 0;

export function displayUIScreen(uiScreenToShow) {
  return {
    type: 'DISPLAY_UI_SCREEN',
    ui: uiScreenToShow,
  };
}
export function toggleModelSelection(index) {
  console.log("EXECUTING TOGGLE_MODEL_ITEM index:" + index);
  return {
    type: 'TOGGLE_MODEL_SELECTED',
    index: index,
  };
}

export function changeLoadState(index, loadState) {
  console.log("EXECUTING TOGGLE_MODEL_ITEM index:" + index);
  return {
    type: 'CHANGE_LOAD_STATE',
    index: index,
    loadState: loadState,
  };
}
