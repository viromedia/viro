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
  }
}

export function addModelWithIndex(index) {
  return {
      type:'ADD_MODEL',
      index: index,
  }
}

export function removeModelWithUUID(uuid) {
  return {
      type:'REMOVE_MODEL',
      uuid: uuid,
  }
}

export function toggleModelSelection(index) {
  console.log("EXECUTING TOGGLE_MODEL_ITEM index:" + index);
  return {
    type: 'TOGGLE_MODEL_SELECTED',
    index: index,
  };
}

export function toggleEffectSelection(index) {
  return {
    type: 'TOGGLE_EFFECT_SELECTED',
    index: index,
  };
}

export function togglePortalSelection(index) {
  console.log("EXECUTING TOGGLE PORTAL SELECTED index:" + index);
  return {
    type: 'TOGGLE_PORTAL_SELECTED',
    index: index,
  };
}

export function switchListMode(listMode, listTitle) {
  return {
    type: 'SWITCH_LIST_MODE',
    listMode: listMode,
    listTitle: listTitle,
  };
}

export function changeModelLoadState(uuid, loadState) {
  console.log("EXECUTING TOGGLE_MODEL_ITEM uuid:" + uuid);
  return {
    type: 'CHANGE_MODEL_LOAD_STATE',
    uuid: uuid,
    loadState: loadState,
  };
}

export function changePortalPhoto(index, photo) {
  return {
    type: 'CHANGE_PORTAL_PHOTO',
    index: index,
    photoSource: photo,
  }
}

export function changePortalLoadState(index, loadState) {
  console.log("EXECUTING CHANGE PORTAL LOAD STATE index:" + index);
  return {
    type: 'CHANGE_PORTAL_LOAD_STATE',
    index: index,
    loadState: loadState,
  };
}
export function changeItemClickState(index, clickState, itemType) {
  console.log("EXECUTING CHANGE MODEL CLICK STATE index:" + index);
  return {
    type: 'CHANGE_ITEM_CLICK_STATE',
    index: index,
    clickState: clickState,
    itemType: itemType,
  };
}
