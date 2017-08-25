/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

 import *  as UIConstants from '../UIConstants';

 const initialState = {
  currentScreen: UIConstants.SHOW_MAIN_SCREEN,
  listMode: UIConstants.LIST_MODE_MODEL,
  listTitle: UIConstants.LIST_TITLE_MODELS,
  currentItemSelectionIndex: '',
  currentItemClickState: '',
 }

function ui(state = initialState, action) {
  switch (action.type) {
    case 'DISPLAY_UI_SCREEN':
      return {
        ...state,
        currentScreen: action.ui,
      };
    case 'SWITCH_LIST_MODE':
      return {
        ...state,
        listMode: action.listMode,
        listTitle: action.listTitle,
      };
    case 'CHANGE_ITEM_CLICK_STATE':
      return {
        ...state,
        currentItemSelectionIndex: action.index,
        currentItemClickState: action.clickState,
        currentSelectedItemType: action.itemType,
      }
    default:
      return state;
  }
}

module.exports = ui;
